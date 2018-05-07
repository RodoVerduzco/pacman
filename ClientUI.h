/*
 *    The user interface program helper for a multiplayer Pacman game.
 *    Written by Ludovic Cyril Michel, Rodolfo Verduzco and Cynthia Castillo.
 */

#ifndef CLIENTUI_H
#define CLIENTUI_H

// Support Libraries
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <curses.h>

// Custom Libraries
#include "Map.h"
#include "error.h"

WINDOW* mainwin;
char* map;
PLAYER* pacman;
PLAYER* ghost1;
PLAYER* ghost2;
PLAYER* ghost3;

// Function definition
void gameInit();

#endif /* CLIENTUI_H */
