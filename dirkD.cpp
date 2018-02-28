//Author: Dirk Duclos
//Framework used: Asteroids

#include"fonts.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>

//Function to convert RGB value to hex
unsigned long createRGB(int r, int g, int b)
{
    return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}

//Function to display name on screen
void displayName(int x, int y, float r, float g, float b, const char *text)
{
    // Clears screen of asteroids program objects (ship, asteroids, etc)
    //glClear(GL_COLOR_BUFFER_BIT);
    
    // Make a new box to store text
    Rect r2;
    
    // Location of text on screen
    r2.bot = x;
    r2.left = y;

    //Call RGB converter function to get Hex function parameter for ggprint
    unsigned long hexrgb = createRGB(r,g,b);
    ggprint16(&r2, 16, hexrgb, text);
}

