#ifndef ENV_H
#define ENV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VARIABLES 100
#define MAX_NAME 50
#define MAX_VALUE 50

struct EnvironmentVariable {
    char name[MAX_NAME];
    char value[MAX_VALUE];
};

struct EnvironmentVariable environment[MAX_VARIABLES];

#endif 