/*
 *   Date: 06/05/18
 *   Program :   Map.h
 *
 *   Author  :  Cynthia Berenice Castillo Millán
 *               A01374530
 *
 *   Purpose :
 *              Contains all necesary functions to deal with the Map.
 *
 */

#ifndef MAP_H
#define MAP_H

#include <curses.h>

/* **** Macros***  */
#define MAP_COLS 53
#define MAP_ROWS 21

/*Position threshold*/
#define Pacmanx 27
#define Pacmany 14
#define Ghostx 25
#define Ghosty 12

/* Arrow movements */
#define DOWN 1
#define UP 2
#define LEFT 3
#define RIGHT 4

/* MAP */
#define WALL '#'
#define WALL_X 'x'
#define DOOR '_'
#define DOT '.'
#define SPACE ' '

/* Players */
#define PACMAN 1
#define GHOST_1 2
#define GHOST_2 3
#define GHOST_3 4

/*  Structures and typedefs  */
struct PLAYER {
  int number; // 1 - pacman, >1 - Ghost
  int x;
  int y;
};
typedef struct PLAYER PLAYER;

/*  Function prototypes  */
char *getMap();
void drawMap(char *map, int starty, int startx);
PLAYER *initPlayer(int player, int startx, int starty);
void drawPlayer(PLAYER *player);

/* Functions implementation */
char *getMap() {
  char *mapString = (char *)malloc(sizeof(char) * MAP_ROWS * MAP_COLS);

  mapString = "#####################################################\
##x.....................x###x.....................x##\
##x.x#####x.x#########x.x###x.x#########x.x#####x.x##\
##x.x#####x.x#########x.x###x.x#########x.x#####x.x##\
##x...............................................x##\
##x.x#####x.x###x.x###############x.x###x.x#####x.x##\
##x.x#####x.x###x.......x###x.......x###x.x#####x.x##\
##x.........x#########x.x###x.x#########x.........x##\
####x.x###x.x#########x.x###x.x#########x.x####x.x###\
####x.x###x.x###x...................x###x.x####x.x###\
####x.x###x.x###x.x######___######x.x###x.x####x.x###\
####x.x###x.x###x.x###         ###x.x###x.x####x.x###\
##x...............x###         ###x...............x##\
##x.x#####x.x###x.x###############x.x###x.x#####x.x##\
##x.....x#x...............................x#x.....x##\
######x.x#x.x###x.x###############x.x###x.x###x.x####\
##x.........x###x.......x###x.......x###x.........x##\
##x.x#################x.x###x.x#################x.x##\
##x.x#################x.x###x.x#################x.x##\
##x...............................................x##\
#####################################################";

  return mapString;
}

void drawMap(char *map, int starty, int startx) {
  int i, j;
  int tempy = starty;
  int tempx = startx;
  j = MAP_COLS;

  if (!has_colors()) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }

  start_color();
  init_pair(5, COLOR_BLACK,
            COLOR_BLACK); // color for 'x' & background color for ' '
  init_pair(6, COLOR_BLUE, COLOR_BLACK);  // color for '#'
  init_pair(7, COLOR_WHITE, COLOR_BLACK); // color for '.'

  for (i = 0; i < (MAP_ROWS * MAP_COLS) - 1; i++) {

    move(tempy, tempx);
    switch (map[i]) {
    case WALL_X:
      attron(COLOR_PAIR(5));
      addch(map[i]);
      attroff(COLOR_PAIR(5));
      break;

    case WALL:
      attron(COLOR_PAIR(6));
      addch(map[i]);
      attroff(COLOR_PAIR(6));
      break;

    case DOOR:
      attron(COLOR_PAIR(6));
      addch(map[i]);
      attroff(COLOR_PAIR(6));
      break;

    case SPACE:
      attron(COLOR_PAIR(5));
      addch(map[i]);
      attroff(COLOR_PAIR(5));
      break;

    default:
      attron(COLOR_PAIR(7));
      addch(map[i]);
      attroff(COLOR_PAIR(7));
      break;
    }

    if ((i + 1) == j - 1) {
      tempx = startx;
      tempy += 1;
      j += (MAP_COLS);
    } else
      tempx += 1;
  }

  refresh();
}

PLAYER *initPlayer(int player, int startx, int starty) {

  PLAYER *temp;

  if ((temp = malloc(sizeof(PLAYER))) == NULL) {
    perror("couldn't allocate memory for player");
    exit(EXIT_FAILURE);
  }

  switch (player) {
  case PACMAN:
    temp->number = PACMAN;
    temp->x = startx + Pacmanx;
    temp->y = starty + Pacmany;
    break;

  case GHOST_1:
    temp->number = GHOST_1;
    temp->x = startx + Ghostx + 2;
    temp->y = starty + Ghosty - 1;
    break;

  case GHOST_2:
    temp->number = GHOST_2;
    temp->x = startx + Ghostx;
    temp->y = starty + Ghosty;
    break;

  case GHOST_3:
    temp->number = GHOST_3;
    temp->x = startx + Ghostx + 4;
    temp->y = starty + Ghosty;
    break;
  }
  return temp;
}

void drawPlayer(PLAYER *player) {
  start_color();
  init_pair(PACMAN, COLOR_YELLOW, COLOR_BLACK);
  init_pair(GHOST_1, COLOR_CYAN, COLOR_BLACK);
  init_pair(GHOST_2, COLOR_RED, COLOR_BLACK);
  init_pair(GHOST_3, COLOR_GREEN, COLOR_BLACK);

  move(player->y, player->x);
  attron(A_BOLD);
  attron(COLOR_PAIR(player->number));

  if (player->number == PACMAN)
    addch('O');
  else
    addch('M');

  attroff(COLOR_PAIR(player->number));
  attroff(A_BOLD);
  refresh();
}

#endif /* MAP_H  */
