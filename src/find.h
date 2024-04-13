#ifndef FIND_H
#define FIND_H

#define _POSIX_SOURCE
#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

struct Options {
    char isDirectory;
    char isLink;
    char isFile;
    char sort;
    char* dir;
};

struct Options parseOptions(int argc, char** argv);

void dirWalk(char *path, struct Options *options);

#endif /* FIND_H */