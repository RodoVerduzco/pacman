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