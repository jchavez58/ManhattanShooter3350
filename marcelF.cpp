//****************************************
//author: Marcel Furdui
//****************************************
//author: Marcel Furdui
//****************************************

//Lab 5 - 02/23/2018
#include "fonts.h"
#include <time.h>
#include <cstdio>
#include <AL/al.h>
#include <AL/alut.h>
#include <AL/alc.h>

#include "game.h"

const double physicsRate = 1.0 / 60;
const double oobillion = 1.9 / 1e9;
extern struct timespec timeStart, timeCurrent;

extern Global g;
extern Timers timers;
extern bool inGame;
extern bool inMainMenu;
//extern double timeDiff(struct timespec *start, struct timespec *end);
//extern void timeCopy(struct timespec *dest, struct timespec *source);
using namespace std;

void displayGameName (int x, int y, const char* name) {

	unsigned int c = 0x00ffff44;
	Rect r;
	r.bot = x;
	r.center = y;
    ggprint8b(&r, 16, c, name);
}

void renderMagazineCount() {
		glEnable(GL_TEXTURE_2D);
		unsigned int c = 0x00ffffff;
		if(g.magazine < 3) {
			c = 0x00ffff00;
		}
		Rect r;
		r.bot = 50;
		r.left = 10;
		r.center = 0;
		char buffer[128];
		sprintf(buffer, "%d / 7", g.magazine);
		ggprint16(&r, 16, c, buffer);
}

void renderGameTime() {
  glEnable(GL_TEXTURE_2D);
	timers.recordTime(&timers.timeCurrent);

	// The time that has elapsed
	double elapsed = timers.timeDiff(&timers.gameTime, &timers.timeCurrent);
  // The time remaining
  double time = game_duration - elapsed;

		int minutes = time / 60.0;
		int seconds = time / 60 * 60.0;
		int ms = (time / 60 * 60 - seconds) * 100;
		unsigned int c = 0x00ffffff;
		unsigned int c_red = 0x00ff0000;

		Rect r;
		r.bot = 20;
		r.left = 10;
		r.center = 0;
		char buffer[256];
		sprintf(buffer, "%d:%02d:%2d", minutes, seconds, ms);

		if(time <= 0) {
			// game over
			//go back to main menu
			inGame = false;
			inMainMenu = true;
		}
		if (time < 20 && ms < 25) {
			ggprint16(&r, 16, c_red, buffer);
			return;
		}
		ggprint16(&r, 16, c, buffer);
}

/*void timeCount()
{
	static double t = 0.0;
	struct timespec ftimeStart, ftimeEnd;
	clock_gettime(CLOCK_REALTIME, &ftimeStart);
	int a = 200, b = 300;
	for (int i=0; i<100; i++) {
		a = (b*b*b*a*764743*a*b*a*b);
		b = (a&3) * (b&88);
	}
	clock_gettime(CLOCK_REALTIME, &ftimeEnd);
	t += timeDiff(&ftimeStart, &ftimeEnd);
	unsigned int c = 0x00ffff44;
	Rect r;
	r.bot = 350;
	r.left = 400;
	ggprint8b(&r, 80, c, "Marcel Furdui's TimeFunc: %f", t);
}*/

ALuint alBuffer[6];
ALuint alSource[6];
ALint statel;

extern void initSounds() {
	   printf("AUDIO ....\n");
	#ifdef ENABLE_AUDIO
	    printf("AUDIO ENABLED\n");
	    alutInit(0, NULL);
	    if (alGetError() != AL_NO_ERROR) {
	        printf("ERROR: alutInit()\n");
	    }
	    alGetError();

	    float vec[6] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
	    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	    alListenerfv(AL_ORIENTATION, vec);
	    alListenerf(AL_GAIN, 1.0f);


	    alBuffer[0] = alutCreateBufferFromFile("./sound/M4A1_Single.wav");

	    alBuffer[1] = alutCreateBufferFromFile("./sound/Gun_Cocking_Fast.wav");

	    alBuffer[2] = alutCreateBufferFromFile("./sound/shells_falls.wav");

	    alBuffer[3] = alutCreateBufferFromFile("./sound/shells_falls.wav");

	    alBuffer[4] = alutCreateBufferFromFile("./sound/shotgun.wav");

	    alBuffer[5] = alutCreateBufferFromFile("./sound/shotgun-reload-old_school.wav");


	    alGenSources(12, alSource);

	    alSourcei(alSource[0], AL_BUFFER, alBuffer[0]);
	    alSourcef(alSource[0], AL_GAIN, 1.0f);
	    alSourcef(alSource[0], AL_PITCH, 1.0f);
	    alSourcef(alSource[0], AL_LOOPING, AL_FALSE);
	    if (alGetError() != AL_NO_ERROR) {
	        printf("Audio setup error\n");
	    }
	    alSourcei(alSource[1], AL_BUFFER, alBuffer[1]);
	    alSourcef(alSource[1], AL_GAIN, 1.0f);
	    alSourcef(alSource[1], AL_PITCH, 1.0f);
	    alSourcef(alSource[1], AL_LOOPING, AL_FALSE);
	    if (alGetError() != AL_NO_ERROR) {
	        printf("Audio setup error\n");
	    }
	    alSourcei(alSource[2], AL_BUFFER, alBuffer[2]);
	    alSourcef(alSource[2], AL_GAIN, 0.5f);
	    alSourcef(alSource[2], AL_PITCH, 1.0f);
	    alSourcef(alSource[2], AL_LOOPING, AL_FALSE);
	    if (alGetError() != AL_NO_ERROR) {
	        printf("Audio setup error\n");
	    }
	    alSourcei(alSource[3], AL_BUFFER, alBuffer[3]);
	    alSourcef(alSource[3], AL_GAIN, 1.0f);
	    alSourcef(alSource[3], AL_PITCH, 1.1f);
	    alSourcef(alSource[3], AL_LOOPING, AL_FALSE);
	    if (alGetError() != AL_NO_ERROR) {
	        printf("Audio setup error\n");
	    }
	    alSourcei(alSource[4], AL_BUFFER, alBuffer[4]);
	    alSourcef(alSource[4], AL_GAIN, 1.0f);
	    alSourcef(alSource[4], AL_PITCH, 1.0f);
	    alSourcef(alSource[4], AL_LOOPING, AL_FALSE);
	    if (alGetError() != AL_NO_ERROR) {
	        printf("Audio setup error\n");
	    }
	    alSourcei(alSource[5], AL_BUFFER, alBuffer[5]);
	    alSourcef(alSource[5], AL_GAIN, 1.0f);
	    alSourcef(alSource[5], AL_PITCH, 1.0f);
	    alSourcef(alSource[5], AL_LOOPING, AL_FALSE);
	    if (alGetError() != AL_NO_ERROR) {
	        printf("Audio setup error\n");
	    }
#endif
}

extern void makeReloadSound()
{
	#ifdef ENABLE_AUDIO
		alSourcePlay(alSource[5]);
		printf("Reloading...\n");
	#endif
}
//function that is called for select.wav
extern void makeSoundTest()
{
	printf("Magazine is...%d\n", g.magazine);
#ifdef ENABLE_AUDIO
		if(g.magazine == 0) {
  		alSourcePlay(alSource[1]);
			printf("Magazine is empty...%d\n", g.magazine);
		} else {
			printf("Shooting!...\n");
			alSourcePlay(alSource[0]);
			alSourcePlay(alSource[3]);
		}
#endif
}

void playSelection()
{
#ifdef ENABLE_AUDIO

#endif
}

void uninitSounds()
{
#ifdef ENABLE_AUDIO
    alDeleteSources(1, &alSource[0]);
    alDeleteSources(1, &alSource[1]);
    alDeleteSources(1, &alSource[2]);
    alDeleteSources(1, &alSource[3]);
    alDeleteSources(1, &alSource[4]);
    alDeleteSources(1, &alSource[5]);
    alDeleteBuffers(1, &alBuffer[0]);
    alDeleteBuffers(1, &alBuffer[1]);
    alDeleteBuffers(1, &alBuffer[2]);
    alDeleteBuffers(1, &alBuffer[3]);
    alDeleteBuffers(1, &alBuffer[4]);
    alDeleteBuffers(1, &alBuffer[5]);
    ALCcontext *Context = alcGetCurrentContext();
    ALCdevice *Device = alcGetContextsDevice(Context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(Context);
    alcCloseDevice(Device);
#endif
}
