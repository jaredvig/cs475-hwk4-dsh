/*
 * dsh.h
 *
 *  Created on: Aug 2, 2013
 *      Author: chiu
 */

#define MAXBUF 256

// TODO: Your function prototypes below
char** split(char *str,char *delim,int* tokens);
void FullPathGiven(char** command, int* commandTokensLength);
void FullPathConstruction(char **command, int *commandTokensLength);
void RunInForeground(char* command, char**argv);
void RunInBackground(char* command, char**argv);
void pwd();
void cdHome();
void cd(char* path);
void handleExecution(char* path, char**command,int*commandTokensLength,char lastChar);