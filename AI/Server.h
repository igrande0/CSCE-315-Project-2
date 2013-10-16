#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include<netinet/in.h>
#include <arpa/inet.h>

using namespace std;

class Server{
private:
	int port;
	int master_sock;
	string get_word(string&);
	string remove_newlines(string);
	string remove_beginning_whitespace(string);
	string capitalize(string);
	
public:
	//Constructors
	Server() {}
	Server(int i): port(i) {}
	~Server() {close(master_sock);}
	
	//Getters
	int get_port(){ return port;}
	int get_master_sock() {return master_sock;}
	
	//Setters
	void set_port(int i) {port=i;}
	void set_master_sock(int i) {master_sock=i;}
	
	//Members
	void start();
	void play_game(int sock);
	void acquire_master_sock(int, int);
	
	int socket_write(int,string);
	string socket_read(int);
};