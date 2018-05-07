#ifndef CLIENT_H
#define CLIENT_H

// Custom Libriaries
#include "client.h"
#include "ClientUI.h"
#include "error.h"

// Support Libraries
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int open_socket(char *address, char *port);
void send_request(char *buffer, int connection_fd);
void print_response(char *response, char *buffer, int connection_fd);
int test(int connection_fd);

#endif /* CLIENT_H */
