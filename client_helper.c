#include "client_helper.h"

void play(int server_fd) {
  char *data = (char *)malloc(sizeof(char) * BUFFER_SIZE);
  int type;
  int player_num;
  int player_id;

  printf("Enter your player's name: ");
  fflush(stdout);
  scanf("%s", data);

  send_request(server_fd, INIT, data);
  get_response(server_fd, &type, data);

  sscanf(data, "%d %d", &player_num, &player_id);

  game_state_t *game_state = init_game_state(player_num);

  send_request(server_fd, ACK, data);
  get_response(server_fd, &type, data);

  parse_game_state(game_state, player_num, data);

  printf("%s", game_state->player_data[player_id].name);

  fflush(stdout);
}
