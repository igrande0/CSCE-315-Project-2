targets= Server Reversi Client

all: $(targets)

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

Reversi: ReversiMain.o Reversi.o
	g++ -std=gnu++0x -o Reversi ReversiMain.o Reversi.o

Client: Client.cpp
	g++ -std=gnu++0x -lsocket -lnsl -o Client Client.cpp
	
clean:
	rm -f *.o $(targets)
