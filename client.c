/*
 *    The main client program for a multiplayer Pacman game.
 *    Written by Ludovic Cyril Michel, Rodolfo Verduzco and Cynthia Castillo.
 */
#include "client.h"

int main(int argc, char *argv[]) {
  int connection_fd;
  // show program usage
  if (argc != 3) {
    print_usage_error(argv[0], "<address> <port>");
  }
  // start the server
  connection_fd = open_socket(argv[1], argv[2]);
  // play the game
  test(connection_fd);
  // close the socket
  close(connection_fd);

  return 0;
}

int open_socket(char *address, char *port) {
  struct addrinfo hints;
  struct addrinfo *server_info = NULL;
  int connection_fd;

  // prepare the hints structure
  bzero(&hints, sizeof hints);
  // set to use IPv4
  hints.ai_family = AF_INET;
  // set type of socket
  hints.ai_socktype = SOCK_STREAM;
  // set to look for the address automatically
  hints.ai_flags = AI_PASSIVE;

  // use the presets to get the actual information for the socket
  if (getaddrinfo(address, port, &hints, &server_info) == -1) {
    print_network_error("getaddrinfo", 1);
  }

  // open the socket using the information obtained
  connection_fd = socket(server_info->ai_family, server_info->ai_socktype,
                         server_info->ai_protocol);
  if (connection_fd == -1) {
    close(connection_fd);
    print_network_error("socket", 1);
  }

  // connect to the server
  if (connect(connection_fd, server_info->ai_addr, server_info->ai_addrlen) ==
      -1) {
    print_network_error("connect", 1);
  }

  // free the memory used for the address info
  freeaddrinfo(server_info);

  return connection_fd;
}

void send_request(char *buffer, int connection_fd) {
  // send
  if (send(connection_fd, buffer, strlen(buffer) + 1, 0) == -1) {
    print_network_error("send", 0);
  }
  printf("sent %s\n", buffer);
}

void print_response(char *response, char *buffer, int connection_fd) {
  int response_size;
  bzero(buffer, BUFFER_SIZE);
  // clean the response string
  bzero(response, BUFFER_SIZE);
  // read from the buffer
  response_size = recv(connection_fd, buffer, BUFFER_SIZE, 0);
  if (response_size == -1) {
    print_network_error("recv", 0);
  }
  // write the response text
  printf("received %s\n", buffer);
}

int test(int connection_fd) {
  char *buffer = (char *)malloc(BUFFER_SIZE);
  char *response = (char *)malloc(BUFFER_SIZE);
  int type;
  char data[50];

  // get the initial two cards of the player, and show the first card of the
  // dealer
  printf("TESTING...\n");

  // ask the player for his decisions
  while (1) {
    bzero(buffer, BUFFER_SIZE);
    printf("\nEnter the buffer to send: ");
    scanf("%d %[^\t\n]", &type, data);
    sprintf(buffer, "%d %s", type, data);

    // send the request
    send_request(buffer, connection_fd);

    // print the response
    print_response(response, buffer, connection_fd);
  }

  // free the memory
  free(buffer);
  free(response);

  return 0;
}
