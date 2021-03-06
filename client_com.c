#include "server_com.h"

int get_response(int server_fd, int *type, char *data) {
  char buffer[BUFFER_SIZE];

  // clean the buffer
  bzero(buffer, BUFFER_SIZE);
  bzero(data, BUFFER_SIZE);

  // get the request and check for error cases
  int request_size = recv(server_fd, buffer, BUFFER_SIZE, 0);
  if (request_size == 0 || request_size == -1) {
    print_network_error("recv", 0);
    return -1;
  }

  // parse the data
  sscanf(buffer, "%d %[^\t\n]", type, data);

  return 0;
}

int send_request(int server_fd, int type, char *data) {
  char buffer[BUFFER_SIZE];

  // clean the buffer
  bzero(buffer, BUFFER_SIZE);

  // concat the com code and the data
  sprintf(buffer, "%d %s", type, data);

  // send the response
  if (send(server_fd, buffer, strlen(buffer) + 1, 0) == -1) {
    print_network_error("send", 0);
    return -1;
  }

  if (data != NULL) {
    bzero(data, BUFFER_SIZE);
  }

  return 0;
}

int await_response(int server_fd) {
  struct pollfd poll_fds[1];
  int poll_result;

  poll_fds[0].fd = server_fd;
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

void parse_game_state(game_state_t *game_state, int player_num, char *data) {
  char *temp = (char *)malloc(sizeof(char) * BUFFER_SIZE);
  int player_id_temp;

  temp = strtok(data, "/");

  pthread_mutex_lock(&game_state->player_data_lock);
  for (int player_id = 0; player_id < player_num; player_id++) {
    sscanf(temp, "%d %s %d %d %d", &player_id_temp,
           (game_state->player_data + player_id)->name,
           &(game_state->player_data + player_id)->score,
           &(game_state->player_data + player_id)->x,
           &(game_state->player_data + player_id)->y);
    temp = strtok(NULL, "/");
  }
  pthread_mutex_unlock(&game_state->player_data_lock);

  free(temp);
}

void stringify_change_request(char *data, int x, int y) {
  bzero(data, BUFFER_SIZE);
  sprintf(data, "%d %d", x, y);
}
