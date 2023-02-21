/*
 * dsh.c
 *
 *  Created on: Aug 2, 2013
 *      Author: chiu
 */
#include "dsh.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <errno.h>
#include <err.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

//splits the string into tokens based on the delim, and records the number of tokens to the return pointer tokens
char **split(char *str, char *delim, int *tokens)
{
    int numTokens = 1;

    for (int i = 0; i < strlen(str); i++)
    {

        //check each section of the string for the delim, depending on the length of the delim.
        int delimFound = 1;
        for (int j = 0; j < strlen(delim); j++)
        {

            if (delim[j] != str[i + j])
            {

                //if they are not equal, set the found to zero
                delimFound = 0;
            }

        }

        if (delimFound == 1)
        {

            numTokens++;
        }

    }

    //allocate memory on the heap for the tokens
    char **tokenStringArray = (char **)malloc((numTokens + 1) * sizeof(char *));

    for (int i = 0; i < numTokens; i++)
    {

        tokenStringArray[i] = (char *)malloc(MAXBUF * sizeof(char));
    }

    //tokenize each token, and add to the array
    char *token;
    token = strtok(str, delim);
    int index = 0;

    while (token != NULL)
    {
        strcpy(tokenStringArray[index], token);
        token = strtok(NULL, delim);
        index++;
    }

    *tokens = numTokens;
    return tokenStringArray;
}


void FullPathGiven(char **command, int *commandTokensLength)
{
    char *path = command[0];
    char lastChar = command[*commandTokensLength - 1][strlen(command[*commandTokensLength - 1]) - 1];
      if (access(path, F_OK | X_OK) == 0)
    {

  handleExecution(path, command,commandTokensLength,lastChar);
    }else
    {

        printf("%s is not a valid path, please try again\n", path);
    }
    
}

void FullPathConstruction(char **command, int *commandTokensLength)
{

    char path[BUFSIZ];
    getcwd(path, sizeof(path));
    char slash[2];
    strcpy(slash, "/");
    strcat(path, slash);
    strcat(path, command[0]);
    char lastChar = command[*commandTokensLength - 1][strlen(command[*commandTokensLength - 1]) - 1];
//if we find the path in our home directory, we are fine, and can just run it like normally.
    if (access(path, F_OK | X_OK) == 0)
    {
        
        handleExecution(path, command,commandTokensLength,lastChar);
    }

//otherwise, we get the path env variable, and see if we can find the file in any of them.
    else
    {

        char envPath[MAXBUF];
        strcpy(envPath,getenv("PATH"));
        int *envPathTokensLength = (int *)malloc(sizeof(int));
        char **envPathTokens = split(envPath, ":", envPathTokensLength);
        for (int i = 0; i < *envPathTokensLength; i++)
        {

            char currentPath[BUFSIZ];
            strcpy(currentPath, envPathTokens[i]);
            strcat(currentPath, slash);
            strcat(currentPath, command[0]);
            //if we find it,run the command, and then free all the memory.
            if (access(currentPath, F_OK | X_OK) == 0)
            {

                handleExecution(currentPath, command,commandTokensLength,lastChar);
            
                for (int j = 0; j < *envPathTokensLength; j++)
                {

                    free(envPathTokens[j]);
                    envPathTokens[j] = NULL;
                }

                free(envPathTokens);
                envPathTokens = NULL;
                free(envPathTokensLength);
                envPathTokensLength = NULL;
                return;
            }

        }
        //otherwise, we couldn't find it here, therefore we need to free everything here.

        printf("ERROR:%s not found \n", command[0]);
        for (int j = 0; j < *envPathTokensLength; j++)
        {

            free(envPathTokens[j]);
            envPathTokens[j] = NULL;
        }

        free(envPathTokens);
        envPathTokens = NULL;
        free(envPathTokensLength);
        envPathTokensLength = NULL;
        return;
    }

}
// runs the file, and waits for the result from that execution, before it lets the user input a new command
void RunInForeground(char *command, char **argv)
{
    pid_t child_pid = fork();
    if (0 != child_pid)
    {
        wait(NULL);
    }
    else
    {
        execv(command, argv);
    }
}
// runs the file, and does NOT wait for the file to be finished running before letting the user put in new commands
void RunInBackground(char *command, char **argv)
{
    if (0 == fork())
    {
        execv(command, argv);
    }
}
// prints our current working directory
void pwd()
{
    char cwd[BUFSIZ];
    // found at https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-getcwd-get-path-name-working-directory for using getcwd().

    if (getcwd(cwd, sizeof(cwd)) == NULL)
        perror("getcwd() error");
    else
        printf("Current working directory is: %s\n", cwd);
}

// puts us back to the home directory
void cdHome()
{
    chdir(getenv("HOME"));
}
// puts us into this path.
void cd(char *path)
{
    chdir(path);
}
//handles the execution, calliong either the background or foreground, and getting rid of any unnecessary elements (namely the & for indicating background)
void handleExecution(char* path, char**command,int*commandTokensLength,char lastChar){  
        command[*commandTokensLength] = NULL;
        if (lastChar == '&')
        {
            if (strlen(command[*commandTokensLength - 1]) == 1)
            {
                free(command[*commandTokensLength - 1]);
                command[*commandTokensLength - 1] = NULL;
            } 
            RunInBackground(path, command);
        }
        else
        {
            RunInForeground(path, command);
        }
    }

