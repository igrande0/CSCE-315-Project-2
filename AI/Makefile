targets = Server Reversi Client AI

all: $(targets)

AI.o: AI.cpp AI.h Reversi.h
	g++ -g -std=gnu++0x -c AI.cpp
	
AIMain.o: AIMain.cpp AI.h Reversi.h
	g++ -g -std=gnu++0x -c AIMain.cpp
	
Server.o: Server.cpp Server.h
	g++ -g -std=gnu++0x -c Server.cpp

Main.o: Main.cpp Server.h
	g++ -g -std=gnu++0x -c Main.cpp

Reversi.o: Reversi.cpp Reversi.h
	g++ -g -std=gnu++0x -c Reversi.cpp

ReversiMain.o: ReversiMain.cpp Reversi.h
	g++ -g -std=gnu++0x -c ReversiMain.cpp

Server: Server.o Reversi.o AI.o Main.o
	g++ -g -std=gnu++0x -lsocket -lnsl -o Server Main.o AI.o Reversi.o Server.o
	
Reversi: ReversiMain.o Reversi.o AI.o
	g++ -g -std=gnu++0x -o Reversi ReversiMain.o Reversi.o

Client: Client.cpp
	g++ -g -std=gnu++0x -lsocket -lnsl -o Client Client.cpp
	
AI: AI.o Reversi.o AIMain.o
	g++ -g -std=gnu++0x -o AI AI.o Reversi.o AIMain.o 
	
clean:
	rm -f *.o $(targets) core
