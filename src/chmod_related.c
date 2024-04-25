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

void chmod_m(int argc,  char** argv) {
    if (argc < 2) {
        printf("usage: chmod [+/-x] PATH");
        return;
    } else if ((argv[0][0] != '-' || argv[0][0] != '+') && argv[0][1] != 'x') {
        printf("usage: chmod [+/-x] PATH"); 
        return;
    } else if (argv[1] == NULL) {
        printf("usage: chmod [+/-x] PATH"); 
        return;
    }

    struct stat fileStat;

    printf("%s", argv[1]);

    stat(argv[1], &fileStat);
    perror("error to get info");

    if (argv[0][0] == '-') {
        if (!isExecutable(argv[1])) {
            printf("nothing was done");
        } else {
            fileStat.st_mode &= ~(S_IXUSR | S_IXGRP | S_IXOTH);
            chmod(argv[1], fileStat.st_mode);
            perror("error with permition");
        }
    } else if (argv[0][0] == 'x') {
        if (isExecutable(argv[1])) {
            printf("nothing done");
        } else {
            fileStat.st_mode |= (S_IXUSR | S_IXGRP | S_IXOTH);
            chmod(argv[1], fileStat.st_mode);
            perror("error with permition");
        }
    }
}