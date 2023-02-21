/*
 * dsh.h
 *
 *  Created on: Aug 2, 2013
 *      Author: chiu
 */

#define MAXBUF 256

// TODO: Your function prototypes below
char** split(char *str,char *delim,int* tokens);
void FullPathGiven(char** command, int* commandTokens);
void RunInForeground(char* command, char**argv, int argc);
void RunInBackground(char* command, char**argv, int argc);
void pwd();
void cdHome();
void cd(char* path);