/*********************************/
/*

Author: Dirk Duclos
Last Modified: 5/8/18

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

//extern void Destroy(character&);

// make an object to check for colission
struct Shape {
  float width, height;
	float radius;
	int centerx;
  int centery;
	struct timespec end;
} s;

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

//void detectBullEnemyColission(Bullet *b)
//{
    //cout << "bullet pos x: " << (int)b->pos[0] << endl;
    //cout << "shape x: " << s.centerx << endl;
    //if (b->pos[0] == s.centerx && b->pos[1] == s->centery)
   // {
     // cout << "bullet hit" << endl;
   // }
//}

void drawBox(int x,int y)
{
  s.centerx = x;
  s.centery = y;
  glPushMatrix();
  glTranslatef(x, y, 0);
  glColor3f(0.0f, 0.0f, 1.0f); // Let it be blue
  glBegin(GL_LINE_STRIP); // 2x2 pixels
  glVertex2f(-10, -10);
  glVertex2f(-10, 10);
  glVertex2f(10, 10);
  glVertex2f(10, -10);
  glEnd();
  glPopMatrix();
}
