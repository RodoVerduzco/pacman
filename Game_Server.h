/*
*
*   Program :   Game_Server.h
*
*   Authors  :  Cynthia
*               A0102
*
*               Ludovic
*               A0
*
*               Jose Rodolfo Verduzco Torres
*               A01366134
*
*   Purpose :
*              This program has the helper functions to perform the
*              dealer/server operations.
*
*   Usage  :
*              The program Receives all its info from the blackjack_server.c
*              program, and returns the data to it
*
*
*
*/

#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 255

/* Data processing*/
char* processData(char data[], pid_t pid);

#endif  /* NOT GAME_SERVER_H */
