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
void detectBullEnemyColission(Bullet *b, Enemy *e, Global *g)
{
    //cout << "bullet pos x: " << (int)b->pos[0]+5 << endl;
    //cout << "shape x: " << s.centerx << endl;
           if (b->pos[0] > e->pos[0]) {
           Enemy *savee = e->next;
           deleteEnemy(g,e);
           e = savee;
           g->nenemies--;
           }
           if (e == NULL)
       			exit(0);
       		e = e->next;
}
*/
