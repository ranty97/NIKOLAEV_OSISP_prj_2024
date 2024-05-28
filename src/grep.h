#ifndef GREP_H
#define GREP_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

void execute_command_and_grep(char *command, int argc, char **argv, const char *pattern);
#endif