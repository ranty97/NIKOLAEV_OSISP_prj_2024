#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cat_related.h"

#define DEFAULT_LINES 10
#define MAX_LINE_LENGTH 1024

void catm(int argc, char **argv) {
    if (argc < 1) {
        printf("Usage: cat [file...]\n");
        return;
    }

    for (int i = 0; i < argc; i++) {
        FILE *file = fopen(argv[i], "r");
        if (file == NULL) {
            printf("cat: %s: No such file or directory\n", argv[i]);
        } else {
            int ch;
            while ((ch = fgetc(file)) != EOF) {
                putchar(ch);
            }
            printf("\n");
            fclose(file);
        }
    }
}


void headm(int argc, char **argv) {
    int lines = DEFAULT_LINES;
    char *filename = argv[0];

    if (argc < 1) {
        fprintf(stderr, "Usage: head [-n lines] <filename>\n");
        return;
    }

    if (argc == 3 && strcmp(argv[0], "-n") == 0) {
        lines = atoi(argv[1]);
        if (lines <= 0) {
            fprintf(stderr, "Invalid number of lines: %s\n", argv[1]);
            return;
        }

        filename = argv[2];
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(line, sizeof(line), file) != NULL && count < lines) {
        printf("%s", line);
        count++;
    }
    printf("\n");

    fclose(file);
}

void tailm(int argc, char **argv) {
    int lines = DEFAULT_LINES;
    char *filename = argv[0];

    if (argc < 1) {
        fprintf(stderr, "Usage: tail [-n lines] <filename>\n");
        return;
    }

    if (argc == 3 && strcmp(argv[0], "-n") == 0) {
        lines = atoi(argv[1]);
        if (lines <= 0) {
            fprintf(stderr, "Invalid number of lines: %s\n", argv[1]);
            return;
        }
        filename = argv[2];
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int count = 0;

    fseek(file, 0, SEEK_END);

    long offset = ftell(file);
    while (offset > 0 && count < lines) {
        fseek(file, --offset, SEEK_SET);
        if (fgetc(file) == '\n') {
            count++;
        }
    }
    fseek(file, offset, SEEK_SET);

    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }
    printf("\n");

    fclose(file);
}
