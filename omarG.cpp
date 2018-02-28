#include"fonts.h"
void displayName (const char* name, int x, int y) {
    //Object that handles rendering of text
    Rect r;
    //Position of text entered as x and y
    r.bot = x;
    r.left = y; 
    //Print name to screen
    ggprint16(&r, 16, 0x0ffff00, name);
}

