CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: manhattan-shooter

manhattan-shooter: omarG.cpp
	g++ $(CFLAGS) omarG.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o manhattan-shooter

clean:
	rm -f manhattan-shooter
	rm -f *.o

