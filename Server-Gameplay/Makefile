targets = Server Reversi Client

all: $(targets)

Server.o: Server.cpp Server.h
	g++ -g -std=gnu++0x -c Server.cpp

Main.o: Main.cpp Server.h
	g++ -g -std=gnu++0x -c Main.cpp

Server: Server.o Reversi.o Main.o
	g++ -g -std=gnu++0x -lsocket -o Server Main.o Reversi.o Server.o

Reversi.o: Reversi.cpp Reversi.h
	g++ -g -std=gnu++0x -c Reversi.cpp

ReversiMain.o: ReversiMain.cpp Reversi.h
	g++ -g -std=gnu++0x -c ReversiMain.cpp

Reversi: ReversiMain.o Reversi.o
	g++ -g -std=gnu++0x -o Reversi ReversiMain.o Reversi.o

Client: Client.cpp
	g++ -g -std=gnu++0x -lsocket -lnsl -o Client Client.cpp
	
clean:
	rm -f *.o $(targets)
