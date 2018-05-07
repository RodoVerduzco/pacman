#ifndef GAME_H
#define GAME_H

#include "server_types.h"
#include <stdlib.h>

game_state_t *init_game_state();
int check_coordinates(game_state_t *game_state, int player_id, int x, int y);
int check_displacement(game_state_t *game_state, int player_id, int x, int y);
int check_other_players(game_state_t *game_state, int player_id, int x, int y);
void reset_coordinates(game_state_t *game_state);

#endif
