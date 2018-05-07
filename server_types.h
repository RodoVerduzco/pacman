#ifndef SERVER_TYPES_H
#define SERVER_TYPES_H

#include <pthread.h>

typedef struct {
  char name[50];
  int score;
  int x;
  int y;
} player_data_t;

typedef struct {
  int player_count;
  int pacman_id;
  player_data_t *player_data;
  pthread_mutex_t player_count_lock;
  pthread_mutex_t pacman_id_lock;
  pthread_mutex_t player_data_lock;
} game_state_t;

typedef struct {
  int client_fd;
  int player_num;
  int player_id;
  game_state_t *game_state;
} thread_data_t;

#endif
