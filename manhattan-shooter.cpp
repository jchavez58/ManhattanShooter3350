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
#include <iostream>

using namespace std;

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
extern void detectCharWallColission(int,int,Global&); //dirkD
extern void detectBullEnemyColission(Bullet*); //dirkD
extern void EnemyLoop(Global&);
extern void spawnEnemy(Global&,const float, const float);
extern float RandomizeEnemyPosx();
extern float moveEnemy(Global&);
extern float RandomizeEnemyPosx();
float res= RandomizeEnemyPosx();
float res1= RandomizeEnemyPosx();
float res2= RandomizeEnemyPosx();
float res3= RandomizeEnemyPosx();
float res4= RandomizeEnemyPosx();

//extern voindraw();
//extern void draw2();
//extern void TimeFunc();
//extern setBackground();

extern void initSounds();
extern void uninitSounds();
extern void makeSoundTest();
extern void makeReloadSound();        //marcelF
extern void renderMagazineCount();    //marcelF
extern void renderGameTime();         //marcelF
extern Global g;                      //marcelF
extern Timers timers;                 //marcelF
extern bool inGame;                   //marcelF

extern void renderPointer(const int, GLuint);
extern void renderMainMenu(const int, const int, GLuint);
extern void renderTutorial(const int, const int, GLuint);
extern void renderCredits(const int, const int, GLuint);

bool inMainMenu = true;
bool inGame = false;
bool inPauseMenu = false;
bool inTutorial = false;
bool inCredits = false;
int menuPosition = 1;
//bool inMainMenu = false;
//bool inMainMenu = false;

Image img[] = {"images/walk_left.png", "images/Background.jpg",
    "images/menu11.png", "images/pointer.png",
    "images/tutorial.png", "images/credits.png", "images/enemy_left3.png", "images/Background1.jpg"};
//-----------------------------------------------------------------------------
//Setup timers
Timers timers;
//-----------------------------------------------------------------------------

Global g;
//Bullet *b;
//Enemy e; //dirkD


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
    uninitSounds();
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
    glGenTextures(1, &g.backTexture);
    glGenTextures(1, &g.walkTexture);
    glGenTextures(1, &g.alienTexture);
    //glGenTextures(1, &e.alienTexture); //dirkD
    glGenTextures(1, &g.mainMenuTexture);
    glGenTextures(1, &g.pointerTexture);
    glGenTextures(1, &g.tutorialTexture);
    glGenTextures(1, &g.creditsTexture);
    //-------------------------------------------------------------------------
    //silhouette
    //this is similar to a sprite graphic
    //
/*
glBindTexture(GL_TEXTURE_2D, g.backTexture);
        unsigned char *backData = buildAlphaData(&img[7]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, backData);

     g.xc[0] = 0.0;
     g.xc[1] = 0.25;
     g.yc[0] = 0.0;
     g.yc[1] = 1.0;

  */


    glBindTexture(GL_TEXTURE_2D, g.walkTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *walkData = buildAlphaData(&img[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, walkData);




    glBindTexture(GL_TEXTURE_2D, g.alienTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *alienData = buildAlphaData(&img[6]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, alienData);

/*
    glBindTexture(GL_TEXTURE_2D, e.alienTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *alienData2 = buildAlphaData(&img[6]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
      GL_RGBA, GL_UNSIGNED_BYTE, alienData2);
*/



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

    glBindTexture(GL_TEXTURE_2D, g.tutorialTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *tutorialData = buildAlphaData(&img[4]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, tutorialData);

    glBindTexture(GL_TEXTURE_2D, g.creditsTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *creditsData = buildAlphaData(&img[5]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, creditsData);

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

 	w = img[7].width;
	h = img[7].height;

	glBindTexture(GL_TEXTURE_2D, g.backTexture);
        unsigned char *backData = buildAlphaData(&img[7]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, backData);

     g.xc[0] = 0.0;
     g.xc[1] = 0.25;
     g.yc[0] = 0.0;
     g.yc[1] = 1.0;

    //free(walkData);
    //unlink("./images/walk.ppm");
    //-------------------------------------------------------------------------
}

void init() {
    initSounds();
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
	if (key == XK_Escape)
	    exit (1);
	if (key == XK_Return) {
	    if (menuPosition == 1) {
		inMainMenu = false;
		inGame = true;
        // Start game timer
        timers.recordTime(&timers.gameTime);
	    } else if (menuPosition == 2) {
		inMainMenu = false;
		inTutorial = true;
	    } else if(menuPosition == 3) {
		inMainMenu = false;
		inCredits = true;
	    } else if(menuPosition == 4) {
		exit (1);
	    }
	} else if (key == XK_Down || key == XK_s) {
	    menuPosition++;
	} else if (key == XK_Up || key == XK_w) {
	    menuPosition--;
	}
	if(menuPosition > 4)
	    menuPosition = 1;
	if(menuPosition < 1)
	    menuPosition = 4;
    }
    if (inTutorial) {
	if (key == XK_Escape) {
	    inTutorial = false;
	    inMainMenu = true;
	}
    }
    if (inCredits) {
	if (key == XK_Escape) {
	    inCredits = false;
	    inMainMenu = true;
	}
    }
    if(inGame) {

	switch (key) {
	    case XK_a:
		//for(int i =0; i < 300 ; i++)
    makeSoundTest();
    if(g.magazine >= 1) {
      ShootBullets(g,b,timers);
      --g.magazine;
      }
		break;
      case XK_r:
        g.magazine = 7;
        makeReloadSound();
  		break;
	    case XK_w:
		timers.recordTime(&timers.walkTime);
		g.walk ^= 1;
		break;
	    case XK_Left:
		moveBack(g);
		detectCharWallColission(g.xres, g.yres,g);
		flip = true;
		//g.xres -= 5;
		//extern void moveForward(Global&);
		//moveForward(g);
		break;
	    case XK_Right:
		moveForward(g);
		detectCharWallColission(g.xres, g.yres,g);
		flip = false;
		break;
	    case XK_Up:
		//g.yres += 10;
		//moveup;
		//g.walkFrame += 5;
		//extern void movecharUp(Global&);
		movecharUp(g);
		detectCharWallColission(g.xres, g.yres,g);
		break;
	    case XK_Down:
		//extern void movecharDown(Global&);
		movecharDown(g);
		detectCharWallColission(g.xres, g.yres,g);
		//g.yres -= 10;
		break;
	    case XK_equal:
		g.delay -= 0.005;
		if (g.delay < 0.005)
		    g.delay = 0.005;
	//g.xc[0] = 7000.0;
	//g.xc[1] = 7000.0;

		break;
	    case XK_minus:
		g.delay += 0.005;
		break;
	    case XK_Escape:
		inMainMenu = true;
		inGame = false;
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
	    //++g.eneFrame;
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
	moveEnemy(g);
	EnemyLoop(g);
	spawnEnemy(g,0.9,2.0);

	g.xc[0] += 0.001;
	g.xc[1] += 0.001;
	if ( (g.xres == g.eyres) ||  (g.yres == g.exres))
	{
		cout << "Hit Enemy " << endl;
	}

	for (int i=0; i<20; i++) {
	    g.box[i][0] -= 2.0 * (0.05 / g.delay);
	    if (g.box[i][0] < -10.0)
		g.box[i][0] += g.gxres +10.0;

	}

    }
}


void render(void)
{
    Rect r;
    //Clear the screen
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    float cx = g.xres/14.0;
    float cy = g.yres/2.0;
    if (inMainMenu) {
	renderMainMenu(g.menuxres, g.menuyres, g.mainMenuTexture);
	renderPointer(g.menuxres, g.pointerTexture);
    } else if (inTutorial) {
	renderTutorial(g.menuxres, g.menuyres, g.tutorialTexture);
    } else if (inCredits) {
	renderCredits(g.menuxres, g.menuyres, g.creditsTexture);
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

	//for (int i=0; i<20; i++) {
	    /*
	    glPushMatrix();
	    glTranslated(g.box[i][0],g.box[i][1],g.box[i][2]);
	    glColor3f(0.0, 0.1, 0.0);
	    glBegin(GL_QUADS);
	    glVertex2i( 0,  0);
	    glVertex2i( 0, 30);
	    glVertex2i(20, 30);
	    glVertex2i(20,  0);
	    glEnd();
	    glPopMatrix();
	*/
        //glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 1.0, 1.0);

	glBindTexture(GL_TEXTURE_2D, g.backTexture);
        glBegin(GL_QUADS);
              	glTexCoord2f(g.xc[0], g.yc[1]); glVertex2i(0, 280);
           	glTexCoord2f(g.xc[0], g.yc[0]); glVertex2i(0, g.byres);
          	glTexCoord2f(g.xc[1], g.yc[0]); glVertex2i(g.bxres, g.byres);
        	glTexCoord2f(g.xc[1], g.yc[1]); glVertex2i(g.bxres, 280);
        glEnd();


	//walk frame.
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
	//
       // glEnd();
	//glPopMatrix();
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glDisable(GL_ALPHA_TEST);

	//glPushMatrix();
	//glColor3f(1.0, 1.0, 1.0);
	//glBindTexture(GL_TEXTURE_2D, g.alienTexture);

	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.0f);
	//glColor4ub(255,255,255,255);
	/*
	glBindTexture(GL_TEXTURE_2D, g.backTexture);
        glBegin(GL_QUADS);
                glTexCoord2f(g.xc[0], g.yc[1]); glVertex2i(0, 300);
                glTexCoord2f(g.xc[0], g.yc[0]); glVertex2i(0, g.byres);
                glTexCoord2f(g.xc[1], g.yc[0]); glVertex2i(g.bxres, g.byres);
                glTexCoord2f(g.xc[1], g.yc[1]); glVertex2i(g.bxres, 300);
        glEnd();
        */


	//for(int i = 0; i < 5; i++) {
	for(int i = 0; i < 16; i++) {

        //srand((unsigned int)time(NULL));
	//float res =  ( ((float)rand()/(float)(RAND_MAX)) * .10)+ 0.9;
	spawnEnemy(g,0.9,2.0);
	//if(g.exres < 0.0)
	spawnEnemy(g,0.7,3.5);
        //if(g.exres < 0.0)
	spawnEnemy(g,0.6,6.5);
	//if(g.exres < 0.0)
	spawnEnemy(g,0.5,12.5);

	spawnEnemy(g,0.9,14.5);
        ////////////////////////

  spawnEnemy(g,0.9,3.0);
	//if(g.exres < 0.0)
	spawnEnemy(g,0.7,4.5);
        //if(g.exres < 0.0)
	spawnEnemy(g,0.6,2.5);
	//if(g.exres < 0.0)
	spawnEnemy(g,0.5,14.5);

	spawnEnemy(g,0.9,14.5);
        /////////////////////////////
        //spawnEnemy(g,res,3.0);
	//if(g.exres < 0.0)
	//spawnEnemy(g,res,4.5);
        //if(g.exres < 0.0)
	//spawnEnemy(g,res,2.5);
	//if(g.exres < 0.0)
	//spawnEnemy(g,res,14.5);

	//spawnEnemy(g,res,14.5);


     //srand((unsigned int)time(NULL));

    //float a = 16.0;
    //for (int i=0;i<20;i++)
    //float res =  ( ((float)rand()/(float)(RAND_MAX)) * i)+ 2.0;
    //return res;

	//spawnEnemy(g,0.9,i);
	//if(g.exres < 0.0)
	//spawnEnemy(g,0.7,i);
        //if(g.exres < 0.0)
	//spawnEnemy(g,0.6,i);
	//if(g.exres < 0.0)
	//spawnEnemy(g,0.5,i);

	//spawnEnemy(g,0.9,i);
}
	//if(g.exres == 0.0)
	//spawnEnemy(g,0.9,14.5);

	//spawnEnemy(g,0.9,res4);
	//
	/*
	spawnEnemy(g,0.9,res+2.0);
	spawnEnemy(g,0.9,res1+2.0);
        spawnEnemy(g,0.9,res2+2.0);
	spawnEnemy(g,0.9,res3+2.0);
	spawnEnemy(g,0.9,res4+2.0);
	*/
        /*
	spawnEnemy(g,0.9,res+3.5);
	spawnEnemy(g,0.9,res1+3.5);
        spawnEnemy(g,0.9,res2+3.5);
	spawnEnemy(g,0.9,res3+3.5);
	spawnEnemy(g,0.9,res4+3.5);
	*/
	//DrawBullets
	//extern void Drawbullets(Bullet*, Global&);
	Bullet * b = NULL;
	Drawbullets(b,g);

  renderGameTime();
  renderMagazineCount();
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
