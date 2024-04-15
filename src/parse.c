#include "parse.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ParsedInput parse_input(char *input) {
    ParsedInput parsed;
    parsed.num_args = 0;
    parsed.command = NULL;

    char *token = strtok(input, " \t\n");

    if (token != NULL) {
        parsed.command = my_strdup(token);
        token = strtok(NULL, " \t\n");
        while (token != NULL) {
            if (parsed.num_args >= MAX_ARGS - 1) {
                fprintf(stderr, "Превышено максимальное количество аргументов\n");
            //printf("Аргумент %d: %s\n", parsed.num_args, parsed.args[parsed.num_args - 1]);    break;
            }
            parsed.args[parsed.num_args] = my_strdup(token);
            parsed.num_args++;
            token = strtok(NULL, " \t\n");
        }
    }

    return parsed;
}

void free_parsed_input(ParsedInput *parsed) {
    free(parsed->command);
    for (int i = 0; i < parsed->num_args; ++i) {
        free(parsed->args[i]);
    }
}

char *my_strdup(const char *s) {
    size_t len = strlen(s) + 1;
    char *copy = malloc(len);
    if (copy) {
        memcpy(copy, s, len);
    }
    return copy;
}
