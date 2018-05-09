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
#define MAP_COLS 53
#define MAP_ROWS 21
#define MAP_FILE "map.txt"

/* MAP */
#define WALL '#'
#define DOT '.'

/*  Function prototypes  */
char *get_map();
void init_gui();
void draw_map();
void draw_players(game_state_t *game_state, int player_num);

#endif
