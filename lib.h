
#ifndef _LIB_H
#define _LIB_H

#include <unistd.h> //write, execl
#include <fcntl.h> //open	
#include <stdlib.h> //malloc
#include <string.h> //strlen
#include <strings.h> //strcasecmp
#include <stdio.h>  //printf (just for testing purpose, don't fail us)
#include <stdint.h> //uint_16
#include <signal.h> //signals
#include <sys/types.h>
#include <pthread.h> // Threads
#include <sys/socket.h> // Network Stuff
#include <netinet/in.h> // Network Stuff
#include <arpa/inet.h> // Network Stuf
#include <netinet/in.h> // Network Stuf
#include <errno.h> //Client Network Stuff
#include <sys/wait.h> // waitpid()
#include <dirent.h> //file directories

pthread_mutex_t screen;


#endif
