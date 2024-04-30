#ifndef HISTORY_H
#define HISTORY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HISTORY_SIZE 100
#define BUFFER_SIZE 1024
extern char* history[HISTORY_SIZE];
extern int history_count;

void print_history(int argc, char** argv);
void add_to_history(const char* command, int argc, char** argv);

#endif 
