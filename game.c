#include "game.h"

game_state_t *init_game_state() {
  game_state_t *game_state = (game_state_t *)malloc(sizeof(game_state_t));

  game_state->player_count = 0;
  game_state->pacman_id = 0;
  game_state->player_data =
      (player_data_t *)malloc(sizeof(player_data_t) * PLAYER_NUM);

  for (int i = 0; i < PLAYER_NUM; i++) {
    game_state->player_data[i].x = i;
    game_state->player_data[i].y = i;
    game_state->player_data[i].score = 0;
  }

  pthread_mutex_init(&game_state->player_count_lock, NULL);
  pthread_mutex_init(&game_state->pacman_id_lock, NULL);
  pthread_mutex_init(&game_state->player_data_lock, NULL);

  return game_state;
}

int check_coordinates(game_state_t *game_state, int player_id, int x, int y) {
  if (!check_displacement(game_state, player_id, x, y)) {
    return 0;
  }
  return check_other_players(game_state, player_id, x, y);
}

int check_displacement(game_state_t *game_state, int player_id, int x, int y) {
  int displacement = 0;
  displacement += abs(x - game_state->player_data[player_id].x);
  displacement += abs(y - game_state->player_data[player_id].y);
  return displacement <= 1;
}

int check_other_players(game_state_t *game_state, int player_id, int x, int y) {
  int other_player_x;
  int other_player_y;
  int pacman_id = game_state->pacman_id;

  for (int i = 0; i < PLAYER_NUM; i++) {
    other_player_x = game_state->player_data[i].x;
    other_player_y = game_state->player_data[i].y;
    if (i != player_id && other_player_x == x && other_player_y == y) {
      if (i == pacman_id || player_id == pacman_id) {
        return 2;
      } else {
        return 0;
      }
    }
  }

  return 1;
}

void reset_coordinates(game_state_t *game_state) {
  pthread_mutex_lock(&game_state->player_data_lock);
  for (int i = 0; i < PLAYER_NUM; i++) {
    game_state->player_data[i].x = 0;
    game_state->player_data[i].y = 0;
  }
  pthread_mutex_unlock(&game_state->player_data_lock);
}
