#include "game.h"
#include <stdio.h>

//controlli
#define LEFT -1
#define RIGHT 1
#define UP -1
#define DOWN 1


#define SHIFT_COL(b,p,n)  ((p).x + (n) == (b).x )
#define SAME_ROW(b,p,len) (((b).y >= (p).y ) && ((b).y <= (p).y + len))
#define TOP_ROW(b,p) ((b).y == (p).y-1)
#define BOT_ROW(b,p,len)  ((b).y == (p).y+len)

//palla tocco pad1 in bordo
#define PAD1_TOP_BORDER_TOUCH(G) ((SHIFT_COL(G.b,G.p1,-1) || SHIFT_COL(G.b,G.p1,0) || SHIFT_COL(G.b,G.p1,1)) && TOP_ROW(G.b, G.p1))
#define PAD1_BOT_BORDER_TOUCH(G) ((SHIFT_COL(G.b,G.p1,-1) || SHIFT_COL(G.b,G.p1,0) || SHIFT_COL(G.b,G.p1,1)) && BOT_ROW(G.b, G.p1, G.pad_len))

//palla tocco pad2 in bordo
#define PAD2_TOP_BORDER_TOUCH(G) ((SHIFT_COL(G.b,G.p2,0) || SHIFT_COL(G.b,G.p2,-1) || SHIFT_COL(G.b,G.p2,-2)) && TOP_ROW(G.b, G.p2))
#define PAD2_BOT_BORDER_TOUCH(G) ((SHIFT_COL(G.b,G.p2,0) || SHIFT_COL(G.b,G.p2,-1) || SHIFT_COL(G.b,G.p2,-2)) && BOT_ROW(G.b, G.p2, G.pad_len))

//palla tocco pieno pad1 o pad2
#define PAD1_FULL_TOUCH(G) (SHIFT_COL(G.b,G.p1,1) && SAME_ROW(G.b,G.p1,G.pad_len))
#define PAD2_FULL_TOUCH(G) (SHIFT_COL(G.b,G.p2,-2) && SAME_ROW(G.b,G.p2,G.pad_len))

//tocco in generale pad1 o pad2
#define PAD1_TOUCH(G) (PAD1_TOP_BORDER_TOUCH(G) || PAD1_BOT_BORDER_TOUCH(G) || PAD1_FULL_TOUCH(G))
#define PAD2_TOUCH(G) (PAD2_TOP_BORDER_TOUCH(G) || PAD2_BOT_BORDER_TOUCH(G) || PAD2_FULL_TOUCH(G))

//pad1 o pad2 tocco bordo
#define PAD_TOP_BORDER_TOUCH(G) (PAD1_TOP_BORDER_TOUCH(G) || PAD2_TOP_BORDER_TOUCH(G))
#define PAD_BOT_BORDER_TOUCH(G) (PAD1_BOT_BORDER_TOUCH(G) || PAD2_BOT_BORDER_TOUCH(G)) 

//tocco palla in bordo della tavola
#define BALL_TOP_BORDER_TOUCH(G) ((G).b.y == 0)
#define BALL_BOT_BORDER_TOUCH(G) ((G).b.y == (G).height)
#define BALL_LEFT_BORDER_TOUCH(G) ((G).b.x == 0)
#define BALL_RIGHT_BORDER_TOUCH(G) ((G).b.x == (G).width-1)
#define BALL_BORDER_TOUCH(G) (BALL_LEFT_BORDER_TOUCH(G) || BALL_RIGHT_BORDER_TOUCH(G))



struct paddle {
	int x;
	int y;
};

struct ball {
	int x;
	int y;
	int v; // vertical direction
	int h; // horizontal direction
};

struct game {
	unsigned int score1;
	unsigned int score2;
	int pad_len;
	int width;
	int height;
	int starting_ball_x;
	int starting_ball_y;
	struct ball b;
	struct paddle p1;
	struct paddle p2;
};

static struct game G;


void setup_game(int height, int width, struct position ball_pos, struct position ball_dir, struct position pad1_pos, struct position pad2_pos, int pad_len) {

	//set della posizione iniziale della palla
	G.starting_ball_x = ball_pos.x;
	G.starting_ball_y = ball_pos.y;

	//set iniziale del punterggio a 0
	G.score1 = G.score2 = 0;

	//set iniziale della grandezza della tavola e lunghezza della paddle
	G.width = width;
	G.height = height;
	G.pad_len = pad_len;

	//set della posizione di pad1
	G.p1.x = pad1_pos.x;
	G.p1.y = pad1_pos.y;

	//set della posizione di pad2
	G.p2.x = pad2_pos.x;
	G.p2.y = pad2_pos.y;

	//set della posizione di ball
	G.b.x = ball_pos.x;
	G.b.y = ball_pos.y;

	//set della direzione di ball
	G.b.h = ball_dir.x;
	G.b.v = ball_dir.y;
}



/* Moves pad1 one position up. */
void move_pad1_up(void) {

	if (G.p1.y > 0 && !(PAD1_TOP_BORDER_TOUCH(G))) G.p1.y--;

}

/* Moves pad2 one position up */
void move_pad2_up(void) {
	if (G.p2.y > 0 && !(PAD2_TOP_BORDER_TOUCH(G))) G.p2.y--;
}

/* Moves pad1 one position down. */
void move_pad1_down(void) {

	if ((G.p1.y + G.pad_len) <= G.height && !(PAD1_BOT_BORDER_TOUCH(G))) G.p1.y++;

}

/* Moves pad2 one position down. */
void move_pad2_down(void) {

	if ((G.p2.y + G.pad_len) <= G.height && !(PAD2_BOT_BORDER_TOUCH(G))) G.p2.y++;

}

/* Moves the ball in the current direction */
void move_ball(void) {


	//gestisco il caso in cui ball tocca il bordo sinistro o destro della tavola
	if (BALL_BORDER_TOUCH(G)) {
		if (BALL_LEFT_BORDER_TOUCH(G)) {
		
			G.score2++;
			G.b.x = G.starting_ball_x;
			G.b.y = G.starting_ball_y;
		
		} 
		else {
			G.score1++;
			G.b.x = G.starting_ball_x;
			G.b.y = G.starting_ball_y;
		}
	}

	else {
		// PAD1 TOUCH
		if (PAD1_TOUCH(G)) {
			G.b.h = RIGHT;
		}
		// PAD2 TOUCH
		if (PAD2_TOUCH(G)) {
			G.b.h = LEFT;
		}

		// PAD BORDER TOUCH
		if (PAD_BOT_BORDER_TOUCH(G)) {
			G.b.v = DOWN;
		}

		else if (PAD_TOP_BORDER_TOUCH(G)) {
			G.b.v = UP;
		}

		// BALL TOP BORDER TOUCH
		if (BALL_TOP_BORDER_TOUCH(G)) {
			G.b.v = DOWN;
		}
		// BALL BOTTOM BORDER TOUCH
		if (BALL_BOT_BORDER_TOUCH(G)) {
			G.b.v = UP;
		}

		// UPDATE POSITION
		G.b.x += G.b.h;
		G.b.y += G.b.v;
	}

}

/* Returns ball current position */
struct position get_ball_pos(void) {
	struct position p;
	p.x = G.b.x;
	p.y = G.b.y;
	return p;
}

/* Returns pad1 current position */
struct position get_pad1_pos(void) {
	struct position p;
	p.x = G.p1.x;
	p.y = G.p1.y;
	return p;
}

/* Returns pad2 current position */
struct position get_pad2_pos(void) {
	struct position p;
	p.x = G.p2.x;
	p.y = G.p2.y;
	return p;
}

/* Returns the pad length */
unsigned int get_pad_len(void) {
	return G.pad_len;
}

/* Returns pad1 current score */
unsigned int get_pad1_score(void) {
	return G.score1;
}

/* Returns pad2 current score */
unsigned int get_pad2_score(void) {
	return G.score2;
}