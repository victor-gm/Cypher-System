#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

typedef struct configuration
{
	char* user;
	char* folder;
	char* IP;
	char* port;
	char* web;
	char* port_start;
	char* port_end;
} Configuration;

#endif