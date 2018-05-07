/*
 *    The program that handles the User Interace of the pacman game.
 *    Written by Ludovic Cyril Michel, Rodolfo Verduzco and Cynthia Castillo.
 */

#include "server_helper.h"

/*
  This function handles the pacman game
*/
void play_pacman(int client_fd, int player_id, game_state_t *game_state) {
  char *data = (char *)malloc(sizeof(char) * BUFFER_SIZE);
  int pacman_id;

  //Client connection
  handle_init_request(client_fd, player_id, game_state, data);

  while (1) {
    // State updates
    handle_game_requests(client_fd, player_id, game_state, data);

    pthread_mutex_lock(&game_state->pacman_id_lock);
    pacman_id = game_state->pacman_id;
    pthread_mutex_unlock(&game_state->pacman_id_lock);

    if (pacman_id >= PLAYER_NUM) {
      break;
    }
  }

  stringify_game_state(game_state, data);

  if (send_response(client_fd, GAMEOVER, data)) {
    // handle game error
    printf("Error: send WAIT");
  }

  fflush(stdout);
  free(data);
}

/*
  Handle the initial request of the client to prepare the game
*/
void handle_init_request(int client_fd, int player_id, game_state_t *game_state,
                         char *data) {
  int type;
  int player_count;

  // get initial request
  if (get_request(client_fd, &type, data) == -1) {
    // handle game error
    printf("Error: receive INIT");
  }

  if (type != INIT) {
    // handle game error
    printf("Error: not INIT request");
  }

  fflush(stdout);

  strcpy(game_state->player_data[player_id].name, data);

  bzero(data, BUFFER_SIZE);
  sprintf(data, "%d", player_id);

  if (send_response(client_fd, WAIT, data)) {
    // handle game error
    printf("Error: send WAIT");
  }

  fflush(stdout);

  // get initial request
  if (get_request(client_fd, &type, data) == -1) {
    // handle game error
    printf("Error: receive OK");
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

    if (player_count == PLAYER_NUM) {
      break;
    }
  }

  printf("\nend handle_init_request");

  fflush(stdout);
}

/*
  Handle the normal game requests
*/
void handle_game_requests(int client_fd, int player_id,
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

  stringify_game_state(game_state, data);

  if (send_response(client_fd, CHANGE, data)) {
    // handle game error
    printf("Error: send WAIT");
  }

  fflush(stdout);

  while (1) {
    if (await_request(client_fd)) {
      if (get_request(client_fd, &type, data) == -1) {
        // handle game error
        printf("Error: receive OK");
      }

      printf("got %d", type);

      fflush(stdout);

      // The player moved, need tho update the state
      if (type == MOVE) {
        parse_change_request(data, &x, &y);

        pthread_mutex_lock(&game_state->pacman_id_lock);
        pthread_mutex_lock(&game_state->player_data_lock);
        validation_result = check_coordinates(game_state, player_id, x, y);
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
        reset_coordinates(game_state);

        if (send_response(client_fd, WAIT, NULL)) {
          // handle game error
          printf("Error: send WAIT");
        }

        fflush(stdout);

        break;
      }
      // Client sent OK, must send game state 
      else {
        stringify_game_state(game_state, data);

        if (send_response(client_fd, CHANGE, data)) {
          // handle game error
          printf("Error: send WAIT");
        }

        fflush(stdout);
      }

    } else {
      // client timed out
    }
  }
}
