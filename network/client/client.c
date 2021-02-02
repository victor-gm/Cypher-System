#include "client.h"

char* user_global = NULL;

//Shows available servers to connect
void showConnections(Configuration configuration) //TODO think about passing it as a pointer
{
    writeToScreen(SHOW_CONNECTIONS);

    //Create pipe
    int fd[2];
    if (pipe(fd) == -1)
    {
        write(1,ERROR_PIPE, strlen(ERROR_PIPE));
        exit(-1);
    }

    //Create fork
    pid_t new_pid = fork();
    switch(new_pid)
    {
        case -1:
            writeToScreen(ERROR_FORK);
            break;
        case 0:  // (Son/Daughter)
            close(fd[0]);
            dup2(fd[1],1); // Redirecting otuput file descriptor to the pipe
            execl("files_directory/show_connections_v2.sh","show_connections_v2.sh",configuration.port_start,configuration.port_end, WORKING_IP,NULL); // Executing command
            close(fd[1]);
            break;
        default: // (Dad/Mum)
            waitpid(new_pid,NULL,0);
            close(fd[1]);
            showPorts(fd[0],atoi(configuration.port)); // Process obtained string by the command
            close(fd[0]);
            break;
    }
}

// Connect a server
void connect_cypher (char* arg, char** user, POIList *p)
{
    if (user_global == NULL) user_global = *user;

    if (isElement_port(&client,atoi(arg)) == NULL)
    {
        //write(1,CONNECT,strlen(CONNECT));
        writeToScreen(CONNECT);
        int port = atoi(arg);
        int socket_fd = connect_to_server(WORKING_IP, port);
        if(socket_fd != -1)
        {
            // Generate request
            frameGenerator(TYPE_CONNECTION, HEADERS_CONNECTION_REQUEST, (unsigned short) strlen(user_global) + 1, user_global,socket_fd);

            // Read request
            char type;
            char* header = NULL;
            char* data = NULL;
            frameReader(&type,&header,&data,socket_fd);

            // Create Connection data and insert
            Connection c;

            int ID_client_lenght = strlen(data)+1;
            c.ID = malloc(ID_client_lenght);
            memcpy(c.ID,data,ID_client_lenght);

            c.conn_fd = socket_fd;
            c.port = port;

            if(!POIList_insert(p,&c,sizeof(Connection)))
            {
                writeToScreen(ERROR_CONNECT);
            }
            else
            {    
                char buff[4 + strlen("connected: ") + strlen(data) + 1];
                sprintf(buff,"%d connected: %s\n",port,data);
                writeToScreen(buff);
            }
            
            freeString(&data); 
            freeString(&header);
        }
        else
        {
            writeToScreen(ERROR_CONNECT);
        }    
    }
    else
    {
        writeToScreen(ERROR_CONNECT_ALREADY_CONNECTED);
    }   
}

// Send a message to a server
void say (char* arg1, char** arg2, POIList *p)
{
    if(isElement(&client,arg1))
    {
        if (checkString(arg2)) 
        {   
            int server_fd = POIList_obtain_fd(p, arg1);
            frameGenerator(TYPE_SAY, HEADERS_SAY_REQUEST, strlen(*arg2) + 1, *arg2, server_fd);

            char type;
            char* header = NULL;
            char* data = NULL;
            check_ack(server_fd, &type, &header, &data);
            freeString(&header);
            freeString(&data);
        }
        else
        {
            writeToScreen(ERROR_STRING);
        }
    }
    else
    {
        writeToScreen(ERROR_USER_NOT_FOUND);
    }   
}

// Broadcast request. Same as say but sends to everyone present in the server list
void broadcast (char** arg1)
{
    if (checkString(arg1))
    {
        POIList_head(&client);
        while(!POIList_tail(&client))
        {
            Connection * c = (Connection *) POIList_consult(&client);
            int server_fd = c->conn_fd;
            frameGenerator(TYPE_SAY_BROADCAST, HEADERS_BROADCAST_REQUEST, strlen(*arg1) + 1, *arg1, server_fd);

            char type;
            char* header = NULL;
            char* data = NULL;
            check_ack(server_fd, &type, &header, &data);

            if(strcmp(header,HEADERS_SAY_OK) == 0) 
            {
                char* client_name = c->ID;
                char msg[] = "Cool!\n";
                char* buff = malloc(strlen(client_name) + strlen(msg) + 5);
                sprintf(buff,"\n[%s]:%s",client_name,msg);
                writeToScreen(buff);
                freeString(&buff);
            }

            freeString(&header);
            freeString(&data);

            if(!POIList_tail(&client))POIList_forward(&client); //We have this if in case we delete a server and it's the onyly one
        }
    }
    else
    {
        writeToScreen(ERROR_STRING);
    }
}

// Show audio list request to the server
void showAudios(char* arg1)
{
    if(isElement(&client,arg1))
    {

        int server_fd = POIList_obtain_fd(&client, arg1);
        frameGenerator(TYPE_SHOW_AUDIOS, HEADERS_SHOW_AUDIOS_REQUEST, 0, NULL, server_fd);

        char type;
        char* header = NULL;
        char* data = NULL;
        if(check_ack(server_fd, &type, &header, &data))
        {
            Connection* c = POIList_consult(&client);
            char aux[50];
            sprintf(aux, "%s%s%s%s\n","[",c->ID,"] ", AUDIOS);
            writeToScreen(aux);

            writeToScreen(data);
        }
        freeString(&header);
        freeString(&data);
    }
    else
    {
        writeToScreen(ERROR_USER_NOT_FOUND);
    }
}

void download(char* arg1, char* arg2, char* path)
{
    if(isElement(&client,arg1))
    {
        // Generates the path where the file is going to be saved
        char final [strlen(path) + strlen(arg2) + 2];
        strcpy(final,path);
        strcat(final, "/");
        strcat(final,arg2);

        int fd = open(final, O_WRONLY|O_CREAT|O_TRUNC, 0600);
	    if (fd < 0) 
	    {
            writeToScreen(FILE_NOT_CREATED);
	    }
        else
        {
            // Generate request
            int server_fd = POIList_obtain_fd(&client, arg1);
            frameGenerator(TYPE_DOWNLOAD_AUDIO, HEADERS_AUDIO_REQUEST, strlen(arg2) + 1, arg2, server_fd);

            // Read server confirmation
            char type;
            char* header = NULL;
            char* data = NULL;
            int lenght = check_ack(server_fd, &type, &header, &data);

            if(lenght != -1)
            {
                if(strcmp(header,HEADERS_AUDIO_KO) == 0)
                {
                    writeToScreen(ERROR_AUDIO_NOT_FOUND);
                }
                else
                {
                    // Downloads the file
                    while(strcmp(header,HEADERS_AUDIO_RESPONSE) == 0)
                    {
                        write(fd,data,lenght);
                        freeString(&header);
                        freeString(&data);   

                        lenght = frameReader(&type,&header,&data, server_fd);
                    }
                    close(fd);

                    // Executes de md5 and sends the results back to the server
                    char * md5 = NULL;
                    md5 = execute_command("/usr/bin/md5sum", "md5sum", final);
                    if(strcmp(md5,data) == 0) 
                    {
                        frameGenerator(TYPE_DOWNLOAD_AUDIO, HEADERS_MD5_OK, 0, NULL, server_fd);
                        inform(arg1,arg2,DOWNLOADED,0);
                    }
                    else
                    {
                        frameGenerator(TYPE_DOWNLOAD_AUDIO, HEADERS_MD5_KO, 0, NULL, server_fd);
                    }
                    freeString(&md5);
 
                }
            }
            freeString(&header);
            freeString(&data);   
        }
    }
    else
    {
        writeToScreen(ERROR_USER_NOT_FOUND);
    }

}

// Disconnects from all the servers
void disconnect_from_servers()
{
    POIList_head(&client);
    Connection *c = NULL;
    char type;
    char* header = NULL;
    char* data = NULL;

    if(!POIList_empty(&client))
	{
		while(!POIList_tail (&client))
		{
	    	c = (Connection*)POIList_consult(&client);
            frameGenerator(TYPE_EXIT, HEADERS_EXIT, (unsigned short) strlen(user_global) + 1, user_global,c->conn_fd);
            
            header = NULL;
            data = NULL;
            check_ack(c->conn_fd, &type, &header, &data);
            
            if(strcmp(header,HEADERS_CONNECTION_OK) == 0)
            {
                freeString(&(c->ID));// We free the Connection from the node as we do on the server
                close(c->conn_fd);
                //printf("Successful Disconnection\n");
            }
            else
            {
                //printf("Unsuccseful Disconnection");
            }

            freeString(&header);
            freeString(&data);

	    	if(!POIList_empty(&client))POIList_forward(&client);
		}
        POIList_destroy(&client);   
	}
}



