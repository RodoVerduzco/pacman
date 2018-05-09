/*
 *    The helper for the game program of a pacman game.
 *    Written by Ludovic Cyril Michel, Rodolfo Verduzco and Cynthia Castillo.
 */
#ifndef GAME_H
#define GAME_H

#include "types.h"

#include <stdlib.h>

// Function Declaration
game_state_t *init_game_state(int player_num);
int check_coordinates(game_state_t *game_state, int player_num, int player_id,
                      int x, int y);
int check_displacement(game_state_t *game_state, int player_id, int x, int y);
int check_other_players(game_state_t *game_state, int player_num, int player_id,
                        int x, int y);
void reset_coordinates(game_state_t *game_state, int player_num);

#endif /* GAME_H */
