
#include "string_utils.h"

// Reads a file descriptor until a given character is met
char* readUntil(int fd, char end) 
{
    int i = 0;
    //char c = '\0';
    char c = '\a';
    char* string = (char*)malloc(sizeof(char));

    while (c != end) 
    {
        //printf("%c",c);
        read(fd, &c, sizeof(char));
        if (c != end) 
        {
            string = (char*)realloc(string, sizeof(char) * (i + 2));
            string[i] = c;
        }
        i++;
    }
    string[i - 1] = '\0';
    return string;
}

// Checks a message string and if it's ok eliminates the " " surrounding it
int checkString(char** string) 
{   
    int size = strlen(*string);
    if((*string)[0] == '"' && (*string)[size - 1] == '"')
    {        
        for(int i = 0; i < size - 2; i++)
        {
            (*string)[i] = (*string)[i+1];
        }
        (*string)[size - 2] = '\0';
        *string = realloc(*string, size - 1);
        return 1;
    }
    return 0;
  
}

void freeString(char** s) 
{
    //if(s == NULL) printf("Null!\n");
    if(s != NULL)
    {
        free(*s);
        *s = NULL;
    }
}




