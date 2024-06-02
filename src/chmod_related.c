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
        my_execvp(path, argv);
    } else if (pid < 0) {
        perror("lsh");
    } else {
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
        printf("usage: chmod [+/-x] PATH\n");
        return;
    } 

    if ((argv[0][0] != '-' && argv[0][0] != '+') || argv[0][1] != 'x' || argv[0][2] != '\0') {
        printf("usage: chmod [+/-x] PATH\n");
        return;
    } 

    if (argv[1] == NULL) {
        printf("usage: chmod [+/-x] PATH\n");
        return;
    }

    struct stat fileStat;
    if (stat(argv[1], &fileStat) != 0) {
        perror("error to get info");
        return;
    }

    mode_t new_mode;
    if (argv[0][0] == '-') {
        new_mode = fileStat.st_mode & ~(S_IXUSR | S_IXGRP | S_IXOTH);
    } else if (argv[0][0] == '+') {
        new_mode = fileStat.st_mode | (S_IXUSR | S_IXGRP | S_IXOTH);
    } else {
        printf("usage: chmod [+/-x] PATH\n");
        return;
    }

    if (chmod(argv[1], new_mode) != 0) {
        perror("error with permission");
    } else {
        printf("Permissions changed successfully\n");
    }
}