//Author: Omar Gonzalez
//Date: 03/05/2018
//File contains code for sprite animations and texture mapping

#include"fonts.h"
#include <GL/glx.h>
#include <time.h>

void displayName (const char* name, int x, int y) {
	//Object that handles rendering of text
	Rect r;
	//Position of text entered as x and y
	r.bot = x;
	r.left = y; 
	//Print name to screen
	ggprint16(&r, 16, 0x0ffff00, name);
}

//setup timers
const double physicsRate = 1.0 / 60;
const double oobillion = 1.90 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);



void draw()
{
	static double time = 0.0;
	struct timespec start, end;
	clock_gettime(CLOCK_REALTIME, &start);

	glBegin(GL_TRIANGLES);
	glColor3f(0.1,0.2,0.3);
	glVertex2f(300, 210);
	glVertex2f(340, 215);
	glVertex2f(320, 250);
	glEnd();
	glPopMatrix();    

	int div =100000; 
	int res = 7890202;
	for (int i= 0; i < 1000; i++)
	{ 
		int total = div / res;
		total = total / 12;
		total = total / 100;
	} 
	
	clock_gettime(CLOCK_REALTIME, &end);
	time +=  timeDiff(&start, &end);

	//text to display time
	Rect r;

	r.bot = 200;
	r.left = 250;

	ggprint16(&r, 16, 0x0ffff00, "Function Time Omar Gonzalez: %f ", time);
}



void draw2()
{
	static double time = 0.0;
	struct timespec start, end;
	clock_gettime(CLOCK_REALTIME, &start);

	glBegin(GL_TRIANGLES);
	glColor3f(0.1,0.2,0.3);
	glVertex2f(300, 210);
	glVertex2f(340, 215);
	glVertex2f(320, 250);
	glEnd();
	glPopMatrix();    

	int div =100000; 
	int res = 7890202;
	for (int i= 0; i < 1000; i++)
	{ 
		int total = (div & res);
		total = (total & 12);
		total = (total / 100);
	} 
	
	clock_gettime(CLOCK_REALTIME, &end);
	time +=  timeDiff(&start, &end);

	//text to display time
	Rect r;

	r.bot = 320;
	r.left = 250;

	ggprint16(&r, 16, 0x0ffff00, "Function Time 2 Omar Gonzalez: %f ", time);
}


