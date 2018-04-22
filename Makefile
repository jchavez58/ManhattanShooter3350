CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: manhattan-shooter

manhattan-shooter:  omarG.cpp dirkD.cpp marcelF.cpp rainforest.cpp Sprite.cpp
	g++ $(CFLAGS) omarG.cpp dirkD.cpp marcelF.cpp rainforest.cpp Sprite.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o manhattan-shooter

clean:
	rm -f manhattan-shooter
	rm -f *.o
