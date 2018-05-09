/*
 *    The program that handles main game state of the pacman game.
 *    Written by Ludovic Cyril Michel, Rodolfo Verduzco and Cynthia Castillo.
 */
#include "game.h"

/*
  Set the initial state of the game
*/
game_state_t *init_game_state(int player_num) {
  game_state_t *game_state = (game_state_t *)malloc(sizeof(game_state_t));

  game_state->player_count = 0;
  game_state->pacman_id = 0;
  game_state->player_data =
      (player_data_t *)malloc(sizeof(player_data_t) * player_num);

  for (int i = 0; i < player_num; i++) {
    game_state->player_data[i].x = init_x(i);
    game_state->player_data[i].y = init_y(i);
    game_state->player_data[i].score = 0;
  }

  pthread_mutex_init(&game_state->player_count_lock, NULL);
  pthread_mutex_init(&game_state->pacman_id_lock, NULL);
  pthread_mutex_init(&game_state->player_data_lock, NULL);

  return game_state;
}

/*
  Check coordinates of a given player
*/
int check_coordinates(game_state_t *game_state, int player_num, int player_id,
                      int x, int y) {
  if (!check_displacement(game_state, player_id, x, y)) {
    return 0;
  }
  return check_other_players(game_state, player_num, player_id, x, y);
}

/*
  Check the correct displacement of the players
*/
int check_displacement(game_state_t *game_state, int player_id, int x, int y) {
  int displacement = 0;
  displacement += abs(x - game_state->player_data[player_id].x);
  displacement += abs(y - game_state->player_data[player_id].y);
  return displacement <= 1;
}

/*
  Check the correct displacement of the players
*/
int check_walls(game_state_t *game_state, int player_id, int x, int y) {
  int displacement = 0;
  displacement += abs(x - game_state->player_data[player_id].x);
  displacement += abs(y - game_state->player_data[player_id].y);
  return displacement <= 1;
}

/*
  Check the coordinates of the other players
*/
int check_other_players(game_state_t *game_state, int player_num, int player_id,
                        int x, int y) {
  int other_player_x;
  int other_player_y;
  int pacman_id = game_state->pacman_id;

  // Go through every player
  for (int i = 0; i < player_num; i++) {
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

/*
  Reset the player coordinates
*/
void reset_coordinates(game_state_t *game_state, int player_num) {
  pthread_mutex_lock(&game_state->player_data_lock);
  for (int i = 0; i < player_num; i++) {
    game_state->player_data[i].x = 0;
    game_state->player_data[i].y = 0;
  }
  pthread_mutex_unlock(&game_state->player_data_lock);
}
