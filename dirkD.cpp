/*********************************/
/*

Author: Dirk Duclos
Last Modified: 5/10/18

Tasks Assigned:
  1. Colission detection
  2. Basic AI for mafia

*/
/*********************************/

#include"fonts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <ctime>
#include <sstream>
#include <iostream>
//#include "Sprite.cpp"
#include "game.h"
using namespace std;

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

#define PI 3.141592653589793
#define ALPHA 1
const float GRAVITY =  -0.2f;

//Check for character colission with window edges
void detectCharWallColission(int posx, int posy, Global &g)
{
    if (posx <= 100 && posy >= 620) {
    g.xres = 100;
    g.yres = 620;
    }
    else if (posx <= 100 && posy <= 80) {
    g.xres = 100;
    g.yres = 80;
    }
    else if (posx <= 100) {
    g.xres = 100;
    }
    else if (posx >= 11000 && posy >= 620) {
    g.xres = 11000;
    g.yres = 620;
    }
    else if (posx >= 11000 && posy <= 80) {
    g.xres = 11000;
    g.yres = 80;
    }
    else if (posx >= 11000) {
    g.xres = 11000;
    }
    else if (posy <= 80) {
    g.yres = 80;
    }
    else if (posy >= 620) {
    g.yres = 620;
    }
}

/*void deleteEnemy(Global *g, Enemy *node)
{
	//Remove a node from doubly-linked list.
	//Must look at 4 special cases below.
	if (node->prev == NULL) {
		if (node->next == NULL) {
			//only 1 item in list.
			g->ehead = NULL;
		} else {
			//at beginning of list.
			node->next->prev = NULL;
			g->ehead = node->next;
		}
	} else {
		if (node->next == NULL) {
			//at end of list.
			node->prev->next = NULL;
		} else {
			//in middle of list.
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
	}
	delete node;
	node = NULL;
}

/*void detectBullEnemyColission(Bullet *b, Enemy *e, Global *g)
{
    //cout << "bullet pos x: " << (int)b->pos[0]+5 << endl;
    //cout << "shape x: " << s.centerx << endl;
           if (b->pos[0] > e->pos[0]) {
           Enemy *savee = e->next;
           deleteEnemy(g,e);
           e = savee;
           g->nenemies--;
           }
           if (e == NULL)
       			exit(0);
       		e = e->next;
}
*/

void drawLine(int x,int y, int posx, int posy, Global&g)
{
  glPushMatrix();
  glTranslatef(posx, posy, 0);
  g.centerx = posx;
  g.centery = posy;
  g.enxres = x;
  g.enyres = y;
  glColor3f(1.0f, 0.0f, 0.0f); // Let it be blue
  glBegin(GL_LINE_STRIP); // 2x2 pixels
  glVertex2i(x-10, y+30);
  glVertex2i(x-10, -y-30);
  glEnd();
  glPopMatrix();
}
