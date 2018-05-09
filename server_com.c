/*
 *    The program that performs the server communication with the client
 *    Written by Ludovic Cyril Michel, Rodolfo Verduzco and Cynthia Castillo.
 */

#include "server_com.h"

/*
  This function receives the client request, and saves
  the request to 'data'
 */
int get_request(int client_fd, int *type, char *data) {
  char buffer[BUFFER_SIZE];

  // clean the buffer
  bzero(buffer, BUFFER_SIZE);
  bzero(data, BUFFER_SIZE);

  // get the request and check for error cases
  int request_size = recv(client_fd, buffer, BUFFER_SIZE, 0);
  if (request_size == 0 || request_size == -1) {
    print_network_error("recv", 0);
    return -1;
  }

  // parse the data
  sscanf(buffer, "%d %[^\t\n]", type, data);
  printf("\nreceived %d %s", *type, data);

  return 0;
}

/*
  This function sends a response to the client.
 */
int send_response(int client_fd, int type, char *data) {
  char buffer[BUFFER_SIZE];

  // clean the buffer
  bzero(buffer, BUFFER_SIZE);

  // concat the com code and the data
  sprintf(buffer, "%d %s", type, data);

  // send the response
  if (send(client_fd, buffer, strlen(buffer) + 1, 0) == -1) {
    print_network_error("send", 0);
    return -1;
  }

  printf("\nsent %s\n", buffer);

  if (data != NULL) {
    bzero(data, BUFFER_SIZE);
  }

  return 0;
}

/*
  This function sets the poll to await for the clients
  request.
 */
int await_request(int client_fd) {
  struct pollfd poll_fds[1];
  int poll_result;

  // Fill the poll structure
  poll_fds[0].fd = client_fd;
  poll_fds[0].events = POLLIN;

  poll_result = poll(poll_fds, 1, TIMEOUT);

  if (poll_result == -1) {
    print_network_error("poll", 0);
  }
  if (poll_fds[0].revents & POLLIN) {
    return 1;
  }
  return 0;
}

/*
  This function converts the game state to the string that will
  be the response to the client.
 */
void stringify_game_state(game_state_t *game_state, int player_num,
                          char *data) {
  char temp[BUFFER_SIZE] = "\0";
  bzero(data, BUFFER_SIZE);

  // Stringify the state being consistent with the data.
  pthread_mutex_lock(&game_state->player_data_lock);
  for (int player_id = 0; player_id < player_num; player_id++) {
    sprintf(temp, "%d %s %d %d %d/", player_id,
            (game_state->player_data + player_id)->name,
            (game_state->player_data + player_id)->score,
            (game_state->player_data + player_id)->x,
            (game_state->player_data + player_id)->y);
    strcat(data, temp);
  }
  pthread_mutex_unlock(&game_state->player_data_lock);
}

/*
  this function parses the change request into a string
 */
void parse_change_request(char *data, int *x, int *y) {
  sscanf(data, "%d %d", x, y);
}
