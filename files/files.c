#include "files.h"

// Reads the configuration file
int readConfigurationFile(char* path, Configuration* configuration)
{
	int fd = open(path, O_RDONLY);
	if (fd < 0) 
	{
		writeToScreen(FILE_NOT_FOUND_ERR);
	    return -1;
	}

	configuration->user = readUntil(fd, '\n');
	configuration->folder = readUntil(fd, '\n');
	configuration->IP = readUntil(fd, '\n');
	configuration->port = readUntil(fd, '\n');
	configuration->web = readUntil(fd, '\n');
	configuration->port_start = readUntil(fd, '\n');
	configuration->port_end = readUntil(fd, '\n');

	close(fd);

	return 0;
}

// Frees the structure of configuration
void freeConfiguration(Configuration configuration)
{
	freeString(&(configuration.user));
	freeString(&(configuration.folder));
	freeString(&(configuration.IP));
	freeString(&(configuration.port));
	freeString(&(configuration.web));
	freeString(&(configuration.port_start));
	freeString(&(configuration.port_end));
}