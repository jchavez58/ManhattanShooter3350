#
# Makefile for personal project source file
#
# Enter one of the following
#
# make
# make all
# make dirkD
# make clean
#
CFLAGS = -I ./include
#LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm
LFLAGS = -lrt -lX11 -lGL

all: dirkD

dirkD: asteroids.cpp dirkD.cpp log.cpp timers.cpp
	g++ $(CFLAGS) dirkD.cpp asteroids.cpp log.cpp timers.cpp libggfonts.a -Wall $(LFLAGS) -o dirkD

clean:
	rm -f dirkD
	rm -f *.o

