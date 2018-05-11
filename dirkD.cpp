/*********************************/
/*

Author: Dirk Duclos
Last Modified: 5/10/18

Tasks Assigned:
  1. Colission detection
  2. Basic AI for mafia

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
//#include "Sprite.cpp"
#include "game.h"
using namespace std;

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

#define PI 3.141592653589793
#define ALPHA 1
const float GRAVITY =  -0.2f;

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


/*void deleteEnemy(Global *g, Enemy *node)
{
	//Remove a node from doubly-linked list.
	//Must look at 4 special cases below.
	if (node->prev == NULL) {
		if (node->next == NULL) {
			//only 1 item in list.
			g->ehead = NULL;
		} else {
			//at beginning of list.
			node->next->prev = NULL;
			g->ehead = node->next;
		}
	} else {
		if (node->next == NULL) {
			//at end of list.
			node->prev->next = NULL;
		} else {
			//in middle of list.
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
	}
	delete node;
	node = NULL;
}

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

/*void physicsSpawnEnemy() {
    timers.recordTime(&timers.timeCurrent);
    // The time that has elapsed
    double elapsed = timers.timeDiff(&timers.gameTime, &timers.timeCurrent);
    float roll = ((float)rand() / RAND_MAX);
    float rollY= ((float)rand() / RAND_MAX);
    //float rollVelocityX = ((float)rand() / RAND_MAX);
    float probabilityOfSpawning = 0.008 + (elapsed / game_duration) * .25;
    if(roll < probabilityOfSpawning) {
        Enemy* en = new Enemy();
        en->pos[0] = g.pos[0] + g.gxres + 100;
        en->pos[1] = 25 + rollY * 300;
        en->vel[0] = -5; //- rollVelocityX * 12.5 + 5;
        g.enemies.push_back(en);
    }
}*/

void calculateCollisionOfBullet(Bullet* b, Global& g, Timers& t)
{
    std::vector<Enemy*> remove;
    cout << "Enemies: " << g.enemies.size() << "\n";
    for (unsigned int i = 0; i < g.enemies.size(); i++)
    {
        Enemy& en = *g.enemies[i];
        double diffX = (b->pos[0] - (en.pos[0] - 30/2.0));
        double diffY = (b->pos[1] - (en.pos[1] - 15/2.0+4));
        double dist = std::sqrt(diffX * diffX + diffY * diffY);
        if(dist < 15) { // experimentally tried value for enemy radius
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
    
    if(!remove.empty() && g.nbullets) {
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
