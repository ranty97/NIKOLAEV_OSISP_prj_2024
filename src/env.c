#include "env.h"

int environmentSize = 0;

void setVariable(const char* name, const char* value) {
    if (environmentSize >= MAX_VARIABLES) {
        printf("Ошибка: достигнуто максимальное количество переменных окружения\n");
        return;
    }

    strncpy(environment[environmentSize].name, name, MAX_NAME - 1);
    strncpy(environment[environmentSize].value, value, MAX_VALUE - 1);
    environment[environmentSize].name[MAX_NAME - 1] = '\0';
    environment[environmentSize].value[MAX_VALUE - 1] = '\0';

    environmentSize++;
}

void getVariable(const char* name) {
    for (int i = 0; i < environmentSize; i++) {
        if (strcmp(environment[i].name, name) == 0) {
            printf("%s=%s\n", environment[i].name, environment[i].value);
            return;
        }
    }

    printf("Переменная окружения не найдена\n");
}

void unsetVariable(const char* name) {
    for (int i = 0; i < environmentSize; i++) {
        if (strcmp(environment[i].name, name) == 0) {
            for (int j = i; j < environmentSize - 1; j++) {
                strcpy(environment[j].name, environment[j + 1].name);
                strcpy(environment[j].value, environment[j + 1].value);
            }
            environmentSize--;
            printf("Переменная окружения успешно удалена\n");
            return;
        }
    }

    printf("Переменная окружения не найдена\n");
}