//Author: Omar Gonzalez
//Date: 03/05/2018
//File contains code for sprite animations and texture mapping

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>
#include <GL/glx.h>
#include "fonts.h"
#include "Sprite.cpp"
//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];
/*
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0];  \(c)[1]=(a)[1]-(b)[1]; \(c)[2]=(a)[2]-(b)[2]
*/
//Initliaze characters
Image1 img1("./images/mafia2.png");
character boss;
Game sprite1;



//Press 'b'  to spawn the characters and objects drawn in the screen

void SpwanChar()
{

	float wid = 120.0f; //120

	glTranslatef(boss.pos[0], boss.pos[1], boss.pos[2]);
	if (!sprite1.silhouette) {
		glBindTexture(GL_TEXTURE_2D, sprite1.bossTexture);
	} else {
		glBindTexture(GL_TEXTURE_2D, sprite1.silhouetteTexture);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
	}
	glBegin(GL_QUADS);
		if (boss.vel[0] > 50.0) {
			glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid); //-wid -wid
			glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
			glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
			glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
		} else {
			glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
			glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
			glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
		}

	glEnd();
}


void displayMenu (const char* mess, int x, int y) {
	//Object that handles rendering of text
	Rect r;
	//Position of text entered as x and y
	r.bot = x;
	r.left = y;
	//Print name to screen
	ggprint16(&r, 16, 0x0ffff00, mess);
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
		total = (total & 100);
	}

	clock_gettime(CLOCK_REALTIME, &end);
	time +=  timeDiff(&start, &end);

	//text to display time
	Rect r;

	r.bot = 320;
	r.left = 250;

	ggprint16(&r, 16, 0x0ffff00, "Function Time 2 Omar Gonzalez: %f ", time);
}
