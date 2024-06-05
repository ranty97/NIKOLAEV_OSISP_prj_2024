#include "cd.h"

void exec_cd(char *arg) {
    char curdir[256];
    char path[256];
    static char lastdir[256] = "";

    // Получение текущей директории
    if (getcwd(curdir, sizeof(curdir)) == NULL) {
        perror("getcwd");
        return;
    }

    // Если аргумент пустой, переходим в домашний каталог
    if (arg == NULL) {
        arg = getenv("HOME");
    } else if (strcmp(arg, "-") == 0) {
        // Если аргумент "-", переходим в последнюю директорию
        if (*lastdir == '\0') {
            fprintf(stderr, "no previous directory\n");
            return;
        }
        arg = lastdir;
    } else if (arg[0] == '~') {
        // Обработка '~'
        if (arg[1] == '/' || arg[1] == '\0') {
            snprintf(path, sizeof(path), "%s%s", getenv("HOME"), arg + 1);
            arg = path;
        } else {
            fprintf(stderr, "syntax not supported: %s\n", arg);
            return;
        }
    }

    // Переход в указанную директорию
    if (chdir(arg) != 0) {
        fprintf(stderr, "chdir: %s: %s\n", strerror(errno), arg);
        return;
    }

    // Сохранение предыдущей директории
    strcpy(lastdir, curdir);
}