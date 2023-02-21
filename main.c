/*
 * main.c
 *
 *  Created on: Mar 17 2017
 *      Author: david
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "dsh.h"

int main(int argc, char **argv)
{
	//	the flag for us to keep getting commands
	int getCommand = 1;
	while (getCommand)
	{
		
		char *command = (char *)malloc(MAXBUF);
		printf("dsh>");
		fgets(command, MAXBUF, stdin);
		int ln = strlen(command);
		//get rid of the \n at the end of each input
		if ((ln > 0) && (command[ln - 1] == '\n'))
		{
			command[ln - 1] = '\0';
		}
		//if the string is exit, we turn the flag off, free command, and then end the loop early.
		if (strcmp(command, "exit") == 0)
		{
			getCommand = 0;
			free(command);
			continue;
		}
		//if it wasn't we then tokenize the command on spaces.
		int *commandTokensLength = (int *)malloc(sizeof(int));
		char **commandTokens = split(command, " ", commandTokensLength);

		if (command[0] == '/')
		{
			FullPathGiven(commandTokens, commandTokensLength);
		}

		else if (strcmp(commandTokens[0], "pwd") == 0)
		{
			pwd();
		}

		else if (strcmp(commandTokens[0], "cd") == 0)
		{
			//if there is a second argument, we cd to that directory
			if (commandTokens[1] != NULL)
			{
				cd(commandTokens[1]);
			}
			//otherwise we just go to the home directory
			else
			{
				cdHome();
			}
		} else{
			//otherwise, it was not a full path or one of our predetermined commands, so we need to see if that file exists elsewhere
			FullPathConstruction(commandTokens, commandTokensLength);
		}
			//after we are done, we free everything we allocated when tokenizing the command
		for (int i = 0; i < *commandTokensLength; i++)
		{
			free(commandTokens[i]);
			commandTokens[i] = NULL;
		}
		free(commandTokens);
		commandTokens = NULL;
		free(command);
		command = NULL;
		free(commandTokensLength);
		commandTokensLength = NULL;
	}
	return 0;
}
