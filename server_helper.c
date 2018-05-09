/*
 *    The program that handles the User Interace of the pacman game.
 *    Written by Ludovic Cyril Michel, Rodolfo Verduzco and Cynthia Castillo.
 */

#include "server_helper.h"

int interrupted = 0;

void on_interrupt(int signal) {
  interrupted = 1;
  printf("\nTHE GAME WAS INTERRUPTED\n");
}

int get_interrupt() { return interrupted; }

/*
  This function handles the pacman game
*/
void serve(int client_fd, int player_num, int player_id,
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

    if (interrupted) {
      break;
    }
  }

  stringify_game_state(game_state, player_num, data);
  send_response(client_fd, GAMEOVER, data);

  fflush(stdout);
  free(data);
}

/*
  Handle the initial request of the client to prepare the game
*/
void handle_init_request(int client_fd, int player_num, int player_id,
                         game_state_t *game_state, char *data) {
  int type;
  int player_count;

  // get initial request
  get_request(client_fd, &type, data);

  // save the player's name
  strcpy(game_state->player_data[player_id].name, data);

  // send the number of players and the player id
  bzero(data, BUFFER_SIZE);
  sprintf(data, "%d %d", player_num, player_id);

  send_response(client_fd, WAIT, data);

  // get initial request
  get_request(client_fd, &type, data);

  // New player added
  pthread_mutex_lock(&game_state->player_count_lock);
  game_state->player_count++;
  pthread_mutex_unlock(&game_state->player_count_lock);

  while (1) {
    pthread_mutex_lock(&game_state->player_count_lock);
    player_count = game_state->player_count;
    pthread_mutex_unlock(&game_state->player_count_lock);

    // start the game when we reach an adequate number of players
    if (player_count == player_num) {
      break;
    }
  }
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

  // get the pacman id
  pthread_mutex_lock(&game_state->pacman_id_lock);
  pacman_id = game_state->pacman_id;
  pthread_mutex_unlock(&game_state->pacman_id_lock);

  // send game state at beginning of round
  stringify_game_state(game_state, player_num, data);
  send_response(client_fd, CHANGE, data);

  fflush(stdout);

  while (1) {
    if (await_request(client_fd)) {
      if (get_request(client_fd, &type, data) == -1) {
        // handle game error
        printf("error communicating with player %d, terminating corresponding "
               "thread",
               player_id);
        break;

      } else {
        // The player moved, need to update the state
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
            if (player_id == pacman_id) {
              game_state->player_data[player_id].score++;
            }
          }
          pthread_mutex_unlock(&game_state->player_data_lock);
          pthread_mutex_unlock(&game_state->pacman_id_lock);
        }

        // get the pacman id
        pthread_mutex_lock(&game_state->pacman_id_lock);
        current_pacman_id = game_state->pacman_id;
        pthread_mutex_unlock(&game_state->pacman_id_lock);

        // if the round is over
        if (current_pacman_id != pacman_id) {
          // put back the players in each corner of the map
          reset_coordinates(game_state, player_num);

          // send a wait response and get an acknowledgement
          send_response(client_fd, WAIT, NULL);
          get_request(client_fd, &type, data);

          // give time to the clients to show the leaderboards
          if (!interrupted) {
            sleep(10);
          }

          break;
        }
        // Client sent OK, must send game state
        else {
          stringify_game_state(game_state, player_num, data);

          // send game state
          send_response(client_fd, CHANGE, data);
        }
      }
    }

    // Server INTERRUPTED
    if (interrupted) {
      send_response(client_fd, ERROR, NULL);
      break;
    }
  }
}
