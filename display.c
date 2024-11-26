#include "display.h"
#include <string.h>


#define N 5
#define M 4 

static int MAX_X, MAX_Y;

static char num[][N][M] = {{{1,1,1,1},{1,0,0,1},{1,0,0,1},{1,0,0,1},{1,1,1,1}},
                           {{0,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,0,1}},
                           {{1,1,1,1},{0,0,0,1},{1,1,1,1},{1,0,0,0},{1,1,1,1}},
                           {{1,1,1,1},{0,0,0,1},{1,1,1,1},{0,0,0,1},{1,1,1,1}},
                           {{1,0,0,1},{1,0,0,1},{1,1,1,1},{0,0,0,1},{0,0,0,1}},
                           {{1,1,1,1},{1,0,0,0},{1,1,1,1},{0,0,0,1},{1,1,1,1}},
                           {{1,1,1,1},{1,0,0,0},{1,1,1,1},{1,0,0,1},{1,1,1,1}},
                           {{1,1,1,1},{1,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,0,1}},
                           {{1,1,1,1},{1,0,0,1},{1,1,1,1},{1,0,0,1},{1,1,1,1}},
                           {{1,1,1,1},{1,0,0,1},{1,1,1,1},{0,0,0,1},{1,1,1,1}}
                          };
 

/* Draw  a number in [0,9] */
static void display_num(int x, int y, char num[N][M]);

/* Draw a score */
static void display_score(unsigned int score1, unsigned int score2);

/* Dram the  net in the middle of the table */
static void display_net(void);

/* Draw a paddle */
static void display_paddle(struct position p, int len);

/* Draw the ball */
static void display_ball(struct position p);

void display_open(int *height, int *width) {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE); // make keys work
  curs_set(0);          // hide cursor
  timeout(TIMEOUT);     // Sets the waiting time for getch()

	getmaxyx(stdscr,MAX_Y,MAX_X);
	*height = MAX_Y-1;
	*width  = MAX_X-1; 

	start_color();
	init_pair(1,COLOR_WHITE,COLOR_BLACK); 
}

void display_start(void) {
	short int n=10, m=46;
	char *help1 = "Player 1 keys: 'a' and 'z'";
	char *help2 = "Player 2 keys: up and down arrows";
	char *help3 = "Push ANY key to start, 'p' for pause and ESC to quit";

	mvprintw((MAX_Y-n)/2+0,(MAX_X-m)/2," ________  ________  ________   ________                       \n");
 	mvprintw((MAX_Y-n)/2+1,(MAX_X-m)/2,"|\\   __  \\|\\   __  \\|\\   ___  \\|\\   ____\\              \n");    
  mvprintw((MAX_Y-n)/2+2,(MAX_X-m)/2,"\\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\\\ \\  \\ \\  \\___|      \n");
  mvprintw((MAX_Y-n)/2+3,(MAX_X-m)/2," \\ \\   ____\\ \\  \\\\\\  \\ \\  \\\\ \\  \\ \\  \\  ___     \n");
  mvprintw((MAX_Y-n)/2+4,(MAX_X-m)/2,"  \\ \\  \\___|\\ \\  \\\\\\  \\ \\  \\\\ \\  \\ \\  \\|\\  \\ \n");
  mvprintw((MAX_Y-n)/2+5,(MAX_X-m)/2,"   \\ \\__\\    \\ \\_______\\ \\__\\\\ \\__\\ \\_______\\     \n");
  mvprintw((MAX_Y-n)/2+6,(MAX_X-m)/2,"    \\|__|     \\|_______|\\|__| \\|__|\\|_______|             \n");
  mvprintw((MAX_Y-n)/2+7,(MAX_X-m)/2,"                                                               \n");
	mvprintw((MAX_Y-n)/2+8,(MAX_X-m)/2,"                                                               \n");
  mvprintw((MAX_Y-n)/2+9,(MAX_X-m)/2,help1);
	mvprintw((MAX_Y-n)/2+10,(MAX_X-m)/2,help2);
	mvprintw((MAX_Y-n)/2+11,(MAX_X-m)/2,help3);
	timeout(-1);  
	getch();
	timeout(TIMEOUT); 
}


static void display_net(void) {
	int i;

	attron(COLOR_PAIR(1));
  attron(A_REVERSE);
	for(i = 0; i < MAX_Y; i += 2)
		mvaddch(i,MAX_X/2,' ');
	attroff(A_REVERSE);
  attroff(COLOR_PAIR(1));
}

static void display_paddle(struct position p, int len) {
	int i;

	attron(COLOR_PAIR(1));
	attron(A_REVERSE);
	for(i = 0; i < len; i++) 
		mvaddch(p.y+i,p.x,' ');
	attroff(A_REVERSE);
	attroff(COLOR_PAIR(1));
}

static void display_ball(struct position p) {
	attron(COLOR_PAIR(1));
  attron(A_REVERSE);
	mvaddch(p.y,p.x,' ');
	mvaddch(p.y,p.x+1,' ');
	attroff(A_REVERSE);
  attroff(COLOR_PAIR(1));
}

void display_table(void) {
	erase();
	display_net();
	display_score(get_pad1_score(),get_pad2_score());
	display_ball(get_ball_pos());
	display_paddle(get_pad1_pos(),get_pad_len());
	display_paddle(get_pad2_pos(),get_pad_len());
}

void display_close() {
  endwin();
}

void display_pause() {
	while(getch()!='p');
}

static void display_num(int x, int y, char num[N][M]) {
	int i,j;
	attron(COLOR_PAIR(1));
	attron(A_REVERSE);
	for(i=0; i < N; i++)
		for(j=0; j < M; j++)
			if(num[i][j]) mvaddch(x+i,y+j,' ');
	attroff(A_REVERSE);
	attroff(COLOR_PAIR(1));
	
}

static void display_score(unsigned int score1, unsigned int score2) {
	display_num(2,MAX_X/2-7,num[score1]);
	display_num(2,MAX_X/2+4,num[score2]);
}

void display_win(int player) {
	char *str[] = {"Player 1 wins!!","Player 2 wins!!"};
	
	attron(A_BLINK);
	mvprintw(MAX_Y/2,(MAX_X-strlen(str[1]))/2,str[player-1]);
	attroff(A_BLINK);	
}

void display_quit(void) {
	char *str = "Press ENTER to quit";

	mvprintw(MAX_Y-2,(MAX_X-strlen(str))/2,str);
	timeout(-1);
	while(getch()!='\n');
}

int display_read(void) {
	return getch();
}
