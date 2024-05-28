#include "parse.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "env.h"

//const char* env_filename = "/home/ivan/OSISP/Shell/enviroment.txt";

ParsedInput parse_input(char *input) {
    ParsedInput parsed;
    parsed.num_args = 0;
    parsed.command = NULL;

    char *token = strtok(input, " \t\n");

    if (token != NULL) {
        parsed.command = my_strdup(token);
        if(startWithDollar(parsed.command)) {
            parsed.command = getVariableFromFile(removeDollar(parsed.command));
        }
        token = strtok(NULL, " \t\n");
        while (token != NULL) {
            if (parsed.num_args >= MAX_ARGS - 1) {
                fprintf(stderr, "Превышено максимальное количество аргументов\n");
                break;
            }
            if(startWithDollar(my_strdup(token))) {
                char* env_var = removeDollar(my_strdup(token));
                parsed.args[parsed.num_args] = getVariableFromFile(env_var);
                free(env_var);
            }else {
                parsed.args[parsed.num_args] = my_strdup(token);
                //printf("%s",parsed.args[parsed.num_args]);
            }
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

bool startWithDollar(char* str) {
    return (str[0] == '$');
}

char* removeDollar(const char* str) {
    char* newStr = my_strdup(str + 1);
    return newStr;
}


ParsedInput* split_commands(char* input, int* num_commands) {
    ParsedInput* commands = NULL;
    *num_commands = 0;

    char* command_start = input;
    char* pipe_position = strchr(input, '|');
    while (pipe_position != NULL) {
        if (*num_commands >= MAX_ARGS - 1) {
            fprintf(stderr, "Превышено максимальное количество команд\n");
            break;
        }

        ParsedInput parsed;
        parsed.num_args = 0;

        // Определяем длину команды
        size_t command_length = pipe_position - command_start;
        parsed.command = malloc((command_length + 1) * sizeof(char));
        strncpy(parsed.command, command_start, command_length);
        parsed.command[command_length] = '\0';

        char* arg_token = strtok(parsed.command, " \t\n");
        while (arg_token != NULL) {
            if (parsed.num_args >= MAX_ARGS - 1) {
                fprintf(stderr, "Превышено максимальное количество аргументов\n");
                break;
            }
            parsed.args[parsed.num_args] = strdup(arg_token);
            parsed.num_args++;
            arg_token = strtok(NULL, " \t\n");
        }

        commands = realloc(commands, (*num_commands + 1) * sizeof(ParsedInput));
        commands[*num_commands] = parsed;
        (*num_commands)++;

        // Перемещаем указатель на начало следующей команды
        command_start = pipe_position + 1;
        pipe_position = strchr(command_start, '|');
    }

    // Обработка последней команды после последнего символа "|"
    if (*command_start != '\0') {
        ParsedInput parsed;
        parsed.num_args = 0;

        // Определяем длину команды
        size_t command_length = strlen(command_start);
        parsed.command = malloc((command_length + 1) * sizeof(char));
        strcpy(parsed.command, command_start);

        char* arg_token = strtok(parsed.command, " \t\n");
        while (arg_token != NULL) {
            if (parsed.num_args >= MAX_ARGS - 1) {
                fprintf(stderr, "Превышено максимальное количество аргументов\n");
                break;
            }
            parsed.args[parsed.num_args] = strdup(arg_token);
            parsed.num_args++;
            arg_token = strtok(NULL, " \t\n");
        }

        commands = realloc(commands, (*num_commands + 1) * sizeof(ParsedInput));
        commands[*num_commands] = parsed;
        (*num_commands)++;
    }

    return commands;
}

ParsedPipeline parse_pipeline(char *input) {
    ParsedPipeline pipeline;
    pipeline.num_commands = 0;

    // Копируем input, чтобы strtok не изменял оригинал
    char *input_copy = my_strdup(input);

    char *saveptr;
    char *command_str = strtok_r(input_copy, "|", &saveptr);
    while (command_str != NULL) {
        if (pipeline.num_commands >= MAX_COMMANDS) {
            fprintf(stderr, "Превышено максимальное количество команд\n");
            break;
        }

        // Создаем копию команды для парсинга
        char *command_copy = my_strdup(command_str);
        ParsedInput parsed_command = parse_input(command_copy);
        pipeline.commands[pipeline.num_commands] = parsed_command;
        pipeline.num_commands++;

        free(command_copy);

        command_str = strtok_r(NULL, "|", &saveptr);
    }

    free(input_copy);
    return pipeline;
}

int contains_pipe(const char *input) {
    return strchr(input, '|') != NULL;
}
