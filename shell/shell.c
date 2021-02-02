
#include "shell.h"
#include "network/server/server.h"

Configuration* conf;

// Manages the the insertions of arguments by the user
void listenCommand(Configuration configuration)
{	
    conf = &configuration;
    signal (SIGINT, exit_handleler);
    signal(SIGPIPE, SIG_IGN); // For the closed sockets, I take care later on

    char aux[strlen(configuration.user) + 4];
    client = POIList_create();

    while(1)
    {
        sprintf(aux, "\n%c%s%s",'$',configuration.user,": ");
        writeToScreen(aux);
        
        char** args = NULL;
        int argc = readArguments(&args);
        int exit_int = manageArguments(args,argc,configuration,&client);
        freeMatrix(argc,&args); // I need to free args itself

        if (exit_int == 1)
        {
            exitTrinity();
            break;
        }
    }
}

// Processes each argument, places it into a matrix and returns the number of arguments
int readArguments(char*** args)
{
    char aux;
    int nBytes = read(0,&aux,1); // Initial read to check that the user is not trolling me
    int iArg = 1,iArgs = 0; //iArg initially 1 because we have already read the first character to test
    if(nBytes == -1 || nBytes == 0 || aux == '\n') //TODO: Alomejor se puede simplificar y no hace falta comprobar tanto. Comprobar rango ASCII
    {
        //TODO: Error;
    }
    else
    {
        *args = malloc(sizeof(char));
        char* arg = malloc (sizeof(char)); 
        arg[0] = aux;

        while(aux != '\n')
        {
            int is_literal = 0; // Flag for determining if we are inside a " ", so spaces don't count as arguments
            read(0,&aux,1);
            while((aux != ' ' && aux != '\n') || is_literal == 1) // We read each argument separated by an space
            {
                if (aux == '"')
                {
                    if (is_literal == 0) is_literal = 1;
                    else is_literal = 0;
                }
                arg = realloc(arg, sizeof(char) * iArg + 2); //2 because of the \0
                arg[iArg] = aux;
                iArg++;
                read(0,&aux,1);
            } 
            arg[iArg] = '\0';

            *args = realloc(*args, sizeof(char*) * (iArgs + 1)); // We make room for the char* that will point to the beginning of the string of the argument
            (*args)[iArgs] = malloc(strlen(arg) + 1); // We make room for the string containing the argument
            strcpy((*args)[iArgs],arg); // We copy the string  containing the argument

            iArgs++;
            iArg = 0;
            //freeString(arg);
        }
        freeString(&arg);     
    }
    return iArgs;
}

// Analyses the commands matrix and executes the correspondant function
int manageArguments(char** args, int argc, Configuration configuration, POIList *p)
{
    int exit = 0;
    
    if (argc >= 2 && argc <= 3) 
    {
        if(strcasecmp(args[0],"SHOW") == 0)
        {
            if(strcasecmp(args[1],"CONNECTIONS") == 0)
            {   
                if (argc == 2)
                {
                    showConnections(configuration);
                }
                else
                {
                    error(1);
                }
            }
            else if(strcasecmp(args[1],"AUDIOS") == 0)
            {
                if (argc == 3)
                {
                    showAudios(args[2]);
                }
                else
                {
                    error(1);
                }
            }
            else
            {
                error(0);
            }
        }
        else if(strcasecmp(args[0],"CONNECT") == 0)
        {
            if (argc == 2)
            {   
                //Could be a function check numeric value or something
                int ok = 1, i = 0;
                char aux = args[1][i];
                while(aux != '\0')
                {
                    if(aux < '0' || aux > '9')
                    {
                        ok = 0;
                    }
                    i++;
                    aux = args[1][i];   
                }

                if(i > 4 || !ok) 
                {
                    error(2);
                }
                else if(strcmp(args[1],configuration.port) == 0) 
                {
                    error(3);
                }
                else
                {
                    connect_cypher(args[1],&(configuration.user),p);
                }
            }
            else
            {
                error(1);
            }
        }
        else if(strcasecmp(args[0],"SAY") == 0)
        {
            if (argc == 3)
            {
                say(args[1], &(args[2]), p);
            }
            else
            {
                error(1);
            }
        }
        else if(strcasecmp(args[0],"BROADCAST") == 0)
        {
            if (argc == 2)
            {
                broadcast(&(args[1]));
            }
            else
            {
                error(1);
            }
        }
        else if(strcasecmp(args[0],"DOWNLOAD") == 0)
        {
            if (argc == 3)
            {
                download(args[1], args[2], configuration.folder);
            }
            else
            {
                error(1);
            }
        }
        else 
        {
            error(0);
        }
    }
    else
    {
        if(strcasecmp(args[0],"EXIT") == 0)
        {
            exit = 1;
        }
        else
        {
         error(1);
        }
    }
    return exit;
}

void exit_handleler()
{
    write(1," \n",strlen(" \n"));
    exitTrinity();
}

// Disconnects the server and the client and cleans the memory
void exitTrinity()
{
    //Information
    write(1,EXIT, strlen(EXIT));

    //Client disconnects from servers
    disconnect_from_servers();

    //Server Disconnects 
    close_server(); // Closing the connection petition listener

    // We make all the threads finish
    out = 1; 
    POIList_destroy(&client);
    POIList_head(&server);
    while(!POIList_tail(&server))
    {
        Connection* c = (Connection*)POIList_consult (&server);
        freeString(&(c->ID)); //Once we have sent the frame we can free it from the Structure before removing the element from the structure
        close(c->conn_fd);
        POIList_forward(&server);
    }

    POIList_destroy(&server);
    freeConfiguration(*conf);

    exit(EXIT_SUCCESS);
}

// Manages several kinds of errors
void error(int type)
{
    switch (type)
    {
        case 0 : write(1,COMMAND_ERROR_SHELL, strlen(COMMAND_ERROR_SHELL));
        break;

        case 1: write(1,ARGS_ERROR_SHELL, strlen(ARGS_ERROR_SHELL));
        break;

        case 2: write(1,CONNECT_ERROR_PORT,strlen(CONNECT_ERROR_PORT));
        break;

        case 3: write(1,CONNECT_ERROR,strlen(CONNECT_ERROR));
        break;
    }
}







