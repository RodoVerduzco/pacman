#ifndef SERVER_HELPER_H
#define SERVER_HELPER_H

#include "game.h"
#include "server_com.h"

void play_pacman(int client_fd, int player_id, game_state_t *game_state);
void handle_init_request(int client_fd, int player_id, game_state_t *game_state,
                         char *data);
void handle_game_requests(int client_fd, int player_id,
                          game_state_t *game_state, char *data);

#endif
