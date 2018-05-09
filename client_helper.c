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

  printf("%s", game_state->player_data[player_id].name);

  initscr();  // ncurses init
  noecho();   // Dont show keys pressed in console

  while (1){
    bzero(data, BUFFER_SIZE);
    get_keys_pressed(player_id, game_state, data, &type);
    send_request(server_fd, type, data);
    get_response(server_fd, &type, data);
    parse_game_state(game_state, player_num, data);

  }

  fflush(stdout);
}

/*
  This function detects if the arrow keys where pressed within the
  timeout to change player's position, if not it updates other player's
  position
*/
void get_keys_pressed(const int player_id, game_state_t *game_state, char *data, int *type)
{
  int pos_x;
  int pos_y;

  timeout(3000);
  if (getch() == '\033') {
    getch(); // skip the [
    switch(getch()) {
        case 'A':   // up
            pos_x = game_state->player_data[player_id].x;
            pos_y = game_state->player_data[player_id].y + 1;
            sprintf(data, "%d %d", pos_x, pos_y);
            *type = MOVE;
            break;
        case 'B':   // down
            pos_x = game_state->player_data[player_id].x;
            pos_y = game_state->player_data[player_id].y - 1;
            sprintf(data, "%d %d", pos_x, pos_y);
            *type = MOVE;
            break;
        case 'C':   // right
            pos_x = game_state->player_data[player_id].x + 1;
            pos_y = game_state->player_data[player_id].y;
            sprintf(data, "%d %d", pos_x, pos_y);
            *type = MOVE;
            break;
        case 'D':   // left
            pos_x = game_state->player_data[player_id].x - 1;
            pos_y = game_state->player_data[player_id].y;
            sprintf(data, "%d %d", pos_x, pos_y);
            *type = MOVE;
            break;
          }
  }
  else {
    *type = ACK;
  }
}
