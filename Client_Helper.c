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

    int header    = atoi(strtok(data, "_")); /* To identify the data */
    char* message = strtok(NULL, "_");      /* The real message from sever */

    switch(header){

      case 0:
             printf("   > Received Message from the server: %s\n\n", message);
             return 0;

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
  int chars_read;
  char receivedData[BUFFER_SIZE];

  /* Prepare the request to the server */
    sprintf(buffer, "%s\n", bf);

  /*  *  *  *  *  SEND  *  *  *  *  */
  /* Send the response */
    if (send(connection_fd, buffer, strlen(buffer) + 1, 0) == -1 )
    {
        fatalError("send");
        fatalErrorMsg("sendData/send", "Couldn't send the response to the Server");
    }

  /* Clear the buffer */
    bzero(&buffer, BUFFER_SIZE);

  /*  *  *  *  *  RECV  *  *  *  *  */
  /* Receive the request from the server */
    chars_read = recv(connection_fd, buffer, BUFFER_SIZE, 0);
    if (chars_read == -1)
    {
        fatalError("recv");
        fatalErrorMsg("sendData/recv", "Couldn't receive the request from the server");
    }

  /* Scans everything including spaces */
    sscanf(buffer, "%[^\t]", receivedData);

  /* Return the data alredy processed */
    return processReceivedData(receivedData);
}
C