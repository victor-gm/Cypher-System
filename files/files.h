#ifndef _FILES_H
#define _FILES_H

//#include "../structures/configuration.h"

#include "../structures/configuration.h"
#include "../lib.h"
#include "../utils/string_utils.h"
#include "../utils/general_utils.h"

#define FILE_NOT_FOUND_ERR "Error, the specified file doesn't exist\n"

int readConfigurationFile(char* path, Configuration* configuration);
void freeConfiguration(Configuration configuration);


#endif