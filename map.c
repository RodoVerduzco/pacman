/*
 *    The program that handles the pacman game map.
 *    Written by Ludovic Cyril Michel, Rodolfo Verduzco and Cynthia Castillo.
 */

#include "map.h"

char *get_map() {

  // open the file

  FILE *file = fopen(MAP_FILE, "r");
  if (file == NULL) {
    printf("seg");
  }

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

char get_map_position(int x, int y, char *map) {
  int i = x * (MAP_COLS - 2) + y;
  return map[i];
}

int init_x(int player_id) {
  switch (player_id) {
  case 0:
    return PLAYER_0_X;
  case 1:
    return PLAYER_1_X;
  case 2:
    return PLAYER_2_X;
  case 3:
    return PLAYER_3_X;
  }
  return 0;
}

int init_y(int player_id) {
  switch (player_id) {
  case 0:
    return PLAYER_0_Y;
  case 1:
    return PLAYER_1_Y;
  case 2:
    return PLAYER_2_Y;
  case 3:
    return PLAYER_3_Y;
  }
  return 0;
}

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

void draw_map() {
  char *map = get_map();
  int x = 0;
  int y = 0;

  for (int i = 0; i < MAP_ROWS - 1; i++) {
    for (int j = 0; j < MAP_COLS - 2; j++) {
      move(i, j);
      switch (get_map_position(i, j, map)) {
      case WALL:
        attron(COLOR_PAIR(4));
        addch(get_map_position(i, j, map));
        attroff(COLOR_PAIR(4));
        break;

      default:
        attron(COLOR_PAIR(5));
        addch(get_map_position(i, j, map));
        attroff(COLOR_PAIR(5));
        break;
      }
    }
  }

  refresh();
}

void patch_map(int x, int y, char *map) {
  char patch = get_map_position(x, y, map);
  move(x, y);
  switch (patch) {
  case WALL:
    attron(COLOR_PAIR(4));
    addch(patch);
    attroff(COLOR_PAIR(4));
    break;

  default:
    attron(COLOR_PAIR(5));
    addch(patch);
    attroff(COLOR_PAIR(5));
    break;
  }
}

void draw_players(game_state_t *game_state, int player_num,
                  prev_positions_t *prev_positions) {
  int x;
  int y;
  int pacman_id;

  char *map = get_map();

  pthread_mutex_lock(&game_state->pacman_id_lock);
  pacman_id = game_state->pacman_id;
  pthread_mutex_unlock(&game_state->pacman_id_lock);

  for (int player_id = 0; player_id < player_num; player_id++) {
    patch_map(prev_positions->x[player_id], prev_positions->y[player_id], map);

    pthread_mutex_lock(&game_state->player_data_lock);
    x = game_state->player_data[player_id].x;
    y = game_state->player_data[player_id].y;
    pthread_mutex_unlock(&game_state->player_data_lock);

    prev_positions->x[player_id] = x;
    prev_positions->y[player_id] = y;

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
