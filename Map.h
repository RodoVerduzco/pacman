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
#define MAP_COLS 53
#define MAP_ROWS 21
#define WALL '#'
#define WALL_X 'x'
#define DOOR '_'
#define DOT '.'
#define SPACE ' '
#define GHOST 'M'
#define PACMAN 'O'

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

void drawMap(char* map,int starty, int startx)
{   
    int i, j;
    int tempy = starty;
    int tempx = startx;
    j = MAP_COLS;
    
    if (!has_colors())
    {   endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_BLACK); //color for 'x'
    init_pair(2, COLOR_BLUE, COLOR_BLACK); // color for '#'
    init_pair(3, COLOR_BLACK, COLOR_BLACK); // background color for ' '
    init_pair(4, COLOR_WHITE, COLOR_BLACK); // color for '.'
    
    for (i = 0; i < (MAP_ROWS*MAP_COLS)-1; i++)
    {
        
        move(tempy, tempx);
        switch(map[i])
        {
           case WALL_X:
                attron(COLOR_PAIR(1));
                addch(map[i]);
                attroff(COLOR_PAIR(1));
                break;
                
            case WALL:
                attron(COLOR_PAIR(2));
                addch(map[i]);
                attroff(COLOR_PAIR(2));
                break;
                
           case DOOR:
                attron(COLOR_PAIR(2));
                addch(map[i]);
                attroff(COLOR_PAIR(2));
                break; 
               
           case SPACE:
                attron(COLOR_PAIR(3));
                addch(map[i]);
                attroff(COLOR_PAIR(3));
                break;
           
           default:
                attron(COLOR_PAIR(4));
                addch(map[i]);
                attroff(COLOR_PAIR(4));
                break;
        }
        
        if ((i+1) == j-1)
        {
            tempx = startx;
            tempy += 1;
            j += (MAP_COLS);
        }
        else
            tempx += 1;
    }

    refresh();
}



#endif  /* MAP_H  */