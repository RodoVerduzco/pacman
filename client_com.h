#ifndef CLIENT_COM_H
#define CLIENT_COM_H
#define BUFFER_SIZE 1024
#define TIMEOUT 15000

#include <arpa/inet.h>
#include <string.h>
#include <sys/poll.h>

#include "error.h"
#include "types.h"

typedef enum { INIT, MOVE, ACK } request_t;
typedef enum { WAIT, CHANGE, GAMEOVER, ERROR } response_t;

int get_response(int server_fd, int *type, char *data);
int send_request(int server_fd, int type, char *data);
int await_response(int server_fd);
void parse_game_state(game_state_t *game_state, int player_num, char *data);
void stringify_change_request(char *data, int x, int y);

#endif
