/*
 *    The main client helper program for a multiplayer Pacman game.
 *    Written by Ludovic Cyril Michel, Rodolfo Verduzco and Cynthia Castillo.
 */

#ifndef CLIENT_H
#define CLIENT_H
#define BUFFER_SIZE 1024

// Custom Libriaries
#include "client.h"
#include "ClientUI.h"
#include "error.h"
#include "client_types.h"

// Support Libraries
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <curses.h>

/*
Enums to handle the communication protocol
Communication protocol explained in the documentation
*/
typedef enum { INIT, MOVE, ACK } request_t;
typedef enum { WAIT, CHANGE, GAMEOVER, ERROR } response_t;

int open_socket(char *address, char *port);
void send_request(char *buffer, int connection_fd);
void print_response(char *response, char *buffer, int connection_fd);
int play(int connection_fd);
void * handle_logic(void * arg);
void parse_data(char * buffer, int *type, char * data);

#endif /* CLIENT_H */
