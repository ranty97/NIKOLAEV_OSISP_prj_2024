#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "parse.h"
#include "find.h" // Включаем заголовочный файл для команды find

#define MAX_INPUT_LENGTH 100

void execute_command(ParsedInput *parsed_input) {
    // Проверяем встроенные команды
    if (strcmp(parsed_input->command, "exit") == 0) {
        exit(0);
    } else if (strcmp(parsed_input->command, "cd") == 0) {
        if (parsed_input->num_args == 1) {
            chdir(parsed_input->args[0]);
        } else {
            fprintf(stderr, "Используйте: cd <путь>\n");
        }
    } else if (strcmp(parsed_input->command, "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("Ошибка getcwd");
        }
    } else if (strcmp(parsed_input->command, "echo") == 0) {
        for (int i = 0; i < parsed_input->num_args; ++i) {
            printf("%s ", parsed_input->args[i]);
        }
        printf("\n");
    } else if (strcmp(parsed_input->command, "find") == 0) {
        struct Options options = parseOptions(parsed_input->num_args, parsed_input->args);
        dirWalk(options.dir, &options);
    } else {
        // Выполняем команду с помощью execvp
        pid_t pid = fork();
        if (pid == 0) { // Это дочерний процесс
            char *args[MAX_ARGS + 2]; // +2 для имени команды и NULL в конце
            args[0] = parsed_input->command;
            for (int i = 0; i < parsed_input->num_args; ++i) {
                args[i+1] = parsed_input->args[i];
            }
            args[parsed_input->num_args + 1] = NULL;
            execvp(parsed_input->command, args);
            // Если execvp() вернул ошибку, печатаем сообщение об ошибке
            perror("Ошибка execvp");
            exit(1);
        } else if (pid < 0) { // Ошибка при создании дочернего процесса
            perror("Ошибка fork");
        } else { // Это родительский процесс
            wait(NULL); // Ждем завершения дочернего процесса
        }
    }
}

int main() {
    char input[MAX_INPUT_LENGTH];

    while (1) {
        printf("myshell> ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // Удаляем символ новой строки из ввода

        ParsedInput parsed_input = parse_input(input);
        execute_command(&parsed_input);
        free_parsed_input(&parsed_input);
    }

    return 0;
}
