/*
 *Author: Joshua C
 */
#include "math.h"
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
#include <cstring>
#define MAXBUTTONS 4

extern bool mainmenu;
extern bool tutorialmenu;
extern bool creditsmenu;
extern bool startgame;
extern int posarray;

typedef struct t_button {
    Rect r;
    char text[32];
    int over;
    int down;
    int click;
    float color[3];
    float dcolor[3];
    unsigned int text_color;
} Button;
Button button[MAXBUTTONS];
int nbuttons=0;

void Pointer(GLuint textid){ 
    //those that are commented out are the values which put the arrow positions in which it will center out for laptop/tutoring 
    int posX,posY;

    if (posarray == 1) {
        //posX = 800;
        posX = 1150;
        posY = 435;
        //posY = 435;
    }
    else if (posarray == 2) {
        //posX = 800; 
        posX = 1150;
        posY = 335; 
        //posY = 335;

    } 
    else if (posarray == 3) {
        //posX = 800;
        posX = 1150;
        posY = 235;
        //posY = 230;

    } 
    else if (posarray  == 4) {
        //posX = 800;
        posX = 1150;
        posY = 135;
        //posY = 130;
    }

    float height = 20.0;
    float width = height*1.666667;
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(posX, posY, 0);
    glBindTexture(GL_TEXTURE_2D, textid);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-width,-height);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-width, height);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( width, height);
    glTexCoord2f(1.0f, 1.0f); glVertex2i( width,-height);
    glEnd();
    glPopMatrix(); 
} 

void InitializedButtons() {
    nbuttons=0;
    //size and position
    button[nbuttons].r.width = 140;
    button[nbuttons].r.height = 60;
    button[nbuttons].r.left = 1200;//850;
    button[nbuttons].r.bot = 400;
    button[nbuttons].r.right =
        button[nbuttons].r.left + button[nbuttons].r.width;
    button[nbuttons].r.top = button[nbuttons].r.bot + button[nbuttons].r.height;
    button[nbuttons].r.centerx =
        (button[nbuttons].r.left + button[nbuttons].r.right) / 2;
    button[nbuttons].r.centery =
        (button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
    strcpy(button[nbuttons].text, "Start Game");
    button[nbuttons].down = 0;
    button[nbuttons].click = 0;
    button[nbuttons].color[0] = 0.4f;
    button[nbuttons].color[1] = 0.4f;
    button[nbuttons].color[2] = 0.7f;
    button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
    button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
    button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
    button[nbuttons].text_color = 0x00ffffff;
    nbuttons++;

    button[nbuttons].r.width = 140;
    button[nbuttons].r.height = 60;
    button[nbuttons].r.left =1200; //850;
    button[nbuttons].r.bot = 300;
    button[nbuttons].r.right =
        button[nbuttons].r.left + button[nbuttons].r.width;
    button[nbuttons].r.top = button[nbuttons].r.bot + button[nbuttons].r.height;
    button[nbuttons].r.centerx =
        (button[nbuttons].r.left + button[nbuttons].r.right) / 2;
    button[nbuttons].r.centery =
        (button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
    strcpy(button[nbuttons].text, "Tutorial");
    button[nbuttons].down = 0;
    button[nbuttons].click = 0;
    button[nbuttons].color[0] = 0.4f;
    button[nbuttons].color[1] = 0.4f;
    button[nbuttons].color[2] = 0.7f;
    button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
    button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
    button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
    button[nbuttons].text_color = 0x00ffffff;
    nbuttons++;

    button[nbuttons].r.width = 140;
    button[nbuttons].r.height = 60;
    button[nbuttons].r.left = 1200;//850;
    button[nbuttons].r.bot = 200;
    button[nbuttons].r.right =
        button[nbuttons].r.left + button[nbuttons].r.width;
    button[nbuttons].r.top = button[nbuttons].r.bot + button[nbuttons].r.height;
    button[nbuttons].r.centerx =
        (button[nbuttons].r.left + button[nbuttons].r.right) / 2;
    button[nbuttons].r.centery =
        (button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
    strcpy(button[nbuttons].text, "Credits");
    button[nbuttons].down = 0;
    button[nbuttons].click = 0;
    button[nbuttons].color[0] = 0.4f;
    button[nbuttons].color[1] = 0.4f;
    button[nbuttons].color[2] = 0.7f;
    button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
    button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
    button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
    button[nbuttons].text_color = 0x00ffffff;
    nbuttons++;

    button[nbuttons].r.width = 140;
    button[nbuttons].r.height = 60;
    button[nbuttons].r.left = 1200;//850;
    button[nbuttons].r.bot = 100;
    button[nbuttons].r.right =
        button[nbuttons].r.left + button[nbuttons].r.width;
    button[nbuttons].r.top = button[nbuttons].r.bot + button[nbuttons].r.height;
    button[nbuttons].r.centerx =
        (button[nbuttons].r.left + button[nbuttons].r.right) / 2;
    button[nbuttons].r.centery =
        (button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
    strcpy(button[nbuttons].text, "Quit");
    button[nbuttons].down = 0;
    button[nbuttons].click = 0;
    button[nbuttons].color[0] = 0.4f;
    button[nbuttons].color[1] = 0.4f;
    button[nbuttons].color[2] = 0.7f;
    button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
    button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
    button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
    button[nbuttons].text_color = 0x00ffffff;
    nbuttons++;
}

void Buttons(int yres){
    Rect r;
    r.bot = yres - 20;
    r.left = 10;
    r.center = 0;

    //added draw all buttons
    for (int i=0; i<nbuttons; i++) {
        if (button[i].over) {
            glColor3f(1.0f, 0.0f, 0.0f);
            //draw a highlight around button
            glLineWidth(3);
            glBegin(GL_LINE_LOOP);
            glVertex2i(button[i].r.left-2,  button[i].r.bot-2);
            glVertex2i(button[i].r.left-2,  button[i].r.top+2);
            glVertex2i(button[i].r.right+2, button[i].r.top+2);
            glVertex2i(button[i].r.right+2, button[i].r.bot-2);
            glVertex2i(button[i].r.left-2,  button[i].r.bot-2);
            glEnd();
            glLineWidth(1);
        }
        if (button[i].down) {
            glColor3fv(button[i].dcolor);
        } else {
            glColor3fv(button[i].color);
        }
        glBegin(GL_QUADS);
        glVertex2i(button[i].r.left,  button[i].r.bot);
        glVertex2i(button[i].r.left,  button[i].r.top);
        glVertex2i(button[i].r.right, button[i].r.top);
        glVertex2i(button[i].r.right, button[i].r.bot);
        glEnd();
        r.left = button[i].r.centerx;
        r.bot  = button[i].r.centery-8;
        r.center = 1;
        if (button[i].down) {
            ggprint16(&r, 0, button[i].text_color, "Pressed!");
        } else {
            ggprint16(&r, 0, button[i].text_color, button[i].text);
        }
    }
}

void MainMenu(int xres,int yres, GLuint textid) {
    int h = yres/2;
    int w = (h*1.9); //h*1.33333)  
    glPushMatrix();
    glTranslatef(xres/2, yres/2, 0);
    glBindTexture(GL_TEXTURE_2D, textid);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-w,-h);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( w, h);
    glTexCoord2f(1.0f, 1.0f); glVertex2i( w,-h);
    glEnd();
    glPopMatrix();
}

void Tutorial(int x, int y,GLuint textid){
    glColor3ub(255,255,255);
    static float angle = 0.0f;
    //changed wid
    static int wid = 100;
    wid += sin(angle) * 10;
    float fx = (float)x;
    float fy = (float)y;
    float a = 0 ;
    a += sin(angle) * 10.0f;
    fx += a;
    //angle += 0.2f;
    glPushMatrix();
    glTranslatef(fx,fy,0);
    glRotatef(a,0,0,1.0);
    glBindTexture(GL_TEXTURE_2D, textid);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(0.0f,0.0f); glVertex2i(-wid,wid);
    glTexCoord2f(1.0f,0.0f); glVertex2i(wid,wid);
    glTexCoord2f(1.0f,1.0f); glVertex2i(wid,-wid);
    glEnd();
    glPopMatrix();

    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = y - 20;
    r.left = x - wid/2 + 500;
    r.center = 0;
    ggprint16(&r, 32, c, "Left Key-Move Left");

    r.bot = y - 100;
    r.left = x - wid/2 + 500;
    r.center = 0;
    ggprint16(&r, 32, c, "Right Key-Move Right");

    r.bot = y;
    r.left = x - wid/2 + 500;
    r.center = 0;
    ggprint16(&r, 50, c, "Controls");


}

void Credits(int xres, int yres, GLuint textid) {
    glBindTexture(GL_TEXTURE_2D, textid);
    glColor3ub(0,0,0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
    glEnd();
}

void JCimage (int x, int y, GLuint textid)
{
    glColor3ub(255,255,255);
    static float angle = 0.0f;
    //changed wid
    static int wid = 100;
    wid += sin(angle) * 10;
    float fx = (float)x;
    float fy = (float)y;
    float a = 0 ;
    a += sin(angle) * 10.0f;
    fx += a;
    //angle += 0.2f;
    glPushMatrix();
    glTranslatef(fx,fy,0);
    glRotatef(a,0,0,1.0);
    glBindTexture(GL_TEXTURE_2D, textid);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(0.0f,0.0f); glVertex2i(-wid,wid);
    glTexCoord2f(1.0f,0.0f); glVertex2i(wid,wid);
    glTexCoord2f(1.0f,1.0f); glVertex2i(wid,-wid);
    glEnd();
    glPopMatrix();

    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = y - 20;
    r.left = x - wid/2 - 250;
    r.center = 0;
    ggprint16(&r, 32, c, "Joshua Chavez");
}




