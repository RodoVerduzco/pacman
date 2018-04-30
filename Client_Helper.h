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

#include "error.h"
#include "Client_Com.h"


#define BUFFER_SIZE 1024

/* Communication between client/server */
int sendData(int connection_fd, char bf[]);
int processReceivedData(char data[]);

#endif  /* CLIENT_HELPER */
