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
    #define BUFFER_SIZE 1024
    #define NUM_PLAYERS 2   /* Minimum number of players set to 2 for testing purposes */
    #define MAX_QUEUE 2

    enum { CON, ACK, WAIT, INIT }; /* MUST HAVE COPY ON CLIENT_COM */

/*   *   *   *   *   *   *   *   *   *   */
/*                                       */
/*          STRUCTURE DEFINITIONS        */
/*                                       */
/*   *   *   *   *   *   *   *   *   *   */

  /* =================== player_struct ==================== */
  /*  Structure to hold each player's information           */
  /* ====================================================== */
    typedef struct player_struct {
        int id;
        char * name;
        int points;
        int pacman;
    } player_t;

    /* ============ game_struct ============ */
    /*  Data for the bank operations         */
    /* ===================================== */
      typedef struct game_struct   {
          int running_players;          /* Number of playeers counter */
          player_t * player_array;    /* Array of the accounts */
      } game_t;

    /* ==================== locks_struct ==================== */
    /*  Structure for the mutexes to keep the data consistent */
    /* ====================================================== */
      typedef struct locks_struct {
          //pthread_mutex_t transactions_mutex; /* Mutex for the number of transactions variable  */
          pthread_mutex_t * player_mutex;     /* Mutex array for the operations on the players */
          pthread_mutex_t running_players_mutex;       /* Mutex for the Number of threads Running */
      } locks_t;


  /* ============== data_struct =============== */
  /*  Data that will be sent to each thread     */
  /* ========================================== */
    typedef struct data_struct {
      int connection_fd;    /* The file descriptor for the socket */
      game_t * game;
      locks_t  * locks;
    } thread_data_t;



/*   *   *   *   *   *   *   *   *   *   */
/*                                       */
/*           FUNCTION DEFINITIONS        */
/*                                       */
/*   *   *   *   *   *   *   *   *   *   */

  /* Server Functions */
    int initServer(char * port);
    void initGame(game_t * game, locks_t * data_locks);
    void checkThreadStatus(int status, pthread_t tid);

  /* Data processing */
    char * processData(char data[], int fd,  game_t * game, locks_t * data_locks);
    void attendRequest(int client_fd, int port);
    void initializeStruct(thread_data_t * connection_data, int connection_fd, game_t * game, locks_t * data_locks);
    void * attentionThread(void * arg);

  /* Running Threads */
    void updateRunningThreads(int value, game_t * game, locks_t * data_locks);
    int checkNumPlayers(game_t * game, locks_t * data_locks);
    int getNumPlayers(game_t * game, locks_t * data_locks);

#endif  /* SERVER_HELPER_H */
