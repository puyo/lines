/* Gregory McIntyre 1998 */
/* LINES!!!              */

#include <stdio.h>
#include "allegro.h"


/* supports 4 players */
#define MAX_PLAYERS  4

/* player colours */
#define P1_COLOUR  1
#define P2_COLOUR  4
#define P3_COLOUR  2
#define P4_COLOUR 14

/* player controls */
#define KEY_P1UP     KEY_W
#define KEY_P1DOWN   KEY_S
#define KEY_P1LEFT   KEY_A
#define KEY_P1RIGHT  KEY_D

#define KEY_P2UP     KEY_I
#define KEY_P2DOWN   KEY_K
#define KEY_P2LEFT   KEY_J
#define KEY_P2RIGHT  KEY_L

#define KEY_P3UP     KEY_UP
#define KEY_P3DOWN   KEY_DOWN
#define KEY_P3LEFT   KEY_LEFT
#define KEY_P3RIGHT  KEY_RIGHT

#define KEY_P4UP     KEY_PGDN
#define KEY_P4DOWN   KEY_DEL
#define KEY_P4LEFT   KEY_INSERT
#define KEY_P4RIGHT  KEY_ENTER

/* frame delay in ms */
#define DELAY  10

#define WIDTH   640
#define HEIGHT  480


#define UP     0
#define DOWN   1
#define LEFT   2
#define RIGHT  3


/** CLASS DEFINITIONS **********************************************/

typedef struct Player
{
 char direction;
 char colour;
 char wins;
 char dead;
 int x;
 int y;
 int oldx;
 int oldy;
 char up;
 char down;
 char left;
 char right;
} Player;



/** FUNCTION PROTOTYPES ********************************************/

void handle_keys();
void move();
void redraw();
void crash(char);



/** GLOBAL DATA ****************************************************/

struct Player p[MAX_PLAYERS];
char players;
char won;


/** MAIN FUNCTION **************************************************/

int main(void)
{
 BITMAP *bmp;
 PALETTE pal;
 int i, c;
 char string[80];

 /* init data */
 p[0].colour = P1_COLOUR;
 p[0].up = KEY_P1UP;
 p[0].down = KEY_P1DOWN;
 p[0].left = KEY_P1LEFT;
 p[0].right = KEY_P1RIGHT;

 p[1].colour = P2_COLOUR;
 p[1].up = KEY_P2UP;
 p[1].down = KEY_P2DOWN;
 p[1].left = KEY_P2LEFT;
 p[1].right = KEY_P2RIGHT;

 p[2].colour = P3_COLOUR;
 p[2].up = KEY_P3UP;
 p[2].down = KEY_P3DOWN;
 p[2].left = KEY_P3LEFT;
 p[2].right = KEY_P3RIGHT;

 p[3].colour = P4_COLOUR;
 p[3].up = KEY_P4UP;
 p[3].down = KEY_P4DOWN;
 p[3].left = KEY_P4LEFT;
 p[3].right = KEY_P4RIGHT;

 do
   {
    printf("Lines!!\n-------\n");
    printf("  2: Two Player Game\n");
    printf("  3: Three Player Game\n");
    printf("  4: Four Player Game\n");
    printf("  Q: Quit\n");

    printf("\nChoice: ");
    fflush(stdout);
    c = getc(stdin);
    if (c == 81 || c == 113)
      return 0;
   }
 while (c < 50 || c > 52);

 players = c - 48;

 for (i = 0; i != 4; ++i)
   p[i].wins = 0;

 /* init allegro */
 allegro_init();
 install_timer();
 install_keyboard();

 /* enter graphics mode */
 set_gfx_mode(GFX_AUTODETECT, WIDTH, HEIGHT, 0, 0);

 /* outer game loop */
 do
   {
    for (i = 0; i != 4; ++i)
       p[i].dead = 0;
    for (i = players; i != 4; ++i)
       p[i].dead = 1;
    won = 0;

    p[0].x = SCREEN_W - SCREEN_H*7/8 + 1;
    p[0].y = SCREEN_H/8;
    p[0].direction = RIGHT;
 
    p[1].x = SCREEN_H*7/8 - 1;
    p[1].y = SCREEN_H*7/8;
    p[1].direction = LEFT;
 
    p[2].x = SCREEN_W*7/8;
    p[2].y = SCREEN_H/8 + 1;
    p[2].direction = DOWN;
 
    p[3].x = SCREEN_W/8;
    p[3].y = SCREEN_H*7/8 - 1;
    p[3].direction = UP;
 
    clear(screen);
    rest(200);
 
    /* inner game loop */
    while (!won)
      {
       handle_keys();
       move();
       redraw();
       rest(DELAY);
 
       /* check for wins */
       if (p[0].dead && p[1].dead && p[2].dead && p[3].dead)
         won = -1;
       else if (!p[0].dead && p[1].dead && p[2].dead && p[3].dead)
         won = 1;
       else if (p[0].dead && !p[1].dead && p[2].dead && p[3].dead)
         won = 2;
       else if (p[0].dead && p[1].dead && !p[2].dead && p[3].dead)
         won = 3;
       else if (p[0].dead && p[1].dead && p[2].dead && !p[3].dead)
         won = 4;
 
       if (won > 0)
         ++p[won - 1].wins;
      }
 
    for (i = 0; i != players; ++i)
      {
       sprintf(string, "Player %d: %3d", i + 1, p[i].wins);
       textout_centre(screen, font, string, SCREEN_W >> 1, (SCREEN_H >> 2) + 16*i, p[i].colour);
      }
    textout_centre(screen, font, "Play again? [N]", SCREEN_W >> 1, (SCREEN_H - 20), 15);
    rest(200);
    clear_keybuf();
    c = readkey() >> 8;
   }
 while (c != KEY_N && c != KEY_ESC);

 allegro_exit();
 return 0;
}

END_OF_MAIN();


void handle_keys()
{
 char i;

 /* for taking a screen shot */
 /*
 BITMAP *bmp;
 PALETTE pal;
 if (key[KEY_F12])
   {
    get_palette(pal);
    bmp = create_sub_bitmap(screen, 0, 0, SCREEN_W, SCREEN_H);
    save_bitmap("dump.pcx", bmp, pal);
    destroy_bitmap(bmp);
   }
 */

 for (i = 0; i != players; ++i)
   {
    if (!p[i].dead)
      {
       if (key[p[i].up] && p[i].direction != DOWN)
         p[i].direction = UP;
       else if (key[p[i].down] && p[i].direction != UP)
         p[i].direction = DOWN;
       else if (key[p[i].left] && p[i].direction != RIGHT)
         p[i].direction = LEFT;
       else if (key[p[i].right] && p[i].direction != LEFT)
         p[i].direction = RIGHT;
      }
   }
}



void move()
{
 char i;

 for (i = 0; i != players; ++i)
   {
    if (!p[i].dead && !won)
      {
       p[i].oldx = p[i].x;
       p[i].oldy = p[i].y;
   
       if (p[i].direction == UP)
         p[i].y--;
       else if (p[i].direction == DOWN)
         p[i].y++;
       else if (p[i].direction == LEFT)
         p[i].x--;
       else if (p[i].direction == RIGHT)
         p[i].x++;
   
       /* collision detection */
       if (getpixel(screen, p[i].x, p[i].y))
         crash(i);
      }
   }
}



void redraw()
{
 char i;

 rect(screen, 0, 0, SCREEN_W - 1, SCREEN_H - 1, 15);

 for (i = 0; i != players; ++i)
    if (!p[i].dead)
      line(screen, p[i].oldx, p[i].oldy, p[i].x, p[i].y, p[i].colour);

}



void crash(char i)
{
 int n;

 for (n = 0; n != 3; n++)
   {
    putpixel(screen, p[i].x, p[i].y, 0);
    rest(80);
    putpixel(screen, p[i].x, p[i].y, 15);
    rest(80);
   }

 p[i].dead = 1;
}
