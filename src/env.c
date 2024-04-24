#include "env.h"

void saveVariableToFile(char* variableString) {
    FILE* file = fopen(env_filename, "a");
    if (file == NULL) {
        printf("Ошибка при открытии файла для записи\n");
        return;
    }

    char* equalsSign = strchr(variableString, '=');
    if (equalsSign != NULL) {
        *equalsSign = '\0';
        char* name = variableString;
        char* value = equalsSign + 1;

        fprintf(file, "%s=%s\n", name, value);

        fclose(file);
        printf("Переменная успешно сохранена в файле\n");
    } else {
        printf("Ошибка: некорректный формат переменной\n");
        fclose(file);
    }
}

char* getVariableFromFile(const char* variableName) {
    FILE* file = fopen(env_filename, "r");
    if (file == NULL) {
        printf("Ошибка при открытии файла для чтения\n");
        return NULL;
    }

    char line[100];

    while (fgets(line, sizeof(line), file) != NULL) {
        // Удаление символа новой строки, если он присутствует
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        char* equalsSign = strchr(line, '=');
        if (equalsSign != NULL) {
            *equalsSign = '\0';
            char* name = line;
            char* value = equalsSign + 1;

            if (strcmp(name, variableName) == 0) {
                fclose(file);
                char* variableValue = (char*)malloc(strlen(value) + 1);
                strcpy(variableValue, value);
                return variableValue;
            }
        }
    }

    printf("Переменная не найдена в файле\n");
    fclose(file);
    return NULL;
}