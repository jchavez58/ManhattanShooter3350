//3350
//program: walk.cpp
//author:  Gordon Griesel
//Modified By : Omar Gonzalez
//date:  02/24/2018
//
//Walk cycle using a sprite sheet.
//images courtesy: http://games.ucla.edu/resource/walk-cycles/
//
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
#include "game.h"

//defined types
//typedef double Flt;
//typedef double Vec[3];
//typedef Flt	Matrix[4][4];

//macros
//#define rnd() (((double)rand())/(double)RAND_MAX)
//#define random(a) (rand()%a)
//#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
//#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
//#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
//#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0];  \(c)[1]=(a)[1]-(b)[1]; \(c)[2]=(a)[2]-(b)[2]
//constants
//const float timeslice = 1.0
//const float gravity = -0.2f;
bool flip = false;
bool cd = false;
#define ALPHA 1
extern void Drawbullets(Bullet*, Global&);
extern void ShootBullets(Global&, Bullet*, Timers&);
extern void movecharUp(Global&);
extern void movecharDown(Global&);
extern void moveForward(Global&);
extern void moveBack(Global&);
//extern voindraw();
//extern void draw2();
//extern void TimeFunc();
//extern setBackground();

bool inMainMenu = true;
bool inGame = false;
bool inPauseMenu = false;
int menuPosition = 1;
//bool inMainMenu = false;
//bool inMainMenu = false;

Image img[] = {"images/walk_left.png","images/Background.jpg",
	"images/menu11.png", "images/pointer.png"};
//-----------------------------------------------------------------------------
//Setup timers
Timers timers;
//-----------------------------------------------------------------------------

Global g;
//Bullet *b;


class X11_wrapper {
	private:
		Display *dpy;
		Window win;
	public:
		X11_wrapper() {
			GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
			//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
			XSetWindowAttributes swa;
			setupScreenRes(g.xres, g.yres);
			dpy = XOpenDisplay(NULL);
			if (dpy == NULL) {
				printf("\n\tcannot connect to X server\n\n");
				exit(EXIT_FAILURE);
			}
			Window root = DefaultRootWindow(dpy);
			XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
			if (vi == NULL) {
				printf("\n\tno appropriate visual found\n\n");
				exit(EXIT_FAILURE);
			}

			Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
			swa.colormap = cmap;
			swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
				StructureNotifyMask | SubstructureNotifyMask;
			win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
					vi->depth, InputOutput, vi->visual,
					CWColormap | CWEventMask, &swa);
			GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
			glXMakeCurrent(dpy, win, glc);
			setTitle();
		}
		~X11_wrapper() {
			XDestroyWindow(dpy, win);
			XCloseDisplay(dpy);
		}
		void setTitle() {
			//Set the window title bar.
			XMapWindow(dpy, win);
			XStoreName(dpy, win, "Walk Cycle");
		}
		void setupScreenRes(const int w, const int h) {
			g.xres = w;
			g.yres = h;
		}
		void reshapeWindow(int width, int height) {
			//window has been resized.
			setupScreenRes(width, height);
			glViewport(0, 0, (GLint)width, (GLint)height);
			glMatrixMode(GL_PROJECTION); glLoadIdentity();
			glMatrixMode(GL_MODELVIEW); glLoadIdentity();
			glOrtho(0, g.xres, 0, g.yres, -1, 1);
			setTitle();
		}
		void checkResize(XEvent *e) {
			//The ConfigureNotify is sent by the
			//server if the window is resized.
			if (e->type != ConfigureNotify)
				return;
			XConfigureEvent xce = e->xconfigure;
			if (xce.width != g.xres || xce.height != g.yres) {
				//Window size did change.
				reshapeWindow(xce.width, xce.height);
			}
		}
		bool getXPending() {
			return XPending(dpy);
		}
		XEvent getXNextEvent() {
			XEvent e;
			XNextEvent(dpy, &e);
			return e;
		}
		void swapBuffers() {
			glXSwapBuffers(dpy, win);
		}

} x11;

//function prototypes
void initOpengl(void);
void checkMouse(XEvent *e);
int checkKeys(XEvent *e);
void init();
void physics(void);
void render(void);


int main(void)
{
	initOpengl();
	init();
	int done = 0;
	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.checkResize(&e);
			checkMouse(&e);
			done = checkKeys(&e);
		}
		physics();
		render();
		x11.swapBuffers();

	}
	cleanup_fonts();
	return 0;
}

unsigned char *buildAlphaData(Image *img)
{
	//add 4th component to RGB stream...
	int i;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	unsigned char a,b,c;
	//use the first pixel in the image as the transparent color.
	unsigned char t0 = *(data+0);
	unsigned char t1 = *(data+1);
	unsigned char t2 = *(data+2);
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = 1;
		if (a==t0 && b==t1 && c==t2)
			*(ptr+3) = 0;
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}
	return newdata;
}

void initOpengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, g.xres, g.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//
	//load the images file into a ppm structure.
	//
	int w = img[0].width;
	int h = img[0].height;
	//
	//create opengl texture elements
	glGenTextures(1, &g.walkTexture);
	glGenTextures(1, &g.mainMenuTexture);
	glGenTextures(1, &g.pointerTexture);
	//-------------------------------------------------------------------------
	//silhouette
	//this is similar to a sprite graphic
	//
	glBindTexture(GL_TEXTURE_2D, g.walkTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *walkData = buildAlphaData(&img[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, walkData);



	w = 1024;
	h = 768;
	glBindTexture(GL_TEXTURE_2D, g.mainMenuTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *mainMenuData = buildAlphaData(&img[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, mainMenuData);

	w = 40;
	h = 24;
	glBindTexture(GL_TEXTURE_2D, g.pointerTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *pointerData = buildAlphaData(&img[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, pointerData);


	//free(walkData);
	//unlink("./images/walk.ppm");
	//-------------------------------------------------------------------------
}

void init() {

}

void checkMouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	//
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
}

int checkKeys(XEvent *e)
{
	//keyboard input
	//float moveup = g.yres/2.0;
	Bullet * b = NULL;
	static int shift=0;
	if (e->type != KeyRelease && e->type != KeyPress)
		return 0;
	int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
	if (e->type == KeyRelease) {
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift = 0;
		return 0;
	}
	if (key == XK_Shift_L || key == XK_Shift_R) {
		shift=1;
		return 0;
	}
	(void)shift;
	if(inMainMenu) {
		if (key == XK_Return) {
			if (menuPosition == 1) {
				inMainMenu = false;
				inGame = true;
			}
		} else if (key == XK_Down || key == XK_s) {
			if (menuPosition != 2) {
				menuPosition++;
			}
		} else if (key == XK_Up || key == XK_w) {
			if (menuPosition != 1) {
				menuPosition--;
			}
		}	
	}
	if(inGame) {
		switch (key) {
			case XK_a:
				//for(int i =0; i < 300 ; i++) 
				ShootBullets(g,b,timers);
				break;
			case XK_w:
				timers.recordTime(&timers.walkTime);
				g.walk ^= 1;
				break;
			case XK_Left:
				moveBack(g);
				flip = true;
				//g.xres -= 5;
				//extern void moveForward(Global&);
				//moveForward(g);
				break;
			case XK_Right:
				moveForward(g);
				flip = false;
				break;
			case XK_Up:
				//g.yres += 10;
				//moveup;
				//g.walkFrame += 5;
				//extern void movecharUp(Global&);
				movecharUp(g);
				break;
			case XK_Down:
				//extern void movecharDown(Global&);
				movecharDown(g);
				//g.yres -= 10;
				break;
			case XK_equal:
				g.delay -= 0.005;
				if (g.delay < 0.005)
					g.delay = 0.005;
				break;
			case XK_minus:
				g.delay += 0.005;
				break;
			case XK_Escape:
				return 1;
				break;
		}
	}
	return 0;
}

Flt VecNormalize(Vec vec)
{
	Flt len, tlen;
	Flt xlen = vec[0];
	Flt ylen = vec[1];
	Flt zlen = vec[2];
	len = xlen*xlen + ylen*ylen + zlen*zlen;
	if (len == 0.0) {
		MakeVector(0.0,0.0,1.0,vec);
		return 1.0;
	}
	len = sqrt(len);
	tlen = 1.0 / len;
	vec[0] = xlen * tlen;
	vec[1] = ylen * tlen;
	vec[2] = zlen * tlen;
	return(len);
}
extern void UpdateBulletpos(Bullet*, Global&, Timers&);
//Timers t;
void physics(void)
{        
	Bullet *b = NULL;
	if (g.walk) {
		//man is walking...
		//when time is up, advance the frame.
		timers.recordTime(&timers.timeCurrent);
		double timeSpan = timers.timeDiff(&timers.walkTime, &timers.timeCurrent);
		if (timeSpan > g.delay) {
			//advance
			++g.walkFrame;

			//Dirk Duclos
			//With each frame, update x position of walk frame
			//Need to change walk frame to be just image
			//g.xres += 5;

			if (g.walkFrame >= 16)
				g.walkFrame -= 16;
			timers.recordTime(&timers.walkTime);
			//UpdateBulletpos(b,g);

		}
		UpdateBulletpos(b,g,timers);
		for (int i=0; i<20; i++) {
			g.box[i][0] -= 2.0 * (0.05 / g.delay);
			if (g.box[i][0] < -10.0)
				g.box[i][0] += g.xres + 10.0;

		}
	}
}



void render(void)
{
	Rect r;


	//Clear the screen
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	float cx = g.xres/14.0;
	float cy = g.yres/2.0;
	if (inMainMenu) { 
		int sh = g.yres/2; 
		int sw = (sh*1.33333);
		glPushMatrix();
		glTranslatef(g.xres/2, g.yres/2, 0);
		glBindTexture(GL_TEXTURE_2D, g.mainMenuTexture);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(-sw,-sh);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(-sw, sh);
		glTexCoord2f(1.0f, 0.0f); glVertex2i( sw, sh);
		glTexCoord2f(1.0f, 1.0f); glVertex2i( sw,-sh);
		glEnd();
		glPopMatrix();	
		extern void mainMenu(const int, const int, GLuint);
		mainMenu(g.xres, g.yres, g.pointerTexture);
	} else if (inGame) {
		//
		//show ground
		glBegin(GL_QUADS);
		glColor3f(0.2, 0.2, 0.2);
		glVertex2i(0,       280);
		glVertex2i(g.gxres, 280);
		glColor3f(0.4, 0.4, 0.4);
		glVertex2i(g.gxres,   0);
		glVertex2i(0,         0);
		glEnd();
		//
		//fake shadow
		//glColor3f(0.25, 0.25, 0.25);
		//glBegin(GL_QUADS);
		//	glVertex2i(cx-60, 150);
		//	glVertex2i(cx+50, 150);
		//	glVertex2i(cx+50, 130);
		//	glVertex2i(cx-60, 130);
		//glEnd();
		//

		//show boxes as background
		for (int i=0; i<20; i++) {
			glPushMatrix();
			glTranslated(g.box[i][0],g.box[i][1],g.box[i][2]);
			glColor3f(0.2, 0.2, 0.2);
			glBegin(GL_QUADS);
			glVertex2i( 0,  0);
			glVertex2i( 0, 30);
			glVertex2i(20, 30);
			glVertex2i(20,  0);
			glEnd();
			glPopMatrix();
		}

		//walk frame.
		float h = 40.0;
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
		glBegin(GL_QUADS);
		glTexCoord2f(tx,      ty+1.0); glVertex2i(flip ? cx+w: cx-w, cy-h);
		glTexCoord2f(tx,      ty);    glVertex2i(flip ? cx+w: cx-w, cy+h);
		glTexCoord2f(tx+.220, ty);    glVertex2i(flip ? cx-w: cx+w, cy+h);
		glTexCoord2f(tx+.220, ty+1.0); glVertex2i(flip ? cx-w: cx+w, cy-h); //cy-h;

		//

		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);

		//DrawBullets
		//extern void Drawbullets(Bullet*, Global&);
		Bullet * b = NULL;
		Drawbullets(b,g);

		// Menu

		int topright = 600;
		unsigned int c = 0x00ffff44;
		r.bot = topright - 20;
		r.left = 10;
		r.center = 0;
		//ggprint8b(&r, 16, c, "W   Walk cycle");
		ggprint8b(&r, 16, c, "+   faster");
		ggprint8b(&r, 16, c, "-   slower");
		ggprint8b(&r, 16, c, "right arrow -> walk right");
		ggprint8b(&r, 16, c, "left arrow  <- walk left");
		//ggprint8b(&r, 16, c, "frame: %i", g.walkFrame);
		ggprint8b(&r, 16, c, "Time left: ");
		ggprint8b(&r, 16, c, "Targets Eliminated: ");

		//Names of Group members lab5
		//extern void displayTimeFunc(int x, int y, double (&x)(double));
		extern void displayGameName(int x, int y, const char* name);
		/*extern void displayName (const char* name,int x , int y);
		  extern void displayName(int x, int y, float r, float g, float b, const char *text);
		  displayName(200, 200, 256, 0, 0, "Dirk Duclos");
		  displayName("Omar Gonzalez", 100, 100);
		  displayGameName(300, 50, "Marcel Furdui");
		//displayTimeFunc(300, 50, &calc());
		 */

		// Dirk Duclos
		//Function to draw box for colission detection
		//extern void drawShape(int, int);


		//drawShape(cx+200, cy+150);
		//setBackground();

		//Omar Gonzalez
		//Profiling Function
		//First test
		/*draw();
		//Optimized Fuinction
		draw2();
		 */
		//Amir
		/*TimeFunc();

		  extern void timeCount();
		  timeCount();
		 */
	}
}

