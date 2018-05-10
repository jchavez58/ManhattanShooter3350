CFLAGS = -I ./include -D ENABLE_AUDIO
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm	-D USE_OPENAL_SOUND \
  	/usr/lib/x86_64-linux-gnu/libopenal.so \
  	/usr/lib/x86_64-linux-gnu/libalut.so

all: manhattan-shooter

manhattan-shooter:  omarG.cpp omarG.h dirkD.cpp marcelF.cpp manhattan-shooter.cpp amirB.cpp Sprite.cpp game.h
	g++ $(CFLAGS) omarG.cpp dirkD.cpp marcelF.cpp manhattan-shooter.cpp amirB.cpp Sprite.cpp game.h libggfonts.a -Wall -Wextra $(LFLAGS) -o manhattan-shooter

clean:
	rm -f manhattan-shooter
	rm -f *.o
