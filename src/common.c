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

void ls(int argc, char** argv) {
    (void)argc;
    (void)argv;

DIR *dir;
    struct dirent *entry;
    struct stat file_info;

    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir error");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; 
        }

        char *path = entry->d_name;

        if (lstat(path, &file_info) == -1) {
            perror("lstat error");
            continue;
        }

        if (S_ISDIR(file_info.st_mode)) {
            printf("%s%s%s ", GREEN, entry->d_name, RESET);
        } else if (S_ISREG(file_info.st_mode) && (file_info.st_mode & S_IXUSR)) {
            printf("%s%s%s ", RED, entry->d_name, RESET); 
        } else {
            printf("%s ", entry->d_name); 
        }
    }
    printf("\n");

    closedir(dir);
}