#include "client_utilities.h"

int connect_to_server(char *ip, int port) 
{
    struct sockaddr_in s_addr;
    int socket_conn = -1;

    socket_conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (socket_conn < 0) write(1, MSG_ERR_SOCKET, sizeof(MSG_ERR_SOCKET));
    else {
        memset(&s_addr, 0, sizeof(s_addr)); //Init, necessary?
        s_addr.sin_family   = AF_INET;
        s_addr.sin_port     = htons(port);
        s_addr.sin_addr.s_addr = inet_addr(ip);

        if (connect(socket_conn, (void *) &s_addr, sizeof(s_addr)) < 0) {
            char buff[128];
            sprintf(buff, "errno says: %s\n", strerror(errno)); // molt útil
            writeToScreen(MSG_ERR_CONN);
            writeToScreen(buff);
			close(socket_conn);
            socket_conn = -1;
        }
    }
    return socket_conn;
}

/************************************************** WRITE YOUR AUXILIARY FUNCTIONS HERE **************************************************/

void showPorts(int fd, int port) 
{
    //Obtain a a string of ports from the pipe
    char** ports = NULL;
    char aux = '\0';
    int iPort, iPorts = 0;

    int nBytes = read(fd,&aux,1);
    if(nBytes != -1 || nBytes != 0)
    {  
        ports = (char**) malloc(sizeof(char*));
        while(nBytes != -1 && nBytes != 0)
        {
            iPort = -1;
            ports[iPorts] = (char*) malloc(sizeof(char));
            while(aux != '\n')
            {
                if(aux >= '0' && aux <='9')
                {
                    iPort++;
                    ports[iPorts] = (char*) realloc(ports[iPorts], sizeof(char) * (iPort + 2));
                    ports[iPorts][iPort] = aux;
                }
                nBytes = read(fd,&aux,1);
            }
            ports[iPorts][iPort + 1] = '\0';

             //Avoid self-connect
            int self = 0;
            if(atoi(ports[iPorts]) == port)
            {
                self = 1;
                freeString(&(ports[iPorts]));  
            }

            nBytes = read(fd,&aux,1);
            if( (nBytes != -1 || nBytes != 0) &&!self )
            {
                iPorts++;
                ports = (char**)realloc(ports,sizeof(char*) * (iPorts + 2));
            }      
        }
    }

    //Print the obtained ports
    int size = strlen(CONNECTIONS_AVAILABLE);
    char connections_available[1 + size + 1];
    sprintf(connections_available,"%d%s",iPorts,CONNECTIONS_AVAILABLE);
    writeToScreen(connections_available);
    
    for(int i = 0; i < iPorts; i++)
    {
        int port = atoi(ports[i]);
        char* ID = isElement_port(&client,port);
        char* buff;
        if(ID != NULL)
        {
            buff = malloc(strlen(ports[i]) + strlen(ID) + 3);
            sprintf(buff,"%s %s\n",ports[i],ID);
        }
        else
        {
            buff = (char*)malloc(strlen(ports[i]) + 2);
            sprintf(buff,"%s\n",ports[i]);
        }
        //write(1,buff,strlen(buff));
        writeToScreen(buff);
        freeString(&buff);
    }

    freeMatrix(iPorts, &ports);
}

// Checks if the server stills alive
int check_ack(int server_fd, char* type, char** header, char** data)
{
    int lenght = frameReader(type,header,data,server_fd);
    if( lenght == -1)
    {
        writeToScreen(ERROR_SERVER_DOWN);
        Connection* c = POIList_consult(&client);
        writeToScreen(c->ID);
        freeString(&(c->ID));
        POIList_delete(&client);
        *header = malloc(strlen(KO) + 1);
        strcpy(*header,KO);
    }
    return lenght;
}