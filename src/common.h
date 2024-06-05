#ifndef COMMON_H
#define COMMON_H

#define _POSIX_SOURCE
#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include "cd.h"

#ifdef _WIN32
    #define RESET   ""
    #define GREEN   ""
    #define RED     ""
#else
    #define RESET   "\033[0m"
    #define GREEN   "\033[32m"
    #define RED     "\033[31m"
#endif

#define COLUMN_WIDTH 20
#define COLUMNS 4
#define MAX_FILENAME_LENGTH (COLUMN_WIDTH - 10)

void cd_m(int argc, char **argv);
void exit_m(int argc, char **argv);
void echo_m(int argc, char **argv);
void pwd_m(int argc, char **argv);
void cls(int argc, char** argv);
void ls(int argc, char** argv);
void help(int argc, char** argv);

#endif