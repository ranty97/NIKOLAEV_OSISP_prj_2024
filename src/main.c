#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "parse.h"
#include "find.h"
#include "cd.h"

#define MAX_INPUT_LENGTH 100

void execute_command(ParsedInput *parsed_input) {
    if (strcmp(parsed_input->command, "exit") == 0) {
        exit(0);
    } else if (strcmp(parsed_input->command, "cd") == 0) {
        if (parsed_input->num_args == 1) {
            exec_cd(parsed_input->args[0]);
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
        pid_t pid = fork();
        if (pid == 0) { 
            char *args[MAX_ARGS + 2]; 
            args[0] = parsed_input->command;
            for (int i = 0; i < parsed_input->num_args; ++i) {
                args[i+1] = parsed_input->args[i];
            }
            args[parsed_input->num_args + 1] = NULL;
            execvp(parsed_input->command, args);
            exit(1);
        } else if (pid < 0) { 
            perror("Ошибка fork");
        } else { 
            wait(NULL); 
        }
    }
}

int main() {
    char input[MAX_INPUT_LENGTH];
    char cwd[1024];

    while (1) {
        printf("\033[32minshell:\033[0m");
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s \033[32m$\033[0m ", cwd);
        } else {
            perror("getcwd() error");
            return 1;
        }
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        ParsedInput parsed_input = parse_input(input);
        execute_command(&parsed_input);
        free_parsed_input(&parsed_input);
    }

    return 0;
}
