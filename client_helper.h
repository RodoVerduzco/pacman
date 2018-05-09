#ifndef CLIENT_HELPER_H
#define CLIENT_HELPER_H

#include "client_com.h"
#include "game.h"
#include "map.h"
#include <curses.h>

void play(int server_fd);
void *handle_interactions(void *arg);
void *handle_gui(void *arg);
void get_keys_pressed(const int player_id, game_state_t *game_state, char *data,
                      int *type);
void print_leaderboard(game_state_t *game_state, int player_num);

#endif
