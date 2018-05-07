CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: manhattan-shooter

manhattan-shooter:  omarG.cpp dirkD.cpp  manhattan-shooter.cpp Sprite.cpp game.h
	g++ $(CFLAGS) omarG.cpp dirkD.cpp  manhattan-shooter.cpp Sprite.cpp game.h libggfonts.a -Wall -Wextra $(LFLAGS) -o manhattan-shooter

clean:
	rm -f manhattan-shooter
	rm -f *.o
