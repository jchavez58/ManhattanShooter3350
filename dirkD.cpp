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

/*const double oobillion = 1.0 / 1e9;
extern double timeDiff(struct timespec *start, struct timespec *end);
*/

// vector
struct Vec {
	float x, y, z;

};
// make an object to check for colission
struct Shape {
        float width, height;
	float radius;
	Vec center;
	struct timespec end;
} s;


//Check for colission
//void detectCursorColission(int cx, int cy, int posx, int posy)
//{




//}

void drawBox(int x,int y, int posx, int posy, int posz)
{
	glPushMatrix();
	glTranslatef(posx, posy, posz);
	glColor3f(0.0f, 0.0f, 1.0f); // Let it be blue
	glBegin(GL_LINE_STRIP); // 2x2 pixels
	/*glTexCoord2f(posx,posy);*/ glVertex2f(-x+250, -y+300);
	/*glTexCoord2f(posx,posy);*/ glVertex2f(-x+250, y-310);
	/*glTexCoord2f(posx,posy);*/ glVertex2f(x-250, y-310);
	/*glTexCoord2f(posx,posy);*/ glVertex2f(x-250, -y+300);
	glEnd();
	glPopMatrix();

//Shape s;
//s.center.x = x;
//s.center.y = y;

//glPushMatrix();
//glTranslatef(s.center.x, s.center.y, s.center.z);
//s.width = x;
//s.height = y;

/*float w = s.width;
float h = s.height;

glBegin(GL_QUADS);
    glVertex2i(-w,-h);
    glVertex2i(-w,h);
    glVertex2i(w,h);
    glVertex2i(w,-h);
glEnd();
glPopMatrix();
*/
}
