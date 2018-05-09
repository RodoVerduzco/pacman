/*
 *    The Server Communication program helper for a multiplayer Pacman game.
 *    Written by Ludovic Cyril Michel, Rodolfo Verduzco and Cynthia Castillo.
 */

#ifndef SERVER_COM_H
#define SERVER_COM_H
#define BUFFER_SIZE 1024
#define TIMEOUT 15000

#include <arpa/inet.h>
#include <string.h>
#include <sys/poll.h>

#include "error.h"
#include "types.h"

typedef enum { INIT, MOVE, ACK } request_t;
typedef enum { WAIT, CHANGE, GAMEOVER, ERROR } response_t;

int get_request(int client_fd, int *type, char *data);
int send_response(int client_fd, int type, char *data);
int await_request(int client_fd);
void stringify_game_state(game_state_t *game_state, int player_num, char *data);
void parse_change_request(char *data, int *x, int *y);

#endif /* SERVER_COM_H */
