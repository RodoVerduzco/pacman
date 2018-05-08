/*
 *    The program that handles the User Interace of the pacman game.
 *    Written by Ludovic Cyril Michel, Rodolfo Verduzco and Cynthia Castillo.
 */

#include "server_helper.h"

pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;
int running_threads = 0;
int interrupted = 0;

void onInterrupt(int signal)
{
  interrupted = 1;
  printf("\nTHE GAME WAS INTERRUPTED\n");
}

/*
  This function handles the pacman game
*/
void play_pacman(int client_fd, int player_num, int player_id,
                 game_state_t *game_state) {
  char *data = (char *)malloc(sizeof(char) * BUFFER_SIZE);
  int pacman_id;

  handle_init_request(client_fd, player_num, player_id, game_state, data);

  while (1) {
    handle_game_requests(client_fd, player_num, player_id, game_state, data);

    pthread_mutex_lock(&game_state->pacman_id_lock);
    pacman_id = game_state->pacman_id;
    pthread_mutex_unlock(&game_state->pacman_id_lock);

    if (pacman_id >= player_num) {
      break;
    }
  }

  stringify_game_state(game_state, player_num, data);

  if (send_response(client_fd, GAMEOVER, data)) {
    // handle game error
    printf("Error: send WAIT\n");
  }

  fflush(stdout);
  free(data);
}

/*
  Increment the running thread counter
*/
void addRunningThread()
{
  pthread_mutex_lock(&running_mutex);
  running_threads++;
  pthread_mutex_unlock(&running_mutex);
}

int getRunningThreads()
{
  int threads;
  pthread_mutex_lock(&running_mutex);
  threads = running_threads;
  pthread_mutex_unlock(&running_mutex);
  return threads;
}

int getInterrupted()
{
  return interrupted;
}

void freeThread(game_state_t *game_state)
{
  pthread_mutex_lock(&running_mutex);
    running_threads--;
    if(running_threads<0)
      running_threads = 0;
  pthread_mutex_unlock(&running_mutex);
  printf("NUM THREADS %d %d\n", running_threads, getInterrupted());
  if(running_threads ==0 && getInterrupted()==1)
    exit(EXIT_SUCCESS);
}

/*
  Handle the initial request of the client to prepare the game
*/
void handle_init_request(int client_fd, int player_num, int player_id,
                         game_state_t *game_state, char *data) {
  int type;
  int player_count;

  // get initial request
  if (get_request(client_fd, &type, data) == -1) {
    // handle game error
    printf("Error: receive INIT\n");
  }

  if (type != INIT) {
    // handle game error
    printf("Error: not INIT request\n");
  }

  fflush(stdout);

  strcpy(game_state->player_data[player_id].name, data);

  bzero(data, BUFFER_SIZE);
  sprintf(data, "%d", player_id);

  if (send_response(client_fd, WAIT, data)) {
    // handle game error
    printf("Error: send WAIT\n");
  }

  fflush(stdout);

  // get initial request
  if (get_request(client_fd, &type, data) == -1) {
    // handle game error
    printf("Error: receive OK\n");
  }

  if (type != OK) {
    // handle game error
  }

  // New player added
  pthread_mutex_lock(&game_state->player_count_lock);
  game_state->player_count++;
  pthread_mutex_unlock(&game_state->player_count_lock);

  while (1) {
    pthread_mutex_lock(&game_state->player_count_lock);
    player_count = game_state->player_count;
    pthread_mutex_unlock(&game_state->player_count_lock);

    if (player_count == player_num) {
      break;
    }
  }

  printf("\nend handle_init_request");

  fflush(stdout);
}

/*
  Handle the normal game requests
*/
void handle_game_requests(int client_fd, int player_num, int player_id,
                          game_state_t *game_state, char *data) {
  int pacman_id;
  int current_pacman_id;
  int type;
  int x;
  int y;
  int validation_result;

  printf("\nbegin handle_game_requests");

  fflush(stdout);

  pthread_mutex_lock(&game_state->pacman_id_lock);
  pacman_id = game_state->pacman_id;
  pthread_mutex_unlock(&game_state->pacman_id_lock);

  // send first response

  stringify_game_state(game_state, player_num, data);

  if (send_response(client_fd, CHANGE, data)) {
    // handle game error
    printf("Error: send WAIT\n");
  }

  fflush(stdout);

  while (1) {
    if (await_request(client_fd)) {
      if (get_request(client_fd, &type, data) == -1) {
        // handle game error
        printf("error communicating with player %d", player_id);
      } else {
        printf(" got %d\n", type);

        fflush(stdout);


        // The player moved, need tho update the state
        if (type == MOVE) {
          parse_change_request(data, &x, &y);

          pthread_mutex_lock(&game_state->pacman_id_lock);
          pthread_mutex_lock(&game_state->player_data_lock);
          validation_result =
              check_coordinates(game_state, player_num, player_id, x, y);
          if (validation_result == 2) {
            game_state->pacman_id++;
          } else if (validation_result == 1 &&
                     game_state->pacman_id == pacman_id) {
            game_state->player_data[player_id].x = x;
            game_state->player_data[player_id].y = y;
            game_state->player_data[player_id].score++;
          }
          pthread_mutex_unlock(&game_state->player_data_lock);
          pthread_mutex_unlock(&game_state->pacman_id_lock);
        }

        pthread_mutex_lock(&game_state->pacman_id_lock);
        current_pacman_id = game_state->pacman_id;
        pthread_mutex_unlock(&game_state->pacman_id_lock);

        if (current_pacman_id != pacman_id) {
          reset_coordinates(game_state, player_num);

          if (send_response(client_fd, WAIT, NULL)) {
            // handle game error
            printf("Error: send WAIT\n");
          }

          fflush(stdout);

          break;
        }
        // Client sent OK, must send game state
        else {
          stringify_game_state(game_state, player_num, data);

          if (send_response(client_fd, CHANGE, data)) {
            // handle game error
            printf("Error: send WAIT\n");
          }

          fflush(stdout);
        }
      }
    }

    // Server INTERRUPTED
    if(interrupted == 1){
      send_response(client_fd, ERROR, NULL);
      pthread_exit(NULL);
    }
  }
}
