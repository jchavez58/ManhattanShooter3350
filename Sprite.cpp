//Author: Omar Gonzalez
//Date: 03/30/2018
// Fiel contains classes defintions for sprites and characters
//it also contians the defintion of a vector

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <GL/glx.h>

typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0];\
                      (c)[1]=(a)[1]-(b)[1];\
                      (c)[2]=(a)[2]-(b)[2];
class Image1 {

	public:
		int width, height;
		unsigned char *data;
		~Image1() { delete [] data; }
		Image1(const char *fname) {
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
class character {
public:
	Vec pos;
	Vec vel;
};
class Game {
public:
	int done;
	int xres, yres;
	GLuint bossTexture;
	GLuint silhouetteTexture;
  GLuint walkTexture;
	int showCharacter;
	int showBoss;
  //int table;
	int silhouette;
	int trees;

	Game() {
		//logOpen();
		done=0;
		xres=800; //800
		yres=600;  //600
		showCharacter=0;

    //table=1;
		silhouette=1;
	}
	~Game() {
	//logClose();
	}
};
