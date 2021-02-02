#ifndef _SHELL_H
#define _SHELL_H

#include "../lib.h"
#include "../shell/utils/shell_utils.h"
#include "../network/client/client.h"
#include "../structures/poi_list/poi_list.h"
#include "../utils/general_utils.h"
#include "../files/files.h"

#define ARGS_ERROR_SHELL 			"Error, number of arguments introduced not correct\n"
#define COMMAND_ERROR_SHELL         "Error, command introduced not recognized\n"

#define CONNECT_ERROR               "Error, you can't connect to yourself\n"
#define CONNECT_ERROR_PORT           "Error, ports need a numerical value of 4 integers\n"


#define RETRN       "                                                         \r"


void listenCommand(Configuration configuration);
int readArguments(char*** args);
int manageArguments(char** args, int argc, Configuration configuration, POIList *p);
void exit_handleler();
void exitTrinity();
void error(int type);

#endif