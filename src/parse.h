#ifndef PARSE_H
#define PARSE_H

#include <stdbool.h>

#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#define MAX_ARGS 10  
#define MAX_CMD_LENGTH 100
#define MAX_COMMANDS 5

typedef struct {
    char *command;
    char *args[MAX_ARGS];
    int num_args;
} ParsedInput;

typedef struct {
    ParsedInput commands[MAX_COMMANDS];
    int num_commands;
} ParsedPipeline;

ParsedInput parse_input(char *input);
void free_parsed_input(ParsedInput *parsed);
char *my_strdup(const char *s);
bool startWithDollar(char* str);
char* removeDollar(const char* str);
ParsedInput* split_commands(char* input, int* num_commands);
ParsedPipeline parse_pipeline(char *input);
int contains_pipe(const char *input);

#endif /* PARSE_H */
