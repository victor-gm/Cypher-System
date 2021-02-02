#ifndef _CLIENT_H
#define _CLIENT_H

#include "utils/client_utilities.h"
#include "structures/configuration.h"
#include "network/network.h"
#include "utils/string_utils.h"
#include "utils/general_utils.h"


#define SHOW_CONNECTIONS                "Testing...\n"

#define CONNECT                         "Connecting...\n"
#define AUDIOS                          "Audios\n"
#define DOWNLOADED                      " downloaded"
#define EXIT                            "Disconencting Trinity...\n"

#define ERROR_STRING                    "Error, an string is needed. To put a string make sure to wrap it around \"\n"

#define ERROR_FORK                      "Error, Fork couldn't be done\n"
#define ERROR_PIPE                      "Error, Pipe couldn't be done\n"

#define ERROR_CONNECT                   "Error. couldn't stablish connection with the server\n"
#define ERROR_CONNECT_ALREADY_CONNECTED "Error, you are already connected to this server\n"

#define ERROR_USER_NOT_FOUND            "Error, user could not be found\n"

#define ERROR_AUDIO_NOT_FOUND            "Error, audio could not be found in the server\n"

#define FILE_NOT_CREATED                 "Error, couldn't create file\n"


  

#define WORKING_IP "127.0.0.1"  // Note, it should be Configuration->ip, but since it's the same machine...

void showConnections(Configuration configuration); //TODO think about passing it as a pointer
void connect_cypher (char* arg, char** user, POIList *p);
void say (char* arg1, char** arg2, POIList *p);
void broadcast (char** arg1);
void showAudios(char* arg1);
void download(char* arg1, char* arg2, char* path);
void disconnect_from_servers();


#endif
