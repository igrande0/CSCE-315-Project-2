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
	int port;
	int master_sock;
	string get_word(string&);
	string remove_newlines(string);
	string remove_beginning_whitespace(string);
	string capitalize(string);
	
	public:
	//Constructors
	Server();
	Server(int);
	~Server();
	
	//Getters
	int get_port();
	int get_master_sock();
	
	//Setters
	void set_port(int);
	void set_master_sock(int);
	
	//Members
	void start();
	void acquire_master_sock(int, int);
	
	int socket_write(int,string);
	string socket_read(int);
};