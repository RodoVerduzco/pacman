/*
*
*   Program :   ClientUI.c
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

#include "ClientUI.h"


/* Empty function to avoid warnings*/
void gameInit()
{
    int startx, starty;
	int ch;
	map = getMap();

	// Start curses mode
	if ( (mainwin = initscr()) == NULL ) {
        perror("error initialising ncurses");
        exit(EXIT_FAILURE);
    }
	keypad(stdscr, TRUE); // Get keyboard input
	noecho();			// Don't echo() while we do getch

	starty = (LINES - MAP_ROWS) / 2;	// Calculating for a center placement
	startx = (COLS - MAP_COLS) / 2;	// of the window
	
	drawMap(map, starty, startx);
	pacman = initPlayer(PACMAN, startx, starty);
	drawPlayer(pacman);
	ghost1 = initPlayer(GHOST_1, startx, starty);
	drawPlayer(ghost1);
	ghost2 = initPlayer(GHOST_2, startx, starty);
	drawPlayer(ghost2);
	ghost3 = initPlayer(GHOST_3, startx, starty);
	drawPlayer(ghost3);
    
    getch();			// Wait for user input
	endwin();			// End curses mode
	
	free(pacman);
	free(ghost1);
	free(ghost2);
	free(ghost3);
    return EXIT_SUCCESS;
}
