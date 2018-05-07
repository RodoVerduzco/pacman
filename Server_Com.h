#ifndef SERVER_COM_H
#define SERVER_COM_H
#define BUFFER_SIZE 1024
#define TIMEOUT 15000

#include <arpa/inet.h>
#include <string.h>
#include <sys/poll.h>

#include "error.h"
#include "server_types.h"

typedef enum { INIT, MOVE, OK } request_t;
typedef enum { WAIT, CHANGE, GAMEOVER } response_t;

int get_request(int client_fd, int *type, char *data);
int send_response(int client_fd, int type, char *data);
int await_request(int client_fd);
void stringify_game_state(game_state_t *game_state, char *data);
void parse_change_request(char *data, int *x, int *y);

#endif
