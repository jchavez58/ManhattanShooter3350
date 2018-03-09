// Amir Bakhtar
// header


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <cmath>
#include <ctime>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"

using namespace std;

////////////////

// timers
extern struct timespec timeStart, timeCurrent;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//

void TimeFunc();

void TimeFunc()
{
    static double t = 0.0;
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    int sum = 0;
    Rect r;
    r.bot = 470;
    r.left = 215;
    r.center = 0;
    for(int i = 0; i < 10000; i++){
	sum += i*100;
    }
    clock_gettime(CLOCK_REALTIME, &end);
    t += timeDiff(&start, &end);
    ggprint8b(&r, 16, 0xFFFF00, "amir timer" );
    ggprint8b(&r, 16, 0xFFFF00, "%lf" , t);
}







