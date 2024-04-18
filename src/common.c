#include "common.h"

void exit_m(int argc, char** argv) {
    (void)argc;
    (void)argv;
    exit(0);
}

void cd_m(int argc, char** argv) {
    if (argc == 1) {
            exec_cd(argv[0]);
    } else {
            fprintf(stderr, "Используйте: cd <путь>\n");
    }
}

void pwd_m(int argc, char** argv) {
    (void)argc;
    (void)argv;
    char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("Ошибка getcwd");
        }
}

void echo_m(int argc, char** argv) {
    for (int i = 0; i < argc; ++i) {
            printf("%s ", argv[i]);
    }
    printf("\n");
}

void cls(int argc, char** argv) {
    (void)argc;
    (void)argv;
    printf("\033[2J\033[H");
}