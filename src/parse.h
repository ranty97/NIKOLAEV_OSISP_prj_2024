#ifndef PARSE_H
#define PARSE_H

#define MAX_ARGS 10  
#define MAX_CMD_LENGTH 100  

typedef struct {
    char *command;
    char *args[MAX_ARGS];
    int num_args;
} ParsedInput;

ParsedInput parse_input(char *input);
void free_parsed_input(ParsedInput *parsed);
char *my_strdup(const char *s);

#endif /* PARSE_H */
