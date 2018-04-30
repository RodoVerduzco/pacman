/*
*
*   Program :   Server_Com.c
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
*   Usage  :
*
*
*   Error handling:
*               On any unrecoverable error, the program shows an error
*               message, and exits.
*
*/

#include "Server_Com.h"
/*
    Send a message with error validation
    Receive the file descriptor and the string pointer
*/
void sendStringServer(int connection_fd, char * buffer)
{
    // Send a message to the client, including an extra character for the '\0'
    if ( send(connection_fd, buffer, strlen(buffer)+1, 0) == -1 )
    {
        fatalError("ERROR: send");
    }
}
