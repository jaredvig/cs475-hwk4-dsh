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
//	chdir("$HOME");
	int getCommand = 1;
	while(getCommand){
	char* command = (char*)malloc(MAXBUF);
	printf("dsh>");
	fgets(command,MAXBUF,stdin);
	int ln = strlen (command);
if ((ln > 0) && (command[ln-1] == '\n')){
    command[ln-1] = '\0';
}
	if(strcmp(command,"exit")==0){
		getCommand=0;
		continue;
	}
	int* tokens =(int*)malloc(sizeof(int));
	char** commandTokens = split(command, " ",tokens);
	
	if(command[0]=='/'){
		FullPathGiven(commandTokens,tokens);

	} else if(strcmp(commandTokens[0], "pwd")==0){
		pwd();
	} else if(strcmp(commandTokens[0], "cd")==0){
		if(commandTokens[1]!= NULL){
			cd(commandTokens[1]);
		} else{
			cdHome();
		}
	}

	//char** commandTokens = split(command, " ");
	//char cmdline[MAXBUF]; // stores user input from commmand line
	}
	return 0;
}
