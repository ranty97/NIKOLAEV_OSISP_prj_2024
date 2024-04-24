#ifndef ENV_H
#define ENV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern const char* env_filename;

void saveVariableToFile(char* variableString);
char* getVariableFromFile(const char* variableName);

#endif 