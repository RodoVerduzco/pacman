/*
*
*   Program :   Server_Com.h
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
*              This program handles the server comomunication with
*              the client.
*
*/

#ifndef SERVER_COM_H
#define SERVER_COM_H

/* Custom Libraries */
#include "Server_Helper.h"

void sendStringServer(int connection_fd, char * buffer);
int recvStringThread(int connection_fd, char * buffer);

#endif /* SERVER_HELPER_H */
