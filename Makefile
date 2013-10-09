all: Server

Server.o: Server.cpp Server.h
	g++ -c Server.cpp

Main.o: Main.cpp Server.h
	g++ -c Main.cpp

Server: Server.o Main.o
	g++ -lsocket -o Server Main.o Server.o

clean:
	rm -f *.o Server
