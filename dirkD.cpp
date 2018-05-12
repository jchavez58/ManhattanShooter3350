/*********************************/
/*

Author: Dirk Duclos
Last Modified: 5/10/18

Tasks Assigned:
  1. Colission detection

*/
/*********************************/

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
#include <iostream>
#include <cstdio>
#include <AL/al.h>
#include <AL/alut.h>
#include <AL/alc.h>
#include <vector>
#include <algorithm>
#include "game.h"
using namespace std;

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];
#define PI 3.141592653589793
#define ALPHA 1
const float GRAVITY =  -0.2f;

//Extern vars
extern Global g;
extern Timers timers;

//functions defs
void spawnEnemy(Global&,const float, const float);
void detectCharWallColission(int,int,Global&);
void calculateCollisionOfBullet(Bullet*,Global&);
bool detectBullEnemyColission(Global&);
void drawBox(int,int,int,Global&);
void EnemyWaves(Global&);

//enemy health counters for each wave
int health1 = 0;
int health2 = 0;
int health3 = 0;
int health4 = 0;
int health5 = 0;
int health6 = 0;
int health7 = 0;
int health8 = 0;
int health9 = 0;
int health10 = 0;

//Hit detection for each wave of enemies
bool hit1 = false;
bool hit2 = false;
bool hit3 = false;
bool hit4 = false;
bool hit5 = false;
bool hit6 = false;
bool hit7 = false;
bool hit8 = false;
bool hit9 = false;
bool hit10 = false;

//kill counter
int killCounter = 0;

//Check for character colission with window edges
void detectCharWallColission(int posx, int posy, Global &g)
{
    if (posx <= 100 && posy >= 620) {
        g.xres = 100;
        g.yres = 620;
    }
    else if (posx <= 100 && posy <= 80) {
        g.xres = 100;
        g.yres = 80;
    }
    else if (posx <= 100) {
        g.xres = 100;
    }
    else if (posx >= 11000 && posy >= 620) {
        g.xres = 11000;
        g.yres = 620;
    }
    else if (posx >= 11000 && posy <= 80) {
        g.xres = 11000;
        g.yres = 80;
    }
    else if (posx >= 11000) {
        g.xres = 11000;
    }
    else if (posy <= 80) {
        g.yres = 80;
    }
    else if (posy >= 620) {
        g.yres = 620;
    }
}

/*
This is object oriented approach to Colission
deletes object from mem, as well as bullet
*/
void calculateCollisionOfBullet(Bullet* b, Global& g)
{
    std::vector<Enemy*> remove;
    cout << "Enemies: " << g.enemies.size() << "\n";
    for (unsigned int i = 0; i < g.enemies.size(); i++) {
        Enemy& en = *g.enemies[i];
        double diffX = (b->pos[0] - (en.pos[0] - 30/2.0));
        double diffY = (b->pos[1] - (en.pos[1] - 15/2.0+4));
        double dist = std::sqrt(diffX * diffX + diffY * diffY);
        if (dist < 15) { // experimentally tried value for enemy radius
            remove.push_back(&en);
            break; //one bullet one kill
        }
    }

    for (unsigned int i = 0; i < remove.size(); i++) {
        delete remove[i];
        g.enemies.erase(
            std::remove(
                g.enemies.begin(),
                g.enemies.end(),
                remove[i]
            ),
            g.enemies.end()
        );
        killCounter++;
    }

    if (!remove.empty() && g.nbullets) {
        int bulletIndex = b - g.barr;
        //COpy the last bullet into the current bullet
        //and delete the last bullet
        g.barr[bulletIndex] = g.barr[g.nbullets-1];
        g.nbullets--;
    }
}


/*
function to draw box around enemies
this box function also updates pos as
each enemy moves across the screen
*/
void drawBox(int x,int y, int posx, int posy, Global&g)
{
    glPushMatrix();
    glTranslatef(posx, posy, 0);
    g.centerx = posx;
    g.centery = posy;
    g.enxres = x;
    g.enyres = y;

    glColor3f(1.0f, 0.0f, 0.0f); // Let it be blue
    glBegin(GL_QUADS); // 2x2 pixels
    glVertex2i(x-10, y+30);
    glVertex2i(x+10, y+30);
    glVertex2i(x+10, y-30);
    glVertex2i(x-10, y-30);
    glEnd();
    glPopMatrix();
}

/*
Test if bullet hit enemy
This checks colission for graphical approach
This is old code before object approach
*/
bool detectBullEnemyColission(Global &g)
{
    for (int i = 0; i < g.nbullets; i++) {
        if (g.barr[i].pos[0] > (float)g.centerx-10 && g.barr[i].pos[0] < (float)g.centerx+10 &&
            g.barr[i].pos[1] > (float)g.centery-30 && g.barr[i].pos[1] < (float)g.centery+30) {
                return true;
        }
    }
    return false;
}

/*
function to spawn enemies for graphical approach
this is old function, drawEnemy is now used
*/
void spawnEnemy(Global&g, const float posx, const float posy)
{
    int cx = g.exres/posx;
    int cy = g.eyres/posy;

    g.centerx = cx;
    g.centery = cy;

    extern void drawBox(int,int,int,int, Global&);
    drawBox(posx,posy,cx,cy,g);

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
    glTexCoord2f(tx+.240, ty+1.0); glVertex2i(cx-w, cy-h); //cy-h;

    glEnd();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}

/*
function to spawn enemies to kill
This is rendering(graphical) approach to colission detection
*/
void EnemyWaves (Global &g) {

    if (health1 <=2 || health2 <=2 || health3 <=2 ||
        health4 <=2 || health5 <=2 || health6 <=2 ||
        health7 <=2 || health8 <=2 || health9 <=2 ||
        health10<=2) {

    if (health1 <= 2) {
        spawnEnemy(g,0.1,2.5);
        hit1 = detectBullEnemyColission(g);
        if (hit1) {
            health1++;
        }
    }

    if (health2 <= 2) {
        spawnEnemy(g,0.2,3.5);
        hit2 = detectBullEnemyColission(g);
        if (hit2) {
            health2++;
        }
    }

    if (health3 <= 2) {
        spawnEnemy(g,0.3,6.5);
        hit3 = detectBullEnemyColission(g);
        if (hit3) {
            health3++;
        }
    }

    if (health4 <= 2) {
        spawnEnemy(g,0.4,4.5);
        hit4 = detectBullEnemyColission(g);
        if (hit4) {
            health4++;
        }
    }

    if (health5 <= 2) {
        spawnEnemy(g,0.5,9.5);
        hit5 = detectBullEnemyColission(g);
        if (hit5) {
           health5++;
        }
    }

    if (health6 <= 2) {
        spawnEnemy(g,0.6,7.5);
        hit6 = detectBullEnemyColission(g);
        if (hit6) {
            health6++;
        }
    }

    if (health7 <= 2) {
        spawnEnemy(g,0.7,10.5);
        hit7 = detectBullEnemyColission(g);
        if(hit7) {
            health7++;
        }
    }

    if (health8 <= 2) {
        spawnEnemy(g,0.8,4.5);
        hit8 = detectBullEnemyColission(g);
        if (hit8) {
            health8++;
        }
    }

    if (health9 <= 2) {
        spawnEnemy(g,0.9,6.5);
        hit9 = detectBullEnemyColission(g);
        if (hit9) {
            health9++;
        }
    }

    if (health10 <= 2) {
        spawnEnemy(g,0.9,3.0);
        hit10 = detectBullEnemyColission(g);
        if (hit10) {
            health10++;
        }
    }

    } else {
        //enemy health counters for each wave
        health1 = 0;
        health2 = 0;
        health3 = 0;
        health4 = 0;
        health5 = 0;
        health6 = 0;
        health7 = 0;
        health8 = 0;
        health9 = 0;
        health10 = 0;
        g.exres = 800;
        g.eyres = 600;
    }

return;
}
