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
#include <unistd.h>;

// TODO: Your function definitions (declarations in dsh.h)
char **split(char *str, char *delim, int *tokens)
{
    int numTokens = 1;
    for (int i = 0; i < strlen(str); i++)
    {
        int delimFound = 1;
        for (int j = 0; j < strlen(delim); j++)
        {
            if (delim[j] != str[i + j])
            {
                delimFound = 0;
            }
        }
        if (delimFound == 1)
        {
            numTokens++;
        }
    }
    char **tokenStringArray = (char **)malloc((numTokens + 1) * sizeof(char *));
    for (int i = 0; i < numTokens; i++)
    {
        tokenStringArray[i] = (char *)malloc(MAXBUF * sizeof(char));
    }
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
void FullPathGiven(char **command, int *commandTokens)
{
    char *path = command[0];
    char lastChar = command[*commandTokens - 1][strlen(command[*commandTokens - 1]) - 1];
    printf("%c", lastChar);
    if (access(path, F_OK | X_OK) == 0)
    {
        int argc = *commandTokens - 1;
        if (lastChar == '&')
        {
            if (strlen(command[*commandTokens - 1]) == 1)
            {
                argc--;
            }
            // we are inputting the all of the command tokens, including the initial path, so we will be ignoring the 0th token in the future, as it is not an argument for the file.
            RunInBackground(path, command, argc);
        }
        else
        {
            // we are inputting the all of the command tokens, including the initial path, so we will be ignoring the 0th token in the future, as it is not an argument for the file.
            RunInForeground(path, command, argc);
        }
    }
    else
    {
        printf("%s is not a valid path, please try again\n", path);
    }
}
void FullPathConstruction(char **command)
{
}
void RunInForeground(char *command, char **argv, int argc)
{
    pid_t child_pid = fork();
    if (0 != child_pid)
    {
        wait(NULL);
    }
    if (0 == fork())
    {
    }
}
void RunInBackground(char *command, char **argv, int argc)
{
}
void pwd()
{
    char cwd[256];
    // found at https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-getcwd-get-path-name-working-directory for using getcwd().
    if (chdir("/tmp") != 0)
        perror("chdir() error()");
    else
    {
        if (getcwd(cwd, sizeof(cwd)) == NULL)
            perror("getcwd() error");
        else
            printf("Current working directory is: %s\n", cwd);
    }
}

void cdHome()
{
    chdir(getenv("HOME"));
}
cd(char*path){
    chdir(path);
}