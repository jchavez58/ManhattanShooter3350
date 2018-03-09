//****************************************
//author: Marcel Furdui
//****************************************

//Lab 5 - 02/23/2018
#include "fonts.h"
#include <time.h>

const double physicsRate = 1.0 / 60;
const double oobillion = 1.9 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);

using namespace std;


void displayGameName (int x, int y, const char* name) {

	unsigned int c = 0x00ffff44;

	Rect r;
	r.bot = x;
	r.center = y;
    
    ggprint8b(&r, 16, c, name);
}

void timeCount()
{
	static double t = 0.0;
	struct timespec ftimeStart, ftimeEnd;
	clock_gettime(CLOCK_REALTIME, &ftimeStart);
	int a = 200, b = 300;
	for (int i=0; i<100; i++) {
		a = (b*b*b*a*764743*a*b*a*b);
		b = (a&3) * (b&88);
	}

	clock_gettime(CLOCK_REALTIME, &ftimeEnd);
	t += timeDiff(&ftimeStart, &ftimeEnd);
	
	unsigned int c = 0x00ffff44;

	Rect r;
	r.bot = 350;
	r.left = 400;

	ggprint8b(&r, 80, c, "Marcel Furdui's TimeFunc: %f", t);
	
}

