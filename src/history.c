#include "history.h"

char* strdup(const char* str) {
    size_t len = strlen(str) + 1;
    char* new_str = malloc(len);
    if (new_str == NULL) {
        fprintf(stderr, "Ошибка выделения памяти.\n");
        return NULL;
    }
    return memcpy(new_str, str, len);
}

void add_to_history(const char* command, int argc, char** argv) {
    // Создаем строку для сохранения команды и аргументов
    char buffer[BUFFER_SIZE];
    buffer[0] = '\0';

    // Добавляем команду в буфер
    strncat(buffer, command, BUFFER_SIZE - 1);

    // Добавляем аргументы в буфер
    for (int i = 0; i < argc; i++) {
        strncat(buffer, " ", BUFFER_SIZE - strlen(buffer) - 1);
        strncat(buffer, argv[i], BUFFER_SIZE - strlen(buffer) - 1);
    }

    // Проверяем, не превышен ли размер истории
    if (history_count < HISTORY_SIZE) {
        history[history_count] = strdup(buffer);
        history_count++;
    } else {
        free(history[0]);
        for (int i = 0; i < history_count - 1; i++) {
            history[i] = history[i + 1];
        }
        history[history_count - 1] = strdup(buffer);
    }
}

void print_history(int argc,  char** argv) {
    (void)argc;
    (void)argv;
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i + 1, history[i]);
    }
}