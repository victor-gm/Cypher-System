#include "network.h"

// Sends a frame of the cypher protocol
int frameGenerator(char type, char* header, unsigned short lenght, char* data, int conn_fd) 
{
    write(conn_fd,&type,sizeof(type));
    write(conn_fd,header,strlen(header)+1); //+1 because I want to send '\0'
    write(conn_fd,&lenght,sizeof(lenght));
    write(conn_fd,data,lenght);

    return 1; 
}

// Reads frame of the cypher protocol
int frameReader(char* type, char** header, char** data, int conn_fd)
{
    unsigned short lenght = 0;

    int nBytes = read(conn_fd,type,sizeof(char));
    if (nBytes == 0) return -1; // In case of show_connections type of thing and closed sockets

    *header = readUntil(conn_fd, '\0'); // Could have worked out some stuff with the [], anyway  

    read(conn_fd,&lenght,sizeof(lenght)); 

    
    *data = malloc(lenght);

    nBytes = 0;
    while(nBytes < lenght)
    {
        nBytes += read(conn_fd,*data + nBytes,lenght - nBytes);
        //printf("Read: %s\n",data);
    }

    return lenght;
}

// Searches for an element, and if it finds it marks it as POI as well as returns the correct value
int isElement(POIList *p, char* user) 
{
	POIList_head (p);
	Connection* c;
	if(!POIList_empty(p))
	{
		while(!POIList_tail (p))
		{
	    	c = (Connection*)POIList_consult(p);

	    	if(strcasecmp(c->ID,user) == 0) return 1;
	    	POIList_forward(p);
		}
	}

	return 0;
}

// The same as isElement but using the port as a search value
char* isElement_port(POIList *p, int port) 
{
	POIList_head (p);
	Connection* c;

	if(!POIList_empty(p))
	{
		while(!POIList_tail (p))
		{
	    	c = (Connection*)POIList_consult(p);
	    	if(c->port == port) return c->ID;
	    	POIList_forward(p);
		}
	}

	return NULL;
}

// An implementation of isElement returning a file descriptor
int POIList_obtain_fd(POIList *p, char* user)
{  
    if(isElement(p,user))
    {
        Connection* c = (Connection*)POIList_consult (p);
        return c->conn_fd;
    }
    return -1;
}

void freeSocket(Socket** s)
{
    if(*s!= NULL)
    {
        free(*s);
        *s = NULL;
    }
}