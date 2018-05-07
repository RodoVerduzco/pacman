/*
 *    The main serever helper program.
 *    Written by Ludovic Cyril Michel, Rodolfo Verduzco and Cynthia Castillo.
 */

// Helper Libraries
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Custom Libraries
#include "error.h"
#include "server_helper.h"

#define MAX_QUEUE 5

// Function Declaration
int main(int argc, char *argv[]);
void print_ips();
int init_server(char *port);
void create_games(int server_fd);
void *handle_players(void *arg);
