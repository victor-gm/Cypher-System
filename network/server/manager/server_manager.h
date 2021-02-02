#ifndef _SERVER_MANAGER_H
#define _SERVER_MANAGER_H

#define CONNECTED                         " has connected \n"
#define DISCONNECTED                      " has disconnected successfuly\n"
#define REQUESTED_AUDIOS                  " has requested audios\n"
#define DOWNLOADED_AUDIOS_SUCCESSFULY     " has downloaded audios successfuly\n"
#define DOWNLOADED_AUDIOS_UNSUCCESSFULY   " has downloaded audios unsuccessfuly\n"


#include "lib.h"
#include "structures/poi_list/poi_list.h"
#include "network/network.h"
#include "utils/general_utils.h"


pthread_mutex_t connections;
int out;


void* manage_request(void* args);
int connect_server(char* data,  char*user, POIList *p, int fd_client);
void say_server(char* client_name, char* msg, int fd_client, int type);
void show_audio_server(int client_fd, char* path);
void download_audio_server(char* path, int client_fd, char* data);
void exit_server_from_client(char* client_name);
void close_server();
void free_server();



#endif