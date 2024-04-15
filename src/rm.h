#ifndef RM_H
#define RM_H

#define _POSIX_SOURCE
#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED
#define _DEFAULT_SOURCE

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>


void remove_file(const char *path);
void remove_directory(const char *path);
void remutil(int argc, char** argv);

#endif