/*
 *    The program that handles the pacman game map.
 *    Written by Ludovic Cyril Michel, Rodolfo Verduzco and Cynthia Castillo.
 */

#include "map.h"

/*
  This function draws the map
*/

void init_gui() {
  initscr(); // ncurses init
  noecho();  // Dont show keys pressed in console
  curs_set(FALSE);

  if (!has_colors()) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }

  start_color();

  init_pair(0, COLOR_YELLOW, COLOR_BLACK);
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_WHITE, COLOR_BLACK);
}

char *get_map() {

  // open the file

  FILE *file = fopen(MAP_FILE, "r");

  // get the file size

  fseek(file, 0, SEEK_END);
  int file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  // assign memory to the text

  char *map = (char *)malloc(file_size * sizeof(char) + 1);

  // read the text

  fread(map, sizeof(char), file_size, file);

  // close the file

  fclose(file);

  return map;
}

void draw_map() {
  char *map = get_map();
  int i, j;
  int tempy = 1;
  int tempx = 1;
  j = MAP_COLS;

  for (i = 0; i < (MAP_ROWS * MAP_COLS) - 1; i++) {
    move(tempy, tempx);
    switch (map[i]) {
    case WALL:
      attron(COLOR_PAIR(4));
      addch(map[i]);
      attroff(COLOR_PAIR(4));
      break;

    default:
      attron(COLOR_PAIR(5));
      addch(map[i]);
      attroff(COLOR_PAIR(5));
      break;
    }

    if (i == j - 2) {
      tempx = 1;
      tempy += 1;
      j += MAP_COLS;
    } else {
      tempx += 1;
    }
  }

  refresh();
}

void draw_players(game_state_t *game_state, int player_num) {
  int x;
  int y;
  int pacman_id;

  pthread_mutex_lock(&game_state->pacman_id_lock);
  pacman_id = game_state->pacman_id;
  pthread_mutex_unlock(&game_state->pacman_id_lock);

  for (int player_id = 0; player_id < player_num; player_id++) {
    pthread_mutex_lock(&game_state->player_data_lock);
    x = game_state->player_data[player_id].x;
    y = game_state->player_data[player_id].y;
    pthread_mutex_unlock(&game_state->player_data_lock);

    move(x, y);
    attron(A_BOLD);
    attron(COLOR_PAIR(player_id));

    if (player_id == pacman_id) {
      addch('O');
    } else {
      addch('M');
    }

    attroff(COLOR_PAIR(player_id));
    attroff(A_BOLD);

    refresh();
  }
}
