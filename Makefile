CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: manhattan-shooter

manhattan-shooter: omarG.cpp marcelF.cpp dirkD.cpp manhattan-shooter.cpp
	g++ $(CFLAGS) omarG.cpp marcelF.cpp dirkD.cpp manhattan-shooter.cpp timers.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o manhattan-shooter

clean:
	rm -f manhattan-shooter
	rm -f *.o

