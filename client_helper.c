#include "client_helper.h"

void play(int server_fd) {
  char *data = (char *)malloc(sizeof(char) * BUFFER_SIZE);
  int type;
  int player_num;
  int player_id;

  // logical thread
  pthread_t ltid;

  // graphical thread
  pthread_t gtid;

  printf("Enter your player's name: ");
  fflush(stdout);
  scanf("%s", data);

  send_request(server_fd, INIT, data);
  get_response(server_fd, &type, data);

  sscanf(data, "%d %d", &player_num, &player_id);

  game_state_t *game_state = init_game_state(player_num);

  thread_data_t *thread_data = (thread_data_t *)malloc(sizeof(thread_data_t));
  thread_data->connection_fd = server_fd;
  thread_data->game_state = game_state;
  thread_data->player_id = player_id;
  thread_data->player_num = player_num;

  pthread_create(&ltid, NULL, handle_interactions, thread_data);
  pthread_create(&gtid, NULL, handle_gui, thread_data);

  pthread_join(ltid, NULL);
}

void *handle_interactions(void *arg) {
  thread_data_t *thread_data = (thread_data_t *)arg;
  int server_fd = thread_data->connection_fd;
  int player_num = thread_data->player_num;
  int player_id = thread_data->player_id;
  game_state_t *game_state = thread_data->game_state;

  char *data = (char *)malloc(sizeof(char) * BUFFER_SIZE);
  int type;

  while (1) {
    bzero(data, BUFFER_SIZE);
    get_keys_pressed(player_id, game_state, data, &type);
    send_request(server_fd, type, data);
    get_response(server_fd, &type, data);
    parse_game_state(game_state, player_num, data);

    if (type == WAIT) {
      pthread_mutex_lock(&game_state->pacman_id_lock);
      game_state->pacman_id++;
      pthread_mutex_unlock(&game_state->pacman_id_lock);
      break;
    }
  }

  pthread_exit(NULL);
}

void *handle_gui(void *arg) {
  thread_data_t *thread_data = (thread_data_t *)arg;
  int player_num = thread_data->player_num;
  game_state_t *game_state = thread_data->game_state;

  int pacman_id;
  int current_pacman_id;

  pthread_mutex_lock(&game_state->pacman_id_lock);
  pacman_id = game_state->pacman_id;
  pthread_mutex_unlock(&game_state->pacman_id_lock);

  init_gui();
  draw_map();

  while (1) {
    draw_players(game_state, player_num);

    pthread_mutex_lock(&game_state->pacman_id_lock);
    current_pacman_id = game_state->pacman_id;
    pthread_mutex_unlock(&game_state->pacman_id_lock);

    if (current_pacman_id != pacman_id) {
      break;
    }
  }

  endwin();

  pthread_exit(NULL);
}

/*
  This function detects if the arrow keys where pressed within the
  timeout to change player's position, if not it updates other player's
  position
*/
void get_keys_pressed(const int player_id, game_state_t *game_state, char *data,
                      int *type) {
  int pos_x;
  int pos_y;

  timeout(100);
  switch (getch()) {
  case 65: // up
    pthread_mutex_lock(&game_state->player_data_lock);
    pos_x = game_state->player_data[player_id].x;
    pos_y = game_state->player_data[player_id].y + 1;
    pthread_mutex_unlock(&game_state->player_data_lock);
    sprintf(data, "%d %d", pos_x, pos_y);
    *type = MOVE;
    break;
  case 66: // down
    pthread_mutex_lock(&game_state->player_data_lock);
    pos_x = game_state->player_data[player_id].x;
    pos_y = game_state->player_data[player_id].y - 1;
    pthread_mutex_unlock(&game_state->player_data_lock);
    sprintf(data, "%d %d", pos_x, pos_y);
    *type = MOVE;
    break;
  case 67: // right
    pthread_mutex_unlock(&game_state->player_data_lock);
    pos_x = game_state->player_data[player_id].x + 1;
    pos_y = game_state->player_data[player_id].y;
    pthread_mutex_unlock(&game_state->player_data_lock);
    sprintf(data, "%d %d", pos_x, pos_y);
    *type = MOVE;
    break;
  case 68: // left
    pthread_mutex_lock(&game_state->player_data_lock);
    pos_x = game_state->player_data[player_id].x - 1;
    pos_y = game_state->player_data[player_id].y;
    sprintf(data, "%d %d", pos_x, pos_y);
    pthread_mutex_unlock(&game_state->player_data_lock);
    *type = MOVE;
    break;
  default:
    *type = ACK;
  }
}
