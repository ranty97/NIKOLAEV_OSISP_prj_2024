#include "mk.h"

void touch_m(int argc, char** argv) {
    if (argc < 1) {
        printf("Использование: touch <файл1> [<файл2> ...]\n");
        return;
    }

    for (int i = 0; i < argc; i++) {
        FILE *file = fopen(argv[i], "a");
        if (file == NULL) {
            printf("Ошибка: не удалось создать файл %s\n", argv[i]);
            continue;
        }

        fclose(file);
        utime(argv[i], NULL);
    }
}

void make_dir(int argc, char** argv) {
    if (argc < 1) {
        printf("Использование: mkdir <директория1> [<директория2> ...]\n");
        return;
    }

    for (int i = 0; i < argc; i++) {
        if (mkdir(argv[i], 0777) != 0) {
            printf("Ошибка: не удалось создать директорию %s\n", argv[i]);
        }
    }
}