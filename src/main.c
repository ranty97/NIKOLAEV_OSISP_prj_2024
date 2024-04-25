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

#define MAX_INPUT_LENGTH 100

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
  "ls"
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
    &ls
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
    char input[MAX_INPUT_LENGTH];
    char cwd[1024];

    while (1) {
        printf("\033[32minshell: \033[0m");
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s ", cwd);
        } else {
            perror("getcwd() error");
            return 1;
        }
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        ParsedInput parsed_input = parse_input(input);

        if(hasEqualSign(parsed_input.command)){
            saveVariableToFile(parsed_input.command);
        } else {
            execute_m(parsed_input.command, parsed_input.num_args, parsed_input.args);
        }
        free_parsed_input(&parsed_input);
    }

    return 0;
}
   