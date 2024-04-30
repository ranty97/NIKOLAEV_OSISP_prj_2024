#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include "parse.h"
#include "find.h"
#include "cd.h"
#include "cat_related.h"
#include "rm.h"
#include "mk.h"
#include "common.h"
#include "env.h"
#include "chmod_related.h"
#include "history.h"

#define MAX_INPUT_LENGTH 100
#define HISTORY_SIZE 100
#define BUFFER_SIZE 1024

char* history[HISTORY_SIZE];  // Глобальный массив для хранения истории команд
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
  "history"
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
    &print_history
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

int main() {
    history_count = 0;
    char input[MAX_INPUT_LENGTH];
    char cwd[1024];
    int num_commands = 0;

    while (1) {
        printf("\033[32minshell: \033[0m");
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s ", cwd);
        } else {
            perror("getcwd() error");
            return 1;
        }
        
        fgets(input, sizeof(input), stdin);
        if (input[0] == '\n') {
            continue;
        }
        input[strcspn(input, "\n")] = '\0';

        ParsedInput* commands = split_commands(input, &num_commands);

        for (int i = 0; i < num_commands; i++) {
        printf("Command %d: %s\n", i, commands[i].command);
        printf("Num Args: %d\n", commands[i].num_args);
        for (int j = 0; j < commands[i].num_args; j++) {
            printf("Arg %d: %s\n", j, commands[i].args[j]);
        }
        printf("\n");
    }

    // Освобождение памяти
    for (int i = 0; i < num_commands; i++) {
        free(commands[i].command);
        for (int j = 0; j < commands[i].num_args; j++) {
            free(commands[i].args[j]);
        }
    }
    free(commands);

        ParsedInput parsed_input = parse_input(input);

        add_to_history(parsed_input.command, parsed_input.num_args, parsed_input.args);

        if(hasEqualSign(parsed_input.command)){
            saveVariableToFile(parsed_input.command);
        } else {
            execute_m(parsed_input.command, parsed_input.num_args, parsed_input.args);
        }
        free_parsed_input(&parsed_input);
    }

    return 0;
}
   