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
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <vector>

using namespace std;

typedef double Flt;
typedef double Vec[3];
typedef Flt     Matrix[4][4];


#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)     ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0];  \(c)[1]=(a)[1]-(b)[1]; \(c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;


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
	    unlink(ppmname); FILE *fp;
	    fp = fopen ("filename.txt","w");
	    if (fp!=NULL)
	    {
		fprintf(fp,"Some String\n");
		fclose (fp);
	    }
	}
};


class Timers {
    public:
	double physicsRate;
	double oobillion;
	struct timespec timeStart, timeEnd, timeCurrent;
	struct timespec walkTime;
  struct timespec gameTime;
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
};


class Bullet {
    public:
	Vec pos;
	Vec vel;
	float color[3];
	struct timespec time;
    public:
	Bullet() { }
};

class Enemy : public Bullet {

};


/*doubly linked list for enemies
class Enemy {
  public:
    int gxres;
    int gyres;
    int xres;
    int yres;
    int exres;
    int eyres;
    int eneFrame;
    int walk;
    int walkFrame;
    Vec pos;
    Vec vel;
    GLuint alienTexture;
    struct Enemy *prev;
    struct Enemy *next;

  public:
    Enemy() {
      walk = 0;
      walkFrame = 0;
      exres = 800;
	    eyres = 600;
      xres=800;
	    yres=600;
      gxres = 800;
      gyres=  600;
      prev = NULL;
      next = NULL;
    };
};
*/


const int game_duration = 60; // seconds
const int magazine_capacity = 33; // rounds
const int max_bullets = 200;

class Global {
    public:
  //Enemy *ehead; //dirkD
  std::vector<Enemy*> enemies;
  int done;
	int xres, yres;
	int gxres, gyres;
  int enxres, enyres;
	int walk;
	int walkFrame;
	int eneFrame;
	int centerx;
	int centery;
	int exres;
	int eyres;
	int menuxres;
  int menuyres;
	int bxres;
	int byres;
	Vec pos;
	Vec vel;
	double delay;
	GLuint walkTexture;
	GLuint mainMenuTexture;
	GLuint pointerTexture;
	GLuint tutorialTexture;
	GLuint creditsTexture;
	GLuint alienTexture;
	GLuint backTexture;
	Vec box[20];
	Bullet *barr;
	int nbullets;
	struct timespec bulletTimer;
	char keys[65536];

        float xc[2];
        float yc[2];

  int magazine;

	Global() {
      //ehead = NULL;
      //nenemies = 0;
      done=0;
	    xres=800;
	    yres=600;
      enxres = 0;
      enyres = 0;
	    gxres = 800;
	    gyres=  600;
	    exres = 800;
	    eyres = 600;
	    bxres = 800;
	    byres = 600;
	    menuxres = 800;
	    menuyres = 600;
	    centerx= xres+xres/2;
	    centery= yres+yres/2;
	    walk=0;
	    walkFrame=0;
	    eneFrame=0;
	    delay = 0.1;
	    barr = new Bullet[max_bullets];
	    nbullets = 0;
	    pos[0] = xres;
	    pos[1] = yres;
	    pos[2] = 0.0f;
	    vel[0] = xres;
	    vel[1] = yres;
            magazine = magazine_capacity;
	    for (int i=0; i<20; i++) {
		box[i][0] = rnd() * xres;
		box[i][1] = rnd() * (yres-220) + 220.0;
		box[i][2] = 0.0;
		//clock_gettime(CLOCK_REALTIME, &bulletTimer);
	    }

      //build 10 enemies...
  		/*for (int j=0; j<10; j++) {
  			Enemy *e = new Enemy;
  			//e->pos[0] = (Flt)(rand() % xres);
  			//e->pos[1] = (Flt)(rand() % yres);
  			//e->pos[2] = 0.0f;
  			//e->vel[0] = (Flt)(rnd()*2.0-1.0);
  			//e->vel[1] = (Flt)(rnd()*2.0-1.0);

  			//add to front of linked list
  			e->next = ehead;
  			if (ehead != NULL)
  				ehead->prev = e;
  			ehead = e;
  			++nenemies;
      }*/
	    clock_gettime(CLOCK_REALTIME, &bulletTimer);
	    memset(keys,0, 65536);
	}
	~Global(){
    delete [] barr;
  }
};
