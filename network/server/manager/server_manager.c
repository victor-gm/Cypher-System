#include "server_manager.h"

char type;
char* header = NULL;
char* data = NULL;

char* server_name;
char client_name [100];

// Reads a frame sent by the client and executes the correspondant function
void* manage_request(void* args)
{   
    Connection *c = (Connection*)args;
    int fd_client = c->conn_fd;

    int ID_server_size = strlen(c->ID) +1;
    char ID_server [ID_server_size];
    memcpy(ID_server, c->ID, ID_server_size);

    char* path = c->path;

    free(c->ID);
    free(c);
    c= NULL;

    /********************************* WRITE YOUR CODE FOR THE SERVER HERE *********************************/
    server_name = ID_server;
    //client_name =  NULL;
    
    while(frameReader(&type,&header,&data,fd_client) != -1)
    {
        out = 0;
        switch(type)
        {
            case TYPE_CONNECTION:
                pthread_mutex_lock(&connections);
                    if(!connect_server(data, server_name, &server, fd_client)) out = 1;
                pthread_mutex_unlock(&connections);
                break;
            case TYPE_SAY:
                say_server(client_name, data, fd_client,0);
                break;
            case TYPE_EXIT:
                pthread_mutex_lock(&connections);
                    exit_server_from_client(data);
                    out = 1;
                pthread_mutex_unlock(&connections);
                break;
            case TYPE_SHOW_AUDIOS:
                show_audio_server(fd_client, path);
                break;
            case TYPE_DOWNLOAD_AUDIO:
                download_audio_server(path, fd_client, data);
                break;
            case TYPE_SAY_BROADCAST:
                say_server(client_name, data, fd_client,1); 
                break;
            default:
                writeToScreen("Option not detected!\n");
                break;
        }
        free_server();

        if(out == 1) break;
    }
    return NULL; 
}

// Stablishes connection with a client
int connect_server(char* data, char* server_name, POIList *p, int fd_client) //Maybe the list is not what I'm looking for
{
    Connection c;

    c.conn_fd = fd_client;

    int client_name_size = strlen(data) + 1;
    c.ID = malloc(client_name_size);
    memcpy(c.ID, data, client_name_size);
    strcpy(client_name,data);
    

    if(!isElement(p,c.ID))
    {
        if(!POIList_insert(p,&c,sizeof(c))) // Error in allocating space for this connection
        {
            frameGenerator(TYPE_CONNECTION, HEADERS_CONNECTION_KO, strlen(server_name) + 1, NULL, fd_client); 
            return 0;  
            //close(fd_client);
        }
    }
    frameGenerator(TYPE_CONNECTION, HEADERS_CONNECTION_OK, strlen(server_name) + 1, server_name, fd_client);
    //freeString(&(c.ID)); // We free the name of the client since we are memcpying in the POIList

    inform(data,server_name,CONNECTED,1);
    return 1;
}

// Displays a message sent by the client
void say_server(char* client_name, char* msg, int fd_client, int type)
{
    char* buff = malloc(strlen(client_name) + strlen(msg) + 5);
    sprintf(buff,"\n[%s]:%s",client_name,msg);
    writeToScreen(buff);
    freeString(&buff);

    if(type == 0)frameGenerator(TYPE_SAY, HEADERS_SAY_OK, 0 , NULL, fd_client);
    else frameGenerator(TYPE_SAY_BROADCAST, HEADERS_SAY_OK, 0, NULL, fd_client);

    printPrompt(server_name);
}

// Sents the list of audios present in the server
void show_audio_server(int client_fd, char* path)
{
    //Obtain files list
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d)
    {
        dir = readdir(d); // we eat out the . and ..
        dir = readdir(d); // we eat out the . and ..

        char** files = malloc(1);
        int i = 0;
        while ((dir = readdir(d)) != NULL)
        {
            files = realloc(files,sizeof(char*) * (i+1));

            int size = strlen(dir->d_name) + 2;
            files[i] = malloc(size);
            strcpy(files[i],dir->d_name);
            files[i][size - 2] = '\n';
            files[i][size - 1] = '\0';
            i++;
        }
        closedir(d);

        // Generate the frame for the client
        char* buff = calloc(1,1);
        for(int i2 = 0; i2 < i; i2++ )
        {
            buff = realloc(buff, strlen(buff) + strlen(files[i2]) + 1 );
            strcat(buff, files[i2]);
        }

        //Sent the frame
        frameGenerator(TYPE_SHOW_AUDIOS, HEADERS_LIST_AUDIOS, strlen(buff) + 1, buff, client_fd);
        freeString(&buff);
        freeMatrix(i,&files);

        inform(client_name,server_name, REQUESTED_AUDIOS, 1);

    }  
}

// Sent the requested file to a client
void download_audio_server(char* path, int client_fd, char* data)
{
    // Creates the complete path of the file
    char final [strlen(path) + strlen(data) + 2];
    strcpy(final,path);
    strcat(final, "/");
    strcat(final,data);
    
    // Opens the file
    int fd = open(final, O_RDONLY);
	if (fd < 0) 
	{
        frameGenerator(TYPE_DOWNLOAD_AUDIO, HEADERS_AUDIO_KO, 0, NULL, client_fd);
	}
    else
    {
        // Sends the file in chunks of 500 bytes
        int size = 500;
        char buff[size];
        int nBytes = 0;

        nBytes = read(fd,&buff,size);
        while (nBytes > 0 )
        {
            frameGenerator(TYPE_DOWNLOAD_AUDIO, HEADERS_AUDIO_RESPONSE, nBytes, buff, client_fd);
            nBytes = read(fd,&buff,size);
        }
        close(fd);

        // Executes de md5 and sends it to the client
        char * md5  = execute_command("/usr/bin/md5sum", "md5sum", final);
        frameGenerator(TYPE_DOWNLOAD_AUDIO, HEADERS_EOF, strlen(md5) + 1, md5, client_fd);
        freeString(&md5);

        // Reads the result of the client
        char type;
        char* header = NULL;
        char* data = NULL;
        frameReader(&type,&header,&data,client_fd);

        if(strcmp(header,HEADERS_MD5_OK) == 0)
        {
            inform(client_name,server_name, DOWNLOADED_AUDIOS_SUCCESSFULY, 1);

        }
        else
        {
            inform(client_name,server_name, DOWNLOADED_AUDIOS_UNSUCCESSFULY, 1);
        }

        freeString(&header);
        freeString(&data);

        
    }
}

void free_server()
{
    freeString(&header);
    freeString(&data);
    //freeString(client_name);
}

// Elimintates the client from the list of server connections and closes the socket
void exit_server_from_client(char* client_name) //IDEA: In abstract implementation, we could call free methods that come with the structures used
{
    Connection* c = NULL;
    if(isElement(&server,client_name)) // Double purpose: 1. Look for the element. 2. If element is found place the POI
    {
        c = (Connection*)POIList_consult (&server);
        frameGenerator(TYPE_EXIT, HEADERS_CONNECTION_OK, 0, NULL, c->conn_fd);
        freeString(&(c->ID)); //Once we have sent the frame we can free it from the Structure before removing the element from the structure
        close(c->conn_fd);
        POIList_delete(&server);

        inform(client_name,server_name, DISCONNECTED, 1);
    }
    else
    {
        // Not found!
        frameGenerator(TYPE_EXIT, HEADERS_CONNECTION_KO, 0, NULL, c->conn_fd); 
    }
    free_server();
}


