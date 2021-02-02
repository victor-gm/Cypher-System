#include "general_utils.h"

// Frees pointers of a given matrix (2x2)
void freeMatrix(int argc, char*** args)
{
    for(int i = 0; i < argc; i++)
    {
        free((*args)[i]);
        (*args)[i] = NULL;
    }
    free(*args);
    *args = NULL;
}

// Execute command in a child process and get the output
char* execute_command(char* path, char* command, char* arg1)
{
    int fd[2];
    if (pipe(fd) == -1)
    {
        //writeToScreen(ERROR_PIPE);
        exit(-1);
    }
    pid_t new_pid = fork();
    switch(new_pid)
    {
        case -1:
        //writeToScreen(ERROR_FORK);
            break;
        case 0:  // (Son/Daughter)
            close(fd[0]);
            dup2(fd[1],1); // Redirecting otuput file descriptor to the pipe
            execl(path,command,arg1,NULL);
            close(fd[1]);
            break;
        default: // (Dad/Mum)
            waitpid(new_pid,NULL,0);
            close(fd[1]);
            char aux;
            char* final = malloc(1);
            int nBytes = read(fd[0],&aux,1);
            while(aux != ' ')
            {
                final[nBytes - 1] = aux;
                nBytes += read(fd[0],&aux,1);  
                final = realloc(final, nBytes);
            }
            final[nBytes - 1] = '\0';

            close(fd[0]);
            return final; 
            break;
    }
    return NULL;
}      

// Write a given string into a screen with a mutex
void writeToScreen(char* text)
{
    pthread_mutex_lock(&screen);
        write(1,text, strlen(text));
    pthread_mutex_unlock(&screen);
}

// Informs the user of an event
void inform(char* arg1, char* arg2, char* action, int is_server)
{
    if(!is_server)
    {
        char aux[strlen(arg1) + 3 + strlen(action) + strlen(arg2) + 1];
        sprintf(aux,"\n%c%s%c%s%c%s",'[',arg1,']',arg2,' ', action);
        writeToScreen(aux); 
    }
    else
    {
        char aux[strlen(arg1) + 3 + strlen(action)];
        sprintf(aux,"\n%c%s%c%s",'[',arg1,']',action);
        writeToScreen(aux); 
    } 

    if(is_server)printPrompt(arg2);
}

// Prints user prompt with name 
void printPrompt(char* name_machine)
{
    char aux[strlen(name_machine) + 1 + 2 + 1];
    sprintf(aux, "\n%c%s%s",'$',name_machine,": ");
    writeToScreen(aux); 
}
