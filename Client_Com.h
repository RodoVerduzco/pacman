/*
*
*   Program :   Client_Com.h
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
*              This program handles the clients communication with
*              the server.
*
*/

#ifndef CLIENT_COM_H
#define CLIENT_COM_H

/* Sockets libraries */
#include <netdb.h>
#include <arpa/inet.h>

#include "Client_Helper.h"

enum {ACK, INIT};

 void sendStringClient(int connection_fd, char * buffer);
 void recvStringClient(int connection_fd, char * buffer, char * receivedData);
 int sendData(int connection_fd, char bf[]);

#endif /* CLIENT_COM_H */
