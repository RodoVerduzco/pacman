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

/*  Macros  */
#define MAP_COLS 54
#define MAP_ROWS 21

/*  Function prototypes  */
char* getMap();
void drawMap(char* map,int starty, int startx);

/* Functions implementation */
char* getMap()
{
    char* mapString = (char *)malloc(sizeof(char) * MAP_ROWS*MAP_COLS);

    mapString ="#####################################################\
##x.....................x###x.....................x##\
##x.x#####x.x#########x.x###x.x#########x.x#####x.x##\
##x.x#####x.x#########x.x###x.x#########x.x#####x.x##\
##x...............................................x##\
##x.x#####x.x###x.x###############x.x###x.x#####x.x##\
##x.x#####x.x###x.......x###x.......x###x.x#####x.x##\
##x.........x#########x.x###x.x#########x.........x##\
####x.x###x.x#########x.x###x.x#########x.x####x.x###\
####x.x###x.x###x...................x###x.x####x.x###\
####x.x###x.x###x.x######   ######x.x###x.x####x.x###\
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

void drawMap(char* map,int starty, int startx)
{	
	int i, j;
	int tempy = starty;
	int tempx = startx;
	j = MAP_COLS;
	
    for (i = 0; i < (MAP_ROWS*MAP_COLS)-21; i++)
    {
    	move(tempy, tempx);
		addch(map[i]);
    	if ((i+1) == j-1)
    	{
    		tempx = startx;
    		tempy += 1;
    		j += (MAP_COLS)-1;
    	}
    	else
    		tempx += 1;
    }

	refresh();
}



#endif  /* MAP_H  */