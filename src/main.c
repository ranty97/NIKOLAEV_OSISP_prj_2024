#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include "text.h"
#include "parse.h"
#include "find.h"
#include "cd.h"
#include "cat_related.h"
#include "rm.h"
#include "mk.h"
#include "common.h"
#include "env.h"
#include "readline.h"
#include "chmod_related.h"
#include "history.h"
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 100
#define HISTORY_SIZE 100
#define BUFFER_SIZE 1024

FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);

char* history[HISTORY_SIZE];  
int history_count;

const char* env_filename = "/home/ivan/OSISP/Shell/enviroment.txt";

char *builtin_str[] = {
  "cd",
  "pwd",
  "echo",
  "find",
  "cat",
  "head",
  "tail",
  "rm",
  "touch",
  "mkdir",
  "clear", 
  "ls",
  "chmod",
  "history",
  "help"
};

void (*builtin_funcs[]) (int argc, char** argv) = {
    &cd_m,
    &pwd_m,
    &echo_m,
    &find_m,
    &catm,
    &headm,
    &tailm,
    &remutil,
    &touch_m,
    &make_dir,
    &cls,
    &ls,
    &chmod_m,
    &print_history,
    &help
};

bool hasEqualSign(const char* str) {
    while (*str != '\0') {
        if (*str == '=') {
            return true;
        }
        str++;
    }
    return false;
}

int count_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

void execute_m(char* command, int argc, char** argv) {
    int commandIndex = -1;

    if(strcmp(command, "exit") == 0) {
        exit(0);
    }

    for (int i = 0; i < count_builtins(); i++) {
        if(strcmp(builtin_str[i], command) == 0) {
            commandIndex = i;
            break;
        }
    }

    if (isExecutable(command)) {
        execute_file(command, argv);
    } else if (commandIndex == -1) {
        printf("command not found\n");
        return;
    } else {
        (*builtin_funcs[commandIndex])(argc, argv);
        return;
    }
}

void execute_command_and_grep(char *command, int argc, char **argv, const char *pattern) {
    int stdout_pipe[2], stderr_pipe[2];
    char buffer[128];
    pid_t pid;

    // Создаем пайпы для stdout и stderr
    if (pipe(stdout_pipe) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (pipe(stderr_pipe) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Создаем дочерний процесс
    if ((pid = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Дочерний процесс
        // Закрываем каналы чтения пайпов
        close(stdout_pipe[0]);
        close(stderr_pipe[0]);

        // Перенаправляем stdout и stderr на пайпы
        dup2(stdout_pipe[1], STDOUT_FILENO);
        dup2(stderr_pipe[1], STDERR_FILENO);

        // Закрываем записи пайпов, которые больше не нужны
        close(stdout_pipe[1]);
        close(stderr_pipe[1]);

        // Выполняем команду
        execute_m(command, argc, argv);

        exit(EXIT_SUCCESS);
    } else { // Родительский процесс
        FILE *grep_pipe;
        char grep_command[256];

        // Закрываем каналы записи пайпов
        close(stdout_pipe[1]);
        close(stderr_pipe[1]);

        // Формируем команду grep
        snprintf(grep_command, sizeof(grep_command), "grep %s", pattern);
        grep_pipe = popen(grep_command, "w");
        if (grep_pipe == NULL) {
            perror("popen");
            exit(EXIT_FAILURE);
        }

        // Читаем из пайпов и пишем в grep
        ssize_t bytes_read;
        while ((bytes_read = read(stdout_pipe[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0'; // добавляем терминальный нуль
            fputs(buffer, grep_pipe);
        }
        while ((bytes_read = read(stderr_pipe[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0'; // добавляем терминальный нуль
            fputs(buffer, grep_pipe);
        }

        // Закрываем пайпы и ждем завершения дочернего процесса
        close(stdout_pipe[0]);
        close(stderr_pipe[0]);
        pclose(grep_pipe);
        wait(NULL);
    }
}

void lee(char *command, int argc, char **argv, const char *output_file) {
    int stdout_pipe[2], stderr_pipe[2];
    char buffer[128];
    pid_t pid;

    // Создаем пайпы для stdout и stderr
    if (pipe(stdout_pipe) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (pipe(stderr_pipe) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Создаем дочерний процесс
    if ((pid = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Дочерний процесс
        // Закрываем каналы чтения пайпов
        close(stdout_pipe[0]);
        close(stderr_pipe[0]);

        // Перенаправляем stdout и stderr на пайпы
        dup2(stdout_pipe[1], STDOUT_FILENO);
        dup2(stderr_pipe[1], STDERR_FILENO);

        // Закрываем записи пайпов, которые больше не нужны
        close(stdout_pipe[1]);
        close(stderr_pipe[1]);

        // Выполняем команду
        execute_m(command, argc, argv);

        exit(EXIT_SUCCESS);
    } else { // Родительский процесс
        FILE *file = NULL;

        // Закрываем каналы записи пайпов
        close(stdout_pipe[1]);
        close(stderr_pipe[1]);

        // Если указан файл, открываем его для записи
        if (output_file != NULL) {
            file = fopen(output_file, "w");
            if (file == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
        }

        // Читаем из пайпов и пишем в файл или на консоль
        ssize_t bytes_read;
        while ((bytes_read = read(stdout_pipe[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0'; // добавляем терминальный нуль
            if (file != NULL) {
                fputs(buffer, file);
            } else {
                fputs(buffer, stdout);
            }
        }
        while ((bytes_read = read(stderr_pipe[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0'; // добавляем терминальный нуль
            if (file != NULL) {
                fputs(buffer, file);
            } else {
                fputs(buffer, stderr);
            }
        }

        // Закрываем пайпы
        close(stdout_pipe[0]);
        close(stderr_pipe[0]);

        // Закрываем файл, если он был открыт
        if (file != NULL) {
            fclose(file);
        }

        // Ждем завершения дочернего процесса
        wait(NULL);
    }
}

int main() {
    char* input;

    while (1) {
        input = get_input();
        if (input == NULL) {
            continue;
        }

        switch (contains_pipe(input)) {
            case 0: {
                ParsedInput parsed_input = parse_input(input);

                add_to_history(parsed_input.command, parsed_input.num_args, parsed_input.args);

                if (hasEqualSign(parsed_input.command)) {
                    saveVariableToFile(parsed_input.command);
                } else {
                    execute_m(parsed_input.command, parsed_input.num_args, parsed_input.args);
                }
                free_parsed_input(&parsed_input);
                break;
            }
            case 1: {
                ParsedPipeline pipeline = parse_pipeline(input);
                if (strcmp(pipeline.commands[1].command, "grep") == 0) {
                    execute_command_and_grep(pipeline.commands[0].command, pipeline.commands[0].num_args, pipeline.commands[0].args, pipeline.commands[1].args[0]);
                } else if (strcmp(pipeline.commands[1].command, "lee")== 0) {
                    lee(pipeline.commands[0].command, pipeline.commands[0].num_args, pipeline.commands[0].args, pipeline.commands[1].args[0]);
                } else {
                    printf("%s", "command doesn't support piping");
                }
                break;
            }
        }
        free(input);
    }

    return 0;
}
   

