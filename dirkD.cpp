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
void detectCursorColission(int cx, int cy, int posx, int posy)
{

    


}
