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

const double oobillion = 1.0 / 1e9;
extern double timeDiff(struct timespec *start, struct timespec *end);

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
};

//Function to convert RGB value to hex
unsigned long createRGB(int r, int g, int b)
{
    return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}

//Function to display name on screen
void displayName(int x, int y, float r, float g, float b, const char *text)
{
    // Make a new box to store text
    Rect r2;
    
    // Location of text on screen
    r2.bot = x;
    r2.left = y;

    //Call RGB converter function to get Hex function parameter for ggprint
    unsigned long hexrgb = createRGB(r,g,b);
    ggprint16(&r2, 16, hexrgb, text);
}


//Draw objects
void drawShape(int x, int y)
{       
    Shape s;
    s.center.z=0;
    glColor3ub(255,0,0);
    glPushMatrix();
    glTranslatef(x, y, s.center.z);
    float w = 100;
    float h = 40;
    glBegin(GL_QUADS);
        glVertex2i(-w, -h);
        glVertex2i(-w,  h);
        glVertex2i( w,  h);
        glVertex2i( w, -h);
    glEnd();
    glPopMatrix();
}

//Check for colission
void detectColission()
{






}
