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
#include <GL/glx.h>
#include "fonts.h"
//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0];  \(c)[1]=(a)[1]-(b)[1]; \(c)[2]=(a)[2]-(b)[2]
/*
class Timers {
	public:
		double physicsRate;
		double oobillion;
		struct timespec timeStart, timeEnd, timeCurrent;
		struct timespec walkTime;
		Timers() {
			physicsRate = 1.0 / 30.0;
			oobillion = 1.0 / 1e9;
		}
		double timeDiff(struct timespec *start, struct timespec *end) {
			return (double)(end->tv_sec - start->tv_sec ) +
				(double)(end->tv_nsec - start->tv_nsec) * oobillion;
		}
		void timeCopy(struct timespec *dest, struct timespec *source) {
			memcpy(dest, source, sizeof(struct timespec));
		}
		void recordTime(struct timespec *t) {
			clock_gettime(CLOCK_REALTIME, t);
		}
} timers;

class Image {

	public:
		int width, height;
		unsigned char *data;
		~Image() { delete [] data; }
		Image(const char *fname) {
			if (fname[0] == '\0')
				return;
			//printf("fname **%s**\n", fname);
			char name[40];
			strcpy(name, fname);
			int slen = strlen(name);
			name[slen-4] = '\0';
			//printf("name **%s**\n", name);
			char ppmname[80];
			sprintf(ppmname,"%s.ppm", name);
			//printf("ppmname **%s**\n", ppmname);
			char ts[100];
			//system("convert eball.jpg eball.ppm");
			sprintf(ts, "convert %s %s", fname, ppmname);
			system(ts);
			//sprintf(ts, "%s", name);
			FILE *fpi = fopen(ppmname, "r");
			if (fpi) {
				char line[200];
				fgets(line, 200, fpi);
				fgets(line, 200, fpi);
				while (line[0] == '#')
					fgets(line, 200, fpi);
				sscanf(line, "%i %i", &width, &height);
				fgets(line, 200, fpi);
				//get pixel data
				int n = width * height * 3;
				data = new unsigned char[n];
				for (int i=0; i<n; i++)
					data[i] = fgetc(fpi);
				fclose(fpi);
			} else {
				printf("ERROR opening image: %s\n",ppmname);
				exit(0);
			}
			unlink(ppmname);
		}
};
Image img2[] = {"images/walk_left.png"};

class Global {
	public:
		int done;
		int xres2, yres2;
		int walk;
		int walkFrame;
		double delay;
		GLuint walkTexture;
		Vec box[20];
		Global() {
			done=0;
			xres2=800;
			yres2=600;
			walk=0;
			walkFrame=0;
			delay = 0.1;
			for (int i=0; i<20; i++) {
        box[i][0] = rnd() * xres2;
				box[i][1] = rnd() * (yres2-220) + 220.0;
				box[i][2] = 0.0;
			}
		}
} g2;
void physics2(void)
{
	if (g2.walk) {
		//man is walking...
		//when time is up, advance the frame.
		timers.recordTime(&timers.timeCurrent);
		double timeSpan = timers.timeDiff(&timers.walkTime, &timers.timeCurrent);
		if (timeSpan > g2.delay) {
			//advance
			++g2.walkFrame;

                        //Dirk Duclos
                        //With each frame, update x position of walk frame
                        //Need to change walk frame to be just image
      //g2.xres2 += 5;

    if (g2.walkFrame >= 16)
				g2.walkFrame -= 16;
			timers.recordTime(&timers.walkTime);


		}
		for (int i=0; i<20; i++) {
		//	g.box[i][0] -= 2.0 * (0.05 / g.delay);
		//	if (g.box[i][0] < -10.0)
		//		g.box[i][0] += g.xres + 10.0;

		}
	}
}


void Sprite1 ()
{
	float cx = g2.xres2/2.0;
	float cy = g2.yres2/2.0;
	float h = 40.0;
	float w = h * 0.5; //0.5 h = 50.0
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, g2.walkTexture);



	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	int ix = g2.walkFrame % 4;
	int iy = 0;
	if (g2.walkFrame >= 4)
		iy = 1;
	float tx = (float)ix / 4.0;
	float ty = (float)iy / 1.0;
	glBegin(GL_QUADS);
	glTexCoord2f(tx,      ty+1.0); glVertex2i(cx-w, cy-h);
	glTexCoord2f(tx,      ty);    glVertex2i(cx-w, cy+h);
	glTexCoord2f(tx+.220, ty);    glVertex2i(cx+w, cy+h);
	glTexCoord2f(tx+.220, ty+1.0); glVertex2i(cx+w, cy-h);
	 glEnd();
	 glPopMatrix();
	 glBindTexture(GL_TEXTURE_2D, 0);
	 glDisable(GL_ALPHA_TEST);

}
*/
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
