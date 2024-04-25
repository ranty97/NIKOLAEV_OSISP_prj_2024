#ifndef CHMOD_RELATED_H

#define CHMOD_RELATED_H

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int isExecutable(char* path);
void execute_file(char* path, char** argv);
void my_execvp(const char* path, char** argv);

#endif