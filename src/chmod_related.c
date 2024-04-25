#include "chmod_related.h"

int isExecutable(char* path) {
    if(access(path, X_OK) == 0) {
        return 1;
    } else return 0;
}

void execute_file(char* path, char** argv) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        my_execvp(path, argv);
    } else if (pid < 0) {
        // Error forking
        perror("lsh");
    } else {
        // Parent process
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

void my_execvp(const char* path, char** argv) {
    int argc = 0;
    while (argv[argc] != NULL) {
        argc++;
    }
    
    argv[argc-1] = NULL;
    
    execvp(path, argv);
    
    perror("execvp error");
    exit(1);
}