/*
*
*   Program :   client_helper.c
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
*              This program has the handler functios for to the
*              perform the game.
*
*/


/* Custom includes */
#include "Client_Helper.h"


/*
 *
 *  Function: processReceivedData
 *
 *  Purpose: This function handles the received data, finds
 *           what kind of data was received to send it to
 *           the correct function
 *
 *  Parameters:
 *           Input   The received data as a string
 *
 *           Output  Depends on the data received.
 */
int processReceivedData(char data[])
{
  int header;
  char message [BUFFER_SIZE] = "";

  sscanf(data, "%d %[^\n]s", &header, message);

    switch(header){

      case WAIT:
             printf("\n%s\n\n", message);
             return WAIT;
             //sprintf(response, "%d WILL WAIT", WAIT);
      case INIT:
            printf("INIT");
            return INIT;

    }

      return 0;
}

/*
 *
 *  Function: sendData
 *
 *  Purpose: This function sends the data though the sockets
 *           to the server
 *
 *  Parameters:
 *           Input   The file descriptor and the data to be
 *                    sent
 *
 *           Output  Doesn't return anything.
 */
int sendData(int connection_fd, char bf[])
{
  char buffer[BUFFER_SIZE];
  char receivedData[BUFFER_SIZE];

  /* Clear the buffer */
  bzero(&buffer, BUFFER_SIZE);

  /* Prepare the request to the server */
    sprintf(buffer, "%s\n", bf);

 /* Send the Data */
    sendStringClient(connection_fd, buffer);

 /* Receive a Response */
    recvStringClient(connection_fd, buffer, receivedData);

  /* Return the data alredy processed */
    return processReceivedData(receivedData);
}
