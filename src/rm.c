#include "rm.h"

void remove_file(const char* path) {
    if (remove(path) == 0) {
        printf("Удален файл: %s\n", path);
    } else {
        perror("Ошибка при удалении файла");
    }
}

void remove_directory(const char* path) {
    DIR* dir = opendir(path);
    size_t path_len = strlen(path);

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        size_t entry_path_len = path_len + strlen(entry->d_name) + 2;
        char* entry_path = (char*)malloc(entry_path_len);
        snprintf(entry_path, entry_path_len, "%s/%s", path, entry->d_name);

        struct stat entry_info;
        if (lstat(entry_path, &entry_info) == 0) {
            if (S_ISDIR(entry_info.st_mode)) {
                remove_directory(entry_path);
            } else {
                remove_file(entry_path);
            }
        }

        free(entry_path);
    }

    closedir(dir);

    if (rmdir(path) == 0) {
        printf("Удалена директория: %s\n", path);
    } else {
        perror("Ошибка при удалении директории");
    }
}

void remutil(int argc, char* argv[]) {
    if (argc < 1) {
        printf("Использование: rm <файлы/директории>\n");
        return;
    }

    int opt;
    int recursive = 0;

    while ((opt = getopt(argc, argv, "r")) != -1) {
        switch (opt) {
            case 'r':
                recursive = 1;
                break;
            default:
                printf("Неизвестный флаг: -%c\n", opt);
                return;
        }
    }


    if (argc >= 2 && strcmp(argv[0], "-r") == 0) {
        recursive = 1;
    }

    for (int i = 0; i < argc; i++) {
        const char* path = argv[i];
        struct stat path_info;
        printf("tutuk");
        if (lstat(path, &path_info) == 0) {
            printf("tut");
            if (S_ISDIR(path_info.st_mode) && recursive) {
                printf("chego");
                remove_directory(path);
            } else {
                printf("dfnjsdhfiusdhfiushfiuh");
                remove_file(path);
            }
        }
    }
}