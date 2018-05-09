/*
 *    The helper program for the server_helper.h the pacman game.
 *    Written by Ludovic Cyril Michel, Rodolfo Verduzco and Cynthia Castillo.
 */

#ifndef SERVER_HELPER_H
#define SERVER_HELPER_H

#include <signal.h>
#include <unistd.h>

// Custom Libraries
#include "game.h"
#include "server_com.h"

void serve(int client_fd, int player_num, int player_id,
           game_state_t *game_state);

void handle_init_request(int client_fd, int player_num, int player_id,
                         game_state_t *game_state, char *data);
void handle_game_requests(int client_fd, int player_num, int player_id,
                          game_state_t *game_state, char *data);
void on_interrupt();

#endif /* SERVER_HELPER_H */
