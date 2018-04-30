/*
*
*   Program :   Server_Helper.c
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


int initServer(char * port)
{
    struct addrinfo hints;
    struct addrinfo * server_info = NULL;
    int server_fd;
    int reuse = 1;

    /* Prepare the hints structure */
      bzero (&hints, sizeof hints);
    /* Set to use IPv4 */
      hints.ai_family = AF_INET;
    /* Set type of socket */
      hints.ai_socktype = SOCK_STREAM;
    /* Set to look for the address automatically */
      hints.ai_flags = AI_PASSIVE;

    /*  *  *  *  *  GETADDRINFO  *  *  *  *  */
    /* Use the presets to get the actual information for the socket */
      if (getaddrinfo(NULL, port, &hints, &server_info) == -1)
      {
          fatalError("getaddrinfo");
      }

    /*  *  *  *  *  SOCKET  *  *  *  *  */
    /* Open the socket using the information obtained */
      server_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
      if (server_fd == -1)
      {
          close(server_fd);
          fatalError("socket");
      }

    /*  *  *  *  *  SETSOCKOPT  *  *  *  *  */
    /* Allow reuse of the same port even when the server does not close correctly */
      if (setsockopt (server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof (int)) == -1)
      {
          fatalError("setsockopt");
      }

    /*  *  *  *  *  BIND  *  *  *  *  */
    /* Connect the port with the desired port */
      if (bind(server_fd, server_info->ai_addr, server_info->ai_addrlen) == -1)
      {
          fatalError("bind");
      }

    /*  *  *  *  *  LISTEN  *  *  *  *  */
    /* Start listening for incomming connections */
      if (listen(server_fd, MAX_QUEUE) == -1)
      {
          fatalError("listen");
      }

    /*  *  *  *  *  FREEADDRINFO  *  *  *  *  */
    /* Free the memory used for the address info */
      freeaddrinfo(server_info);

      printf("Server ready\n");

      return server_fd;
}

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
                printf("\t\t>Client Message %s: \n", message);
                sprintf(response, "0_Hi\n");
                break;

    }
        return response;
}

/*
    Hear the request from the client and send an answer
*/
void attendRequest(int client_fd, int port)
{
    char buffer[BUFFER_SIZE];
    char clientData[BUFFER_SIZE];
    int chars_read;

    while(1)
    {
      // Clear the buffer to avoid errors
        bzero(&buffer, BUFFER_SIZE);

      // RECV
      // Read the request from the client
        chars_read = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if (chars_read == 0)
        {
          printf("Client disconnected");
          return;
        }
        if (chars_read == -1)
        {
          printf("Client receive error");
          return;
        }

      /* Get the received data */
        sscanf(buffer, "%s", clientData);

      /* Store the new data to the buffer*/
        sprintf(buffer, "%s", processData(clientData, port));

      // SEND
      // Write back the reply

        if (send(client_fd, buffer, strlen(buffer) + 1, 0) == -1)
        {
          printf("Could not send reply");
        }
    }
}
