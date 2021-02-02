#ifndef _CLIENT_UTILITIES_H
#define _CLIENT_UTILITIES_H

#include "structures/poi_list/poi_list.h"
#include "network/network.h"
#include "utils/general_utils.h"



/************************************************** DEFAULT  **************************************************/
int connect_to_server(char *ip, int port); 

#define MSG_ERR_SOCKET      "Error during socket creation.\n"
#define MSG_ERR_CONN        "Error de connexion con el servidor.\n"

/************************************************** CUSTOM  **************************************************/

#define CONNECTIONS_AVAILABLE " connections available\n"

#define KO "KO\n"


void showPorts(int fd, int port);
int check_ack(int server_fd, char* type, char** header, char** data);



#endif