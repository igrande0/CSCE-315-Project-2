all: Server reversi

Server.o: Server.cpp Server.h
	g++ -std=gnu++0x -c Server.cpp

Main.o: Main.cpp Server.h
	g++ -std=gnu++0x -c Main.cpp

Server: Server.o Main.o
	g++ -std=gnu++0x -lsocket -o Server Main.o Server.o

Reversi.o: Reversi.cpp Reversi.h
	g++ -std=gnu++0x -c Reversi.cpp

ReversiMain.o: ReversiMain.cpp Reversi.h
	g++ -std=gnu++0x -c ReversiMain.cpp

reversi: ReversiMain.o Reversi.o
	g++ -std=gnu++0x -o reversi ReversiMain.o Reversi.o

clean:
	rm -f *.o Server reversi
