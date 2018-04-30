/*
*
*   Program :   Server_Helper.h
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
*              dealer/server operations.
*
*   Usage  :
*              The program Receives all its info from the blackjack_server.c
*              program, and returns the data to it
*
*
*
*/

#ifndef SERVER_HELPER_H
#define SERVER_HELPER_H

/*   *   *   *   *   *   *   *   *   *   */
/*                                       */
/*                LIBRARIES              */
/*                                       */
/*   *   *   *   *   *   *   *   *   *   */

    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <unistd.h>

  /* Sockets Libraries */
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <ifaddrs.h>

  /* Posix threads library */
    #include <pthread.h>

  /* Custom Libraries */
    #include "error.h"
    #include "Server_Com.h"

/*   *   *   *   *   *   *   *   *   *   */
/*                                       */
/*            GLOBAL VARIABLES           */
/*                                       */
/*   *   *   *   *   *   *   *   *   *   */

  /* Defines */
    #define BUFFER_SIZE 255
    #define MIN_PLAYERS 2   /* Minimum number of players set to 2 for testing purposes */
    #define MAX_QUEUE 5

    enum { CON, ACK, WAIT, INIT }; /* MUST HAVE COPY ON CLIENT_COM */

/*   *   *   *   *   *   *   *   *   *   */
/*                                       */
/*          STRUCTURE DEFINITIONS        */
/*                                       */
/*   *   *   *   *   *   *   *   *   *   */

  /* ============== data_struct =============== */
  /*  Data that will be sent to each thread     */
  /* ========================================== */
    typedef struct data_struct {
      int connection_fd;    /* The file descriptor for the socket */
    } thread_data_t;



/*   *   *   *   *   *   *   *   *   *   */
/*                                       */
/*           FUNCTION DEFINITIONS        */
/*                                       */
/*   *   *   *   *   *   *   *   *   *   */

  /* Server Functions */
    int initServer(char * port);
    void checkThreadStatus(int status, pthread_t tid);

  /* Data processing */
    char * processData(char data[], pid_t pid);
    void attendRequest(int client_fd, int port);
    void initializeStruct(thread_data_t * connection_data, int connection_fd);
    void * attentionThread(void * arg);

  /* Running Threads */
    void updateRunningThreads(int value);
    int checkRunningThreads();

#endif  /* SERVER_HELPER_H */
