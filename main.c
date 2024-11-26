#include "display.h"
#include "game.h"

#define MIN(x,y) ((x)>(y) ? (y) : (x))
#define DELAY        10
#define PAD_OFFSET    4
#define PAD_LEN_RATIO 5

#define LEFT  -1
#define RIGHT  1
#define UP    -1
#define DOWN   1

/* Starting pad1 position */
static struct position pad1_pos(int height, int width);

/* Starting pad2 position */
static struct position pad2_pos(int height, int width);

/* Starting ball position */
static struct position ball_pos(int height, int width);

/* Starting direction position */
static struct position ball_dir(void);

/* Pad length */
static int pad_len(int height);


int main() {
	int STOP = 0, cont = 0, height, width;
	unsigned int s1, s2; 
	#ifdef REG
	FILE *fp;
	fp=fopen("record.txt","w");
	#endif

	display_open(&height,&width);

	#ifdef REG
	fprintf(fp,"%d %d\n",height,width);
	#endif

	setup_game(height,width,ball_pos(height,width),ball_dir(),
						pad1_pos(height,width),pad2_pos(height,width),pad_len(height));

	display_start();

	do {
		// Add a delay to move_ball() 
		if((cont = (cont + 1) % DELAY) == 0) {
			move_ball();
			display_table();
			#ifdef REG
			fprintf(fp,"M");
      #endif
		}
    switch (display_read()) {
      case P1_DOWN: move_pad1_down(); 
										#ifdef REG
										fprintf(fp,"z");
										#endif
										break;
      case P1_UP:   move_pad1_up();
										#ifdef REG
										fprintf(fp,"a");
										#endif
										break;
      case P2_UP:   move_pad2_up();
										#ifdef REG
										fprintf(fp,"u");
										#endif
										break;
      case P2_DOWN: move_pad2_down();
										#ifdef REG
										fprintf(fp,"d");
										#endif
										break;
      case PAUSE:   display_pause();  break;
      case QUIT:    STOP = 1;         break;
    }
		display_table();
  } while(!STOP && ((s1 = get_pad1_score()) != 9 && (s2 = get_pad2_score()) != 9));

	if(s1 == 9 || s2 == 9)
		display_win(s1 == 9 ? 1 : 2);	

	display_quit();
	display_close();
	#ifdef REG
	fclose(fp);
  #endif
}

/* Starting pad1 position */
static struct position pad1_pos(int height, int width) {
  struct position pos;
  pos.x = PAD_OFFSET;
  pos.y = height/2;
  return pos;
}

/* Starting pad2 position */
static struct position pad2_pos(int height, int width) {
  struct position pos;
  pos.x = width-PAD_OFFSET;
  pos.y = height/2;
  return pos;
}

/* Starting ball position */
static struct position ball_pos(int height, int width) {
  struct position pos;
  pos.x = width/2;
  pos.y = height/2;
  return pos;
}

/* Starting direction position */
static struct position ball_dir(void) {
  struct position pos = {DOWN,RIGHT};
  return pos;
}

/* Pad length */
static int pad_len(int height) {
  return MIN(height/2+1,height/PAD_LEN_RATIO);
}

