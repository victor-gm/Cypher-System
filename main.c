
#include "lib.h"
#include "shell/shell.h"
#include "files/files.h"
#include "network/server/server.h"


#define START 				"Starting Trinity...\n"

#define ARGS_ERROR 			"Error with the number of arguments. Must be one.\n"
#define FILE_ERR        	"Error reading data file.\n"
#define COMMAND_ERROR 		"Error, command not found."
#define ERR_FORK    		"Error in fork creation.\n"


int main (int argc,char* argv[])
{
	// Check if the program has the required number of arguments
	if(argc != 2)
	{
		writeToScreen(ARGS_ERROR);
		return EXIT_FAILURE;
	}

	write(1, START, strlen(START));

	// Read the configuration file
	Configuration configuration;
	if(readConfigurationFile(argv[1],&configuration) < 0 )
	{
		writeToScreen(FILE_ERR);
		return EXIT_FAILURE;
	}

	// Start Server
	char* ip = "127.0.0.1";
	if(server_run(ip,atoi(configuration.port),configuration.user, configuration.folder))listenCommand(configuration);
	
	freeConfiguration(configuration); //If the port is not binded, we free the config from here
	return 0;
}
