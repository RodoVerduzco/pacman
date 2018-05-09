/*
 *   Date: 06/05/18
 *   Program :   map.h
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
#include <stdlib.h>

#include "types.h"

/* **** Macros***  */
#define MAP_COLS 54
#define MAP_ROWS 21
#define MAP_FILE "map.txt"

/* MAP */
#define WALL '#'
#define DOT '.'

#define PLAYER_0_X 3
#define PLAYER_0_Y 1

#define PLAYER_1_X 49
#define PLAYER_1_Y 1

#define PLAYER_2_X 3
#define PLAYER_2_Y 19

#define PLAYER_3_X 49
#define PLAYER_3_Y 19

/*  Function prototypes  */
char *get_map();
int init_x(int player_id);
int init_y(int player_id);
void init_gui();
void draw_map();
void draw_players(game_state_t *game_state, int player_num);

#endif
