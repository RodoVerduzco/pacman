/*
 *    The main client program for a multiplayer Pacman game.
 *    Written by Ludovic Cyril Michel, Rodolfo Verduzco and Cynthia Castillo.
 */
#include "client_test.h"

int main(int argc, char *argv[]) {
  int server_fd;

  // show program usage
  if (argc != 3) {
    print_usage_error(argv[0], "<address> <port>");
  }
  // start the server
  server_fd = open_socket(argv[1], argv[2]);
  // play the game
  play(server_fd, 1);

  // close the socket
  close(server_fd);

  return 0;
}
/*
  This Function opens the socket to the server.
*/
int open_socket(char *address, char *port) {
  struct addrinfo hints;
  struct addrinfo *server_info = NULL;
  int server_fd;

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
  server_fd = socket(server_info->ai_family, server_info->ai_socktype,
                     server_info->ai_protocol);
  if (server_fd == -1) {
    close(server_fd);
    print_network_error("socket", 1);
  }

  // connect to the server
  if (connect(server_fd, server_info->ai_addr, server_info->ai_addrlen) == -1) {

    print_network_error("connect", 1);
  }

  // free the memory used for the address info
  freeaddrinfo(server_info);

  return server_fd;
}
