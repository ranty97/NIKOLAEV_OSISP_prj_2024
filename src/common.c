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

void ls(int argc, char **argv) {
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

    int count = 0;
    char names[256][COLUMN_WIDTH * 2];  // массив для хранения имен файлов и директорий с учетом цветов

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
            snprintf(names[count], sizeof(names[count]), "%s%-*.*s%s", GREEN, COLUMN_WIDTH, COLUMN_WIDTH - 9, entry->d_name, RESET);
        } else if (S_ISREG(file_info.st_mode) && (file_info.st_mode & S_IXUSR)) {
            snprintf(names[count], sizeof(names[count]), "%s%-*.*s%s", RED, COLUMN_WIDTH, COLUMN_WIDTH - 9, entry->d_name, RESET);
        } else {
            snprintf(names[count], sizeof(names[count]), "%-*.*s", COLUMN_WIDTH, COLUMN_WIDTH, entry->d_name);
        }
        count++;
    }
    closedir(dir);

    // вывод в виде таблицы
    for (int i = 0; i < count; i++) {
        printf("%-*s", COLUMN_WIDTH, names[i]);
        if ((i + 1) % COLUMNS == 0) {
            printf("\n");
        }
    }
    if (count % COLUMNS != 0) {
        printf("\n");
    }
}


void help(int argc, char** argv) {
    (void)argc;
    (void)argv;
    printf("Доступные команды:\n");
    printf("- cd <directory>: Переход в указанную директорию\n");
    printf("- pwd: Вывод текущей директории\n");
    printf("- echo <text>: Вывод указанного текста\n");
    printf("- find <file/directory>: Поиск файла или директории\n");
    printf("- cat <file>: Вывод содержимого файла\n");
    printf("- head <file>: Вывод первых 10 строк файла\n");
    printf("- tail <file>: Вывод последних 10 строк файла\n");
    printf("- rm <file>: Удаление файла\n");
    printf("- touch <file>: Создание нового файла\n");
    printf("- mkdir <directory>: Создание новой директории\n");
    printf("- clear: Очистка экрана\n");
    printf("- ls: Список файлов и папок в текущей директории\n");
    printf("- chmod <permissions> <file/directory>: Изменение прав доступа\n");
    printf("- history: Вывод истории выполненных команд\n");
    printf("- grep <pattern> <file>: Поиск текста в файле\n");
    printf("- lee <file>: перенаправление в файл\n");
    printf("- help: Вывод этого справочного сообщения\n");
}