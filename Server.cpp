#include "Server.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include<netinet/in.h>
#include <arpa/inet.h>

using namespace std;

void play_game(int*);

//Constructors
Server::Server(){}
Server::Server(int i): port(i) {};
	
//Getters
int Server::get_port(){ return port;}
int Server::get_master_sock(){return master_sock;}
	
//Setters
void Server::set_port(int i){port=i;}
void Server::set_master_sock(int i){master_sock=i;}
	
//Members
void Server::acquire_master_sock(int port, int backlog) {
	struct sockaddr_in sin; /* Internet endpoint address */
	memset(&sin, 0, sizeof(sin)); /* Zero out address */
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = port;
	/* Allocate socket */
	master_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (master_sock < 0){
		exit(1);
	}
	/* Bind the socket */
	bind(master_sock, (struct sockaddr *)&sin, sizeof(sin));
	/* Listen on socket */
	listen(master_sock, backlog);
}


void Server::start(){
	int port=50000;
	int s_sock;
	struct sockaddr_un fsin;
	acquire_master_sock(port, 10);
	socklen_t addr_len=sizeof(fsin);
	
	for(;;){
		s_sock=accept(master_sock,(struct sockaddr*)&fsin,&addr_len);
		if (s_sock < 0){
			exit(2);
		}
		if(fork()==0){ //child
			play_game(&s_sock);
			close(s_sock);
			exit(0);
		}
		close(s_sock);
	}
}

void play_game(int* sock){
	char buf[4];
	write(*sock, "Success!", 9);
	read(*sock,buf,4);
}