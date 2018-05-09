/*
 *    The main client helper program for a multiplayer Pacman game.
 *    Written by Ludovic Cyril Michel, Rodolfo Verduzco and Cynthia Castillo.
 */

#ifndef CLIENT_TEST_H
#define CLIENT_TEST_H
#define BUFFER_SIZE 1024

// Custom Libriaries
#include "client_helper.h"
#include "error.h"

// Support Libraries
#include <arpa/inet.h>
#include <curses.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int open_socket(char *address, char *port);
void print_response(char *response, char *buffer, int connection_fd);

#endif /* CLIENT_H */
