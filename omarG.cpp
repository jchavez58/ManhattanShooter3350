//Author: Omar Gonzalez
//Date: 03/05/2018
//File contains code for sprite animations and texture mapping
//Edit: Will now take care of some collision detection and some physics
//Professor: Gordon Griesel
//CMPS 3350 Software Engineering

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
//#include <sring.h>
#include "fonts.h"
//#include "Sprite.cpp"
#include "game.h"
#include <iostream>
using namespace std;
//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

#define PI 3.141592653589793
#define ALPHA 1
const float GRAVITY =  -0.2f;
extern bool flip;
Timers tl;

void EnemyLoop(Global &g)
{
	//Resets the position of character to
	//starting position; reuses the characters
	if (g.exres == 0) {
		cout << "off Screen" << endl;
		while (!g.exres) {
			g.exres = g.gxres/0.9;
			g.exres = g.exres/0.9;
		}
		g.exres = g.exres/0.9;
	}

}

void drawEnemy(Enemy& enemy, Global& g)
{

	//spawns enemies every n seconds
	tl.recordTime(&tl.timeCurrent);
	//double totalTime = tl.timeDiff(&enemy.time, &tl.timeCurrent);

	int cx = enemy.pos[0];
	int cy = enemy.pos[1];

	float h = 30.0;
	float w = h * 0.5; //0.5 h = 50.0

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, g.alienTexture);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	int ix = g.walkFrame % 4;
	int iy = 0;
	if (g.walkFrame >= 4)
		iy = 1;
	float tx = (float)ix / 4.0;
	float ty = (float)iy / 1.0;
	glBegin(GL_QUADS);

	glTexCoord2f(tx,      ty+1.0); glVertex2i(cx+w, cy-h);
	glTexCoord2f(tx,      ty);    glVertex2i(cx+w, cy+h);
	glTexCoord2f(tx+.240, ty);    glVertex2i(cx-w, cy+h);
	glTexCoord2f(tx+.240, ty+1.0); glVertex2i(cx-w, cy-h);

	glEnd();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);

}

float RandomizeEnemyPosx()
{
	srand((unsigned int)time(NULL));

	float a = 16.0;
	//for (int i=0;i<20;i++)
	float res =  ( ((float)rand()/(float)(RAND_MAX)) * a)+ 2.0;
	return res;
}


void moveEnemy(Global &g)
{
	//updates the postion of the enemies
	g.exres -= 5;

}


void movecharUp(Global &g)
{
	g.yres += 20;
	//updatecenter
	g.centery += 20;
}

void movecharDown(Global &g)
{
	g.yres -= 20;
	//Updatecenter
	g.centery -= 20;
}

void moveForward(Global &g)
{
	g.xres += 100;
	//Updatecenter
	g.centerx += 100;
}
void moveBack(Global &g)
{
	g.xres -= 100;
	//update center
	g.centerx -= 100;
}
void ShootBullets(Global &g, Bullet *b, Timers & h)
{

	//a lets(Global &g, Bullet *b, Timers & h)


	//a little time between each bullet
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	double ts = h.timeDiff(&g.bulletTimer, &bt);
	if (ts > 0.01) {
		h.timeCopy(&g.bulletTimer, &bt);
		if (g.nbullets < max_bullets) {
			//shoot a bullet...
			//Bullet *b = new Bullet;
			b = &g.barr[g.nbullets];
			h.timeCopy(&b->time, &bt);
			b->pos[0] = g.xres/14.0;
			b->pos[1] = g.yres/2.0;
			b->vel[0] = 10;//g.centerx+5;//g.yres/2.0;
			b->vel[1] = 0;//g.centerx-5;//g.yres/2.0;


			//convert man angle to radians
			//Flt rad = ((g.xres+90.0) / 360.0f) * PI * 2.0;
			//convert angle to a vector
			//Flt xdir = cos(rad);
			//Flt ydir = sin(rad);
			// b->pos[0] += g.center; //* 20.0f;
			//b->pos[1] += g.center; //* 20.0f;
			//b->vel[0] += g.center; //* 20.0f;
			// b->vel[1] += g.center; //* 20.0f;

			b->color[0] = 1.0f;
			b->color[1] = 1.0f;
			b->color[2] = 1.0f;
			g.nbullets++;
		}
	}
}


void Drawbullets(Global &g)
{
	Bullet* b = g.barr;
	for (int i=0; i<g.nbullets; i++) {
		//Log("draw bullet...\n");
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_POINTS);
		glVertex2f(b->pos[0],      b->pos[1]);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]);
		glVertex2f(b->pos[0],      b->pos[1]-1.0f);
		glVertex2f(b->pos[0],      b->pos[1]+1.0f);
		glColor3f(0.8, 0.8, 0.8);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
		glEnd();
		++b;
	}

}

void UpdateBulletpos(Bullet *b, Global &g, Timers &t)
{
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	int i=0;
	while (i < g.nbullets) {
		b = &g.barr[i];
		//How long has bullet been alive?
		double ts = t.timeDiff(&b->time, &bt);
		if (ts > 2.5) {
			//time to delete the bullet.
			memcpy(&g.barr[i], &g.barr[g.nbullets-1],
					sizeof(Bullet));
			g.nbullets--;
			//do not increment i.
			continue;
		}
		//move the bullet
		b->pos[0] += b->vel[0];
		b->pos[1] += b->vel[1];

		//Check for collision with window edges

		extern void calculateCollisionOfBullet(Bullet *b, Global& g);
		calculateCollisionOfBullet(b, g);
		i++;
	}

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
	time +=  tl.timeDiff(&start, &end);

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
	time +=  tl.timeDiff(&start, &end);

	//text to display time
	Rect r;

	r.bot = 320;
	r.left = 250;

	ggprint16(&r, 16, 0x0ffff00, "Function Time 2 Omar Gonzalez: %f ", time);
}
void renderHero(Global &g, int cx, int cy)
{

	flip = false;
	float h = 30.0;
	float w = h * 0.5; //0.5 h = 50.0
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, g.walkTexture);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	int ix = g.walkFrame % 4;
	int iy = 0;
	if (g.walkFrame >= 4)
		iy = 1;
	float tx = (float)ix / 4.0;
	float ty = (float)iy / 1.0;
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glTexCoord2f(tx,      ty+1.0); glVertex2i(flip ? cx+w: cx-w, cy-h);
	glTexCoord2f(tx,      ty);    glVertex2i(flip ? cx+w: cx-w, cy+h);
	glTexCoord2f(tx+.240, ty);    glVertex2i(flip ? cx-w: cx+w, cy+h);
	glTexCoord2f(tx+.240, ty+1.0); glVertex2i(flip ? cx-w: cx+w, cy-h); //cy-h;

	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);


}

void renderBackground(Global &g)
{
	glColor3f(1.0, 1.0, 1.0);

	glBindTexture(GL_TEXTURE_2D, g.backTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(g.xc[0], g.yc[1]); glVertex2i(0, 280);
	glTexCoord2f(g.xc[0], g.yc[0]); glVertex2i(0, g.byres);
	glTexCoord2f(g.xc[1], g.yc[0]); glVertex2i(g.bxres, g.byres);
	glTexCoord2f(g.xc[1], g.yc[1]); glVertex2i(g.bxres, 280);
	glEnd();
}

void moveBackground(Global &g)
{
	//updates position of background to loop
	g.xc[0] += 0.001;
	g.xc[1] += 0.001;

}

void renderGround(Global &g)
{
	glBegin(GL_QUADS);
	glColor3f(0.2, 0.2, 0.2);
	glVertex2i(0,       280);
	glVertex2i(g.gxres, 280);
	glColor3f(0.4, 0.4, 0.4);
	glVertex2i(g.gxres,   0);
	glVertex2i(0,         0);
	glEnd();

}

void initB(Global &g)
{
	//initializes pos of background image
	g.xc[0] = 0.0;
	g.xc[1] = 0.25;
	g.yc[0] = 0.0;
	g.yc[1] = 1.0;

}
