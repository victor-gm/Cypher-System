#ifndef _NETWORK_H
#define _NETWORK_H

#include "lib.h"
#include "utils/string_utils.h"
#include "structures/poi_list/poi_list.h"
#include "utils/general_utils.h"


#define TYPE_CONNECTION 0x01
#define TYPE_SAY 0x02
#define TYPE_SAY_BROADCAST 0x03
#define TYPE_SHOW_AUDIOS 0x04
#define TYPE_DOWNLOAD_AUDIO 0x05
#define TYPE_EXIT 0x06

//HEADERS
#define HEADERS_CONNECTION_REQUEST "[TR_NAME]\0"
#define HEADERS_CONNECTION_OK "[CONOK]\0" //Also for EXIT
#define HEADERS_CONNECTION_KO "[CONKO]\0"

#define HEADERS_SAY_REQUEST "[MSG]\0"
#define HEADERS_BROADCAST_REQUEST "[BROADCAST]\0"
#define HEADERS_SAY_OK "[MSGOK]\0"
#define HEADERS_SAY_KO "[MSGKO]\0"

#define HEADERS_SHOW_AUDIOS_REQUEST "[SHOW_AUDIOS]\0"
#define HEADERS_LIST_AUDIOS "[LIST_AUDIOS]\0"

#define HEADERS_AUDIO_REQUEST "[AUDIO_RQST]\0"
#define HEADERS_AUDIO_RESPONSE "[AUDIO_RSPNS]\0"
#define HEADERS_AUDIO_KO "[AUDIO_KO]\0"
#define HEADERS_EOF "[EOF]\0"
#define HEADERS_MD5_OK "[MD5OK]\0"
#define HEADERS_MD5_KO "[MD5KO]\0"

#define HEADERS_EXIT "[]\0"

#define ERROR_SERVER_DOWN "Error, server is down! Removing it from the list of connected servers: \0"

int frameGenerator(char type, char* header, unsigned short lenght, char* data, int conn_fd);
int frameReader(char* type, char** header, char** data, int conn_fd);

int isElement(POIList *p, char* user);
char* isElement_port(POIList *p, int port);
int POIList_obtain_fd(POIList *p, char* user);
/************************************************** DEFAULT  **************************************************/

typedef struct socket 
{
    int port;
    char* ip;
    char* ID_server; // Additional info needed by the server
}Socket;

/*
* Struct for all the connections:
* Case 1: A connection it's accepted and the ID of the server and the file descriptor of the socket is sent to the thread that will talk to the client
* Case 2: The thread in charge of talking to the client saves to the POIList the ID of the client with its file descriptor
*/
typedef struct connection //Struct for all the connections
{
    char* ID; 
    int conn_fd;
    int port; /// Need for retrieving the name is show connections
    char* path; //Extra! Need for this particular case
}Connection;

void freeSocket(Socket** s);
#endif