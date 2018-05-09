#ifndef CLIENT_HELPER_H
#define CLIENT_HELPER_H

#include <curses.h>
#include "client_com.h"
#include "game.h"

void play(int server_fd);
void handle_init_request(int server_fd, int player_num, int player_id,
                         game_state_t *game_state, char *data);
void handle_game_requests(int server_fd, int player_num, int player_id,
                          game_state_t *game_state, char *data);
void get_keys_pressed(const int player_id, game_state_t *game_state,
                      char *data, int *type);

#endif
