#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define INITIAL_BUFFER_SIZE 128
#define KEY_BACKSPACE 127
#define KEY_LEFT 68
#define KEY_RIGHT 67

void move_cursor_left(int pos) {
    if (pos > 0) {
        printf("\033[D");
        fflush(stdout);
    }
}

void move_cursor_right(int pos, int len) {
    if (pos < len) {
        printf("\033[C");
        fflush(stdout);
    }
}

char *my_readline(const char *prompt) {
    printf("%s", prompt);
    fflush(stdout);

    int buffer_size = INITIAL_BUFFER_SIZE;
    char *buffer = (char *)malloc(buffer_size);
    if (!buffer) {
        perror("Unable to allocate buffer");
        exit(1);
    }

    int length = 0;
    int position = 0;

    while (1) {
        int ch = getchar();

        if (ch == '\n') {
            buffer[length] = '\0';
            printf("\n");
            return buffer;
        } else if (ch == 27) { // Escape sequence
            if (getchar() == '[') {
                switch (getchar()) {
                    case KEY_LEFT:
                        move_cursor_left(position);
                        if (position > 0) position--;
                        break;
                    case KEY_RIGHT:
                        move_cursor_right(position, length);
                        if (position < length) position++;
                        break;
                }
            }
        } else if (ch == KEY_BACKSPACE || ch == 8) {
            if (position > 0) {
                memmove(buffer + position - 1, buffer + position, length - position);
                position--;
                length--;
                buffer[length] = '\0';

                printf("\033[D \033[D"); // Move cursor back, print space, move back again
                printf("%s ", buffer + position);
                for (int i = 0; i <= length - position; i++) {
                    printf("\033[D");
                }
                fflush(stdout);
            }
        } else {
            if (length >= buffer_size - 1) {
                buffer_size *= 2;
                buffer = (char *)realloc(buffer, buffer_size);
                if (!buffer) {
                    perror("Unable to reallocate buffer");
                    exit(1);
                }
            }

            memmove(buffer + position + 1, buffer + position, length - position);
            buffer[position] = ch;
            position++;
            length++;
            buffer[length] = '\0';

            // printf("%s", buffer + position - 1);
            // for (int i = 0; i < length - position; i++) {
            //     printf("\033[D");
            // }
            // fflush(stdout);
        }
    }
}
