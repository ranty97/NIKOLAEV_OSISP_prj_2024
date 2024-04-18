#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "cd.h"

void cd_m(int argc, char **argv);
void exit_m(int argc, char **argv);
void echo_m(int argc, char **argv);
void pwd_m(int argc, char **argv);
void cls(int argc, char** argv);

#endif