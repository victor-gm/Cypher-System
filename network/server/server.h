#ifndef _SERVER_H
#define _SERVER_H

#include "manager/server_manager.h" // For manage_request
#include "utils/general_utils.h"

#define LISTEN_BACKLOG		64
#define MSG_ERR_BIND        "Error during port binding.\n"
#define MSG_ERR_SOCKET      "Error during socket creation.\n"

int server_run(char *ip, int port, char* ID_server_in, char* path);
void* server_thread();
void start_request_thread(int fd_client, char* ID_server, char* path);
int server_launch(int port, char *ip, int *socket_fd);
void close_server();


#endif
