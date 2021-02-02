#ifndef _GENERAL_UTILS_H
#define _GENERAL_UTILS_H

#include "lib.h"

void freeMatrix(int argc, char*** args);
void freeObject(void** s);
char* execute_command(char* path, char* command, char* arg1);
void writeToScreen(char* text);
void inform(char* arg1, char* arg2, char* action, int is_server);
void printPrompt(char* name_machine);



#endif