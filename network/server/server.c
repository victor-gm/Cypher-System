#include "server.h"

int socket_fd; // Open Socket for the server to receive petitions
char* ID_server; // Name of the server

// Launches a sever and launches a waiting server thread
int server_run(char *ip, int port, char* ID_server_in, char* path) 
{    
    ID_server = ID_server_in;
    int binded = 0;

    //Creation of the socket
    if(server_launch(port,ip,&socket_fd))
    {
        //We mark the binded flag
        binded = 1;

        //Creation of the list that the server will use to save all clients
        server = POIList_create();

        pthread_t serverID;

	    pthread_create(&serverID, NULL, server_thread, path);
        pthread_detach(serverID);
    }
    return binded;
}

// Waits for a client to connect and starts a thread to process the clients requests
void* server_thread(void* args)
{
    char* path = (char*)args;

    // Run server and start accepting requests from the clients
    struct sockaddr_in c_addr;
    socklen_t c_len = sizeof (c_addr);

    int fd_client; 
    while(1)
    {
        fd_client = accept(socket_fd, (void *) &c_addr, &c_len); // TODO NULL?
        start_request_thread(fd_client,ID_server,path);
    }
       
    return NULL;
}

// Creates the necessary structure for the seerver_manager thread and starts its
void start_request_thread(int fd_client, char* ID_server, char* path)
{
    Connection *c = malloc(sizeof(Connection));
    c->conn_fd = fd_client;

    int ID_server_size = strlen (ID_server) +1;
    c->ID = malloc(ID_server_size);
    memcpy(c->ID, ID_server, ID_server_size); //TODO: Revisar, igual innecesario

    c->path = path;

    pthread_t thread; // I don't need to refer later to the thread so I don't need to keep the IDs
    pthread_create(&thread, NULL, manage_request,c);
    pthread_detach(thread); 
}

// Launches the server
int server_launch(int port, char *ip, int *socket_fd)
{
    struct sockaddr_in s_addr;
    
    *socket_fd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (*socket_fd < 0)
        writeToScreen(MSG_ERR_SOCKET);
    else 
    {
        bzero (&s_addr, sizeof (s_addr));
        s_addr.sin_family = AF_INET;
        s_addr.sin_port = htons (port);
        s_addr.sin_addr.s_addr = inet_addr(ip);

        if (bind (*socket_fd, (void *) &s_addr, sizeof (s_addr)) < 0)
        {
            writeToScreen(MSG_ERR_BIND);
        }
        else {
         	listen(*socket_fd, LISTEN_BACKLOG);
            return 1;
        }
    }
    return 0;
}

void close_server()
{
    close(socket_fd);
}

