/*
*
*   Program :   Server_Helper.c
*
*   Authors  :  Cynthia Berenice Castillo Millán
 *               A01374530
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
 *  Function: initGame
 *
 *  Purpose:   Function to initialize all the information
 *             necessary. This will allocate memory for the
 *             players, and for the mutexes.
 *
 *  Parameters:
 *           Input   Receives a pointer to the game_t
 *                   structure and another one to the
 *                   data_locks structure.
 *
 */
void initGame(game_t * game, locks_t * data_locks)
{
    /* Set the number of transactions */
      game->running_players = 0;

    /* Allocate the arrays in the structures */
      game->player_array = malloc(NUM_PLAYERS * sizeof (player_t));
    /* Allocate the arrays for the mutexes */
      data_locks->player_mutex = malloc(NUM_PLAYERS * sizeof (pthread_mutex_t));

    /*
       Initialize the mutexes, using a different method for dynamically created
       ones
    */
      pthread_mutex_init(&data_locks->running_players_mutex, NULL);

      for (int i=0; i<NUM_PLAYERS; i++)
      {
          game->player_array[i].id = i+1;
          game->player_array[i].points = 0;
          pthread_mutex_init(&data_locks->player_mutex[i], NULL);
      }
}

int checkNumPlayers(game_t * game, locks_t * data_locks)
{
  /* Variable to store the mutex for readability*/
    pthread_mutex_t running_players_mutex = data_locks->running_players_mutex;
    int waiting = 0;

  /* Retrieve the balance of the account */
    pthread_mutex_lock(&running_players_mutex); /* Start the critical section */
        if(game->running_players<NUM_PLAYERS)
          waiting = 1;
    pthread_mutex_unlock(&running_players_mutex);   /* Finishes critical Section */

    return waiting;
}

int getNumPlayers(game_t * game, locks_t * data_locks)
{
  /* Variable to store the mutex for readability*/
    pthread_mutex_t running_players_mutex = data_locks->running_players_mutex;
    int num;

  /* Retrieve the balance of the account */
    pthread_mutex_lock(&running_players_mutex); /* Start the critical section */
        num = game->running_players;
    pthread_mutex_unlock(&running_players_mutex);   /* Finishes critical Section */

    return num;
}

void updateRunningThreads(int value, game_t * game, locks_t * data_locks)
{
  /* Variable to store the mutex for readability*/
    pthread_mutex_t running_players_mutex = data_locks->running_players_mutex;

  /* Number of threads Counter */
    pthread_mutex_lock(&running_players_mutex);    /* Start Critical Section */
        game->running_players = game->running_players + value;
    pthread_mutex_unlock(&running_players_mutex);  /* End Critical Section */
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
void initializeStruct(thread_data_t * connection_data, int connection_fd, game_t * game, locks_t * data_locks)
{
  /* Set the value of the file Descriptor */
    connection_data->connection_fd = connection_fd;

  /* Allocate bank_data structure pointer */
    connection_data->game = game;

  /* Allocate locks_t structure pointer */
    connection_data->locks = data_locks;
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
    game_t  * game       = connection_data->game;
    locks_t * data_locks = connection_data->locks;
    int connection_fd    = connection_data->connection_fd;

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

              sprintf(buffer, "%s", processData(buffer, connection_fd, game, data_locks));
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
char * processData(char buffer[], int fd, game_t * game, locks_t * data_locks)
{
    int  header;
    char message [BUFFER_SIZE] = "";
    char * response = (char *)malloc(BUFFER_SIZE * sizeof(char*));


    sscanf(buffer, "%d %s", &header, message);

    switch(header)
    {
      case CON:
                /*  * * * * * * * * * * * *  */
                /* Make it cleaner inside a function */

                if(checkNumPlayers(game, data_locks))
                {
                  sprintf(response, "%d Hello %s! We are currently waiting for other Players to arrive\n", WAIT, message);
                  printf("There are currently %d players connected", getNumPlayers(game, data_locks));
                }
                else
                  sprintf(response, "%d Hello %s! The game is about to Begin\n", INIT, message);
                break;
      case WAIT:
                printf("Players are waiting");
                while(1)
                {
                  if(!checkNumPlayers(game, data_locks))
                  {
                    printf("Everyone is ready");
                    sprintf(response, "%d GAME IS READY", INIT);
                    break;
                  }
                }

    }
        return response;
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
