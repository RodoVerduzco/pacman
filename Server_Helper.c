/*
*
*   Program :   Game_Server.c
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
*              server operations.
*
*   Usage  :
*              The program Receives all its info from the Server.c
*              program, and returns the data to it
*
*   Error handling:
*               On any unrecoverable error, the program shows an error
*               message, and exits.
*
*/


#include "Server_Helper.h"

/*
 *
 *  Function: processData
 *
 *  Purpose: This Function processes the data received from
 *            the client to know how to manipulate it, and
 *            what function to send the data to..
 *
 *  Parameters:
 *           Input   Strings with the data, and the port to
 *                   recogrnize the incoming child
 *
 *           Output  returns a string with the new data to be
 *                    sent to the client.
 */
char* processData(char data[], int port)
{
    int header    = atoi((const char*)strtok(data, "_")); /* To identify the data */
    char* message = strtok(NULL, "_");  /* The main message received */
    char* response = (char *)malloc(sizeof(char*));

    switch(header)
    {
      case 0:
                printf("\t>Client from the port %d connected\n", port);
                sprintf(response, "0_Hi\n");
                break;

    }
        return response;
}
