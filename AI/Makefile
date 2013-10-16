targets = Server Reversi Client

all: $(targets)

AI.o: AI.cpp AI.h
	g++ -g -std=gnu++0x -c AI.cpp

Server.o: Server.cpp Server.h
	g++ -g -std=gnu++0x -c Server.cpp

Main.o: Main.cpp Server.h
	g++ -g -std=gnu++0x -c Main.cpp

Server: Server.o Reversi.o AI.o Main.o
	g++ -g -std=gnu++0x -lsocket -o Server Main.o Reversi.o Server.o

Reversi.o: Reversi.cpp Reversi.h
	g++ -g -std=gnu++0x -c Reversi.cpp

ReversiMain.o: ReversiMain.cpp Reversi.h
	g++ -g -std=gnu++0x -c ReversiMain.cpp

Reversi: ReversiMain.o Reversi.o AI.o
	g++ -g -std=gnu++0x -o Reversi ReversiMain.o Reversi.o

Client: Client.cpp
	g++ -g -std=gnu++0x -lsocket -lnsl -o Client Client.cpp
	
clean:
	rm -f *.o $(targets) core
