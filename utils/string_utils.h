#ifndef _STRING_UTILS_H
#define _STRING_UTILS_H

#include "../lib.h"

#define ERROR_STRING        "Error, an string is needed. To put a string make sure to wrap it around \"\n"

char* readUntil(int fd, char end);
int checkString(char** string);
void freeString(char** s); 

#endif 