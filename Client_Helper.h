/*
*
*   Program :   client_helper.h
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
*              This program has the handler functions of the client_helper.c
*
*/


#ifndef CLIENT_HELPER_H
#define CLIENT_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Sockets libraries */
#include <netdb.h>
#include <arpa/inet.h>

#include "fatal_error.h"


#define BUFFER_SIZE 1024
#define CARD_NUM    20

enum cardVal { J=11, Q=12, K=13, A=1, BLACKJACK=21 };
enum header  { BET, PLAY, SUM, PLAYER, DEALER, GAMEOVER, BUSTED };

/* Game Handlers */

/* Communication between client/server */
int sendData(int connection_fd, char bf[]);
int processReceivedData(char data[]);

#endif  /* CLIENT_HELPER */
