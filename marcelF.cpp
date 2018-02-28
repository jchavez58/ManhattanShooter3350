//****************************************
//author: Marcel Furdui
//****************************************

//Lab 5 - 02/23/2018
#include "fonts.h"

using namespace std;


void displayGameName (int x, int y, const char* name) {
	unsigned int c = 0x00ffff44;

	Rect r;
	r.bot = x;
	r.center = y;
    
    ggprint8b(&r, 16, c, name);
}

