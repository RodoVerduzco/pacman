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

/* Number of threads Running */
  pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;
  int running_threads = 0;

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

int checkRunningThreads()
{
  if(running_threads >= MAX_QUEUE)
    return 0;
  else
    if(running_threads > 0)
      if(running_threads > MIN_PLAYERS)
        return 1;
      else
          return 0;
    else
      return 1;
}

void updateRunningThreads(int value)
{
  /* Number of threads Counter */
    pthread_mutex_lock(&running_mutex);    /* Start Critical Section */
      running_threads = running_threads + value;
    pthread_mutex_unlock(&running_mutex);  /* End Critical Section */
}

/*
 *  Function: initializeStruct
 *
 *  Purpose:   Initializes the  values that are going to be
 *             sent to a thread.
 *
 *  Parameters:
 *           Input   Receives the thread_data_t struct to be sent,
 *                   and the values to be stored in it (file descriptor,
 *                   and the pointers to bank_t and locks_t structures).
 *
 */
void initializeStruct(thread_data_t * connection_data, int connection_fd)
{
  /* Set the value of the file Descriptor */
    connection_data->connection_fd = connection_fd;
}

/*
 *  Function: attentionThread
 *
 *  Purpose:   Handles a signle client session (THREAD FUCNTION)
 *
 *  Parameters:
 *           Input   Receives a void pointer to the arguments sent
 *                   by the main Thread
 *
 */
void * attentionThread(void * arg)
{
  /* Receive the data for the bank, mutexes and socket file descriptor */
    thread_data_t * connection_data = (thread_data_t *) arg;

  /*
     Store the values from connection data struct in other variables for
     readability.
  */
    int connection_fd = connection_data->connection_fd;

    char buffer[BUFFER_SIZE] = ""; /* To store the message received and to be sent */

    /* Loop to listen for messages from the client */
      while(1)
      {
          // Clear the buffer to avoid errors
            bzero(&buffer, BUFFER_SIZE);

            /* Get the received data */
              int recv_val = recvStringThread(connection_fd, buffer);

              if(recv_val == 0)
                return (void *)0;

              sprintf(buffer, "%s", processData(buffer, connection_fd));
              printf("BUFFER  %s", buffer);
              sendStringServer(connection_fd, buffer);
      }
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
 *           Input   Strings with the data, and the File
 *                   Descriptor
 *
 *           Output  returns a string with the new data to be
 *                    sent to the client.
 */
char * processData(char buffer[], int fd)
{
    int header;
    char message [BUFFER_SIZE] = "";
    char * response = (char *)malloc(sizeof(char*));


    sscanf(buffer, "%d %s", &header, message);

    switch(header)
    {
      case CON:
                /* Make it cleaner inside a function */
                if(running_threads<MIN_PLAYERS)
                  sprintf(response, "%d Waiting for other Players to arrive\n", WAIT);
                else
                  sprintf(response, "%d GAME IS READY\n", INIT);
                break;
      case WAIT:
                while(1)
                {
                  if(running_threads>MIN_PLAYERS)
                  {
                    printf("IS WAITING");
                    sprintf(response, "GAME IS READY");
                    break;
                  }
                }

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

/*
 *  Check if the thead was created correctly
 */
void checkThreadStatus(int status, pthread_t tid)
{
  if(status == 0){
    printf("\t>Thread created with ID: %lu\n", tid);
  }

  if (status != 0){
    perror("ERROR: pthread_create");
    exit(EXIT_FAILURE);
  }
}
