/* Author: Dirk Duclos

Tasks Assigned:
  1. Colission detection
  2. Basic AI for mafia

*/
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
#include "Sprite.cpp"

/*const double oobillion = 1.0 / 1e9;
extern double timeDiff(struct timespec *start, struct timespec *end);
*/

extern void Destroy(character&);
// vector
//struct Vec {
//	float x, y, z;

//};
// make an object to check for colission
struct Shape {
        float width, height;
	float radius;
	Vec center;
	struct timespec end;
} s;

//Check for colission
/*
void detectCursorColission(int cx, int cy, int posx, int posy, character &g)
{

    if (cx == posx || cy == posy) {
    Destroy(g);
    }
}
*/
void drawBox(int x,int y, int posx, int posy, int posz)
{
    glPushMatrix();
    glTranslatef(posx, posy, posz);
    glColor3f(0.0f, 0.0f, 1.0f); // Let it be blue
    glBegin(GL_LINE_STRIP); // 2x2 pixels
    glVertex2f(-x+250, -y+310);
    glVertex2f(-x+250, y-310);
    glVertex2f(x-250, y-310);
    glVertex2f(x-250, -y+310);
    glEnd();
    glPopMatrix();
}
