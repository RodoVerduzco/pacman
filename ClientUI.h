/*
*
*   Program :   ClientUI.h
*
*   Authors  :  Cynthia Berenice Castillo Millán
 *               A01374530
*
*               Ludovic
*               A0
*
*               Jose Rodolfo Verduzco Torres
*               A01366134
*
*   Purpose :
*              This program handles the User Interface of the client.
*
*   Usage  :
*
*
*   Error handling:
*               On any unrecoverable error, the program shows an error
*               message, and exits.
*
*/

#ifndef CLIENTUI_H
#define CLIENTUI_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <curses.h>

#include "Map.h"
#include "error.h"

WINDOW* mainwin;
char* map;
PLAYER* pacman;
PLAYER* ghost1;
PLAYER* ghost2;
PLAYER* ghost3;

void gameInit();

#endif /* CLIENTUI_H */
