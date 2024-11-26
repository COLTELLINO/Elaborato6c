#ifndef DISPLAY_H
#define DISPLAY_H

#include <curses.h>
#include "game.h"

#define QUIT     0x1B
#define P1_UP    'a'
#define P1_DOWN  'z'
#define P2_UP    KEY_UP
#define P2_DOWN  KEY_DOWN
#define PAUSE    'p'

#define TIMEOUT  3 // 3ms

/* Opens the graphical display. */
void display_open(int *height, int *width);

/* Close the graphical display. */
void display_close(void);

/* Draw the starting display. */
void display_start(void);

/* Quit the game */
void display_quit(void);

/* Draw the current table. */
void display_table(void);

/* Pause the game */
void display_pause(void);

/* Draw the winning infos */
void display_win(int player);

/* Read from keyboard */
int display_read(void);



#endif
