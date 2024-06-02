#include "text.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CWD_LENGTH 1024

char* create_prompt(const char *text) {
    char cwd[MAX_CWD_LENGTH];
    char *prompt = NULL;
    
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        size_t prompt_size = strlen("\033[32minshell: \033[0m") + strlen(cwd) + strlen(text) + 3;
        prompt = (char*)malloc(prompt_size);
        
        if (prompt != NULL) {
            snprintf(prompt, prompt_size, "\033[32minshell: \033[0m%s %s", cwd, text);
        }
    } else {
        perror("getcwd() error");
        return NULL;
    }
    
    return prompt;
}

char* get_input() {
    char *prompt = create_prompt("> ");
    char *input = NULL;
    
    if (prompt != NULL) {
        input = readline(prompt);

        if (input && *input) {
            add_history(input);
        } else {
            if (input) {
                free(input);
                input = NULL;
            }
            input = NULL;
        }
        
        free(prompt);
    }
    
    return input;
}