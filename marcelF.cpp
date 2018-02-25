//****************************************
//author: Marcel Furdui
//****************************************

//Lab 5 - 02/23/2018

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
//#include "log.h"
//#include "ppm.h"
#include "fonts.h"
#include "vector"
#include <math.h>


using namespace std;

unsigned int c = 0x00ffff44;
void ggprint8b(Rect *r, int advance, int cref, const char *fmt, ...);

void displayGameName(Rect *r)
{
	ggprint8b(r, 16, c, "Manhattan Assassin");
}

