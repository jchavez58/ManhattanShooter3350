//Author: Omar Gonzalez
//Date: 03/05/2018
//File contains code for sprite animations and texture mapping 
//Edit: Will now take care of some collision detection and some physics, maybe menu?

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
//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

#define PI 3.141592653589793
#define ALPHA 1
const float GRAVITY =  -0.2f;
//Timers h;
//void setTextures(Image im[])
//{
/*
   im[5] = {
   "./images/mafia2.png",
   "./images/background2.png",
   "./images/couch1.png",
   "./images/umbrella.png",
   "./images/Pillar1.png"
   };
 */
//}
/*
   void SpawnBoss(Game &g, int posy, int posx, int posz, int vel, int wid)
   {
   glPushMatrix();

   glTranslatef(posx,posy, posz);
   if (!g.silhouette) {
   glBindTexture(GL_TEXTURE_2D, g.bigfootTexture);
   } else {
   glBindTexture(GL_TEXTURE_2D, g.silhouetteTexture);
   glEnable(GL_ALPHA_TEST);
   glAlphaFunc(GL_GREATER, 0.0f);
   glColor4ub(255,255,255,255);
   }
   glBegin(GL_QUADS);
   if (vel > 50.0) {
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
   glPopMatrix();

   }
 */
void movecharUp(Global &g)
{
	g.yres += 20;

}

void movecharDown(Global &g)
{
	g.yres -= 20;
}

void moveForward(Global &g)
{
	g.xres += 100;
}
void moveBack(Global &g)
{
	g.xres -= 100;
}

void ShootBullets(Global &g, Bullet *b, Timers & h)
{
        
                //a little time between each bullet
                struct timespec bt;
                clock_gettime(CLOCK_REALTIME, &bt);
                double ts = h.timeDiff(&g.bulletTimer, &bt);
                if (ts > 0.1) {
                        h.timeCopy(&g.bulletTimer, &bt);
                        if (g.nbullets < 20) {
                                //shoot a bullet...
                                //Bullet *b = new Bullet;
                                b = &g.barr[g.nbullets];
                                h.timeCopy(&b->time, &bt);
                                b->pos[0] = g.xres/14.0;
                                b->pos[1] = g.xres/14.0;
                                b->vel[0] = g.yres/2.0;
                                b->vel[1] = g.yres/2.0;
                                
				
				//convert man angle to radians
                                //Flt rad = ((g.xres+90.0) / 360.0f) * PI * 2.0;
                                //convert angle to a vector
                                //Flt xdir = cos(rad);
                                //Flt ydir = sin(rad);
                                b->pos[0] += g.center; //* 20.0f;
                                b->pos[1] += g.center; //* 20.0f;
                                b->vel[0] += g.center; //* 20.0f;
                                b->vel[1] += g.center; //* 20.0f;
                                
				b->color[0] = 1.0f;
                                b->color[1] = 1.0f;
                                b->color[2] = 1.0f;
                                g.nbullets++;
                        }
                }

}




void Drawbullets(Bullet *b, Global &g)
{

	b = &g.barr[0];
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

void UpdateBulletpos(Bullet *b, Global &a, Timers &g)
{
 struct timespec bt;
        clock_gettime(CLOCK_REALTIME, &bt);
        int i=0;
        while (i < a.nbullets) {
                b = &a.barr[i];
                //How long has bullet been alive?
                double ts = g.timeDiff(&b->time, &bt);
                if (ts > 2.5) {
                        //time to delete the bullet.
                        memcpy(&a.barr[i], &a.barr[a.nbullets-1],
                                sizeof(Bullet));
                        a.nbullets--;
                        //do not increment i.
                        continue;
                }
                //move the bullet
                b->pos[0] += b->vel[0];
                b->pos[1] += b->vel[1];
                //Check for collision with window edges
                /*
		if (b->pos[0] < 0.0) {
                        b->pos[0] += (float)a.xres;
                }
                else if (b->pos[0] > (float)a.xres) {
                        b->pos[0] -= (float)a.xres;
                }
                else if (b->pos[1] < 0.0) {
                        b->pos[1] += (float)a.yres;
                }
                else if (b->pos[1] > (float)a.yres) {
                        b->pos[1] -= (float)a.yres;
                }
		*/
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


/*
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
} */
