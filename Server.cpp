#include "Server.h"

#define MAX_MESSAGE 512 //max read size
using namespace std;

void play_game(int);
bool make_move(string);

//Constructors
Server::Server(){};
Server::Server(int i): port(i) {};
Server::~Server(){
	close(master_sock);
}
	
//Getters
int Server::get_port(){ return port;}
int Server::get_master_sock(){return master_sock;}
	
//Setters
void Server::set_port(int i){port=i;}
void Server::set_master_sock(int i){master_sock=i;}
	
//Private Members
string Server::remove_beginning_whitespace(string s){
	int i=0;
	while(s[i]==' '){
		i++;
	}
	return s.substr(i);
}

string Server::get_word(string& s){
	int i=0; 
	while(s[i]!=' '){
			i++;
	}
	string return_str=s.substr(0,i);
	s=s.substr(i+1);
	return return_str;
}

string Server::remove_newlines(string s){
	if(s[s.size()-1]=='\n' || s[s.size()-1]=='\r'){
		s=s.substr(0,s.size()-2);
		return remove_newlines(s);
	}
	return s;
}

string Server::capitalize(string s){
	int i=0;
	while(i<s.size()){
		s[i]=toupper(s[i]);
		i++;
	}
	return s;
}
	
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
	int s_sock;
	struct sockaddr_un fsin;
	acquire_master_sock(port, 10);
	socklen_t addr_len=sizeof(fsin);
	
	for(;;){
		s_sock=accept(master_sock,(struct sockaddr*)&fsin,&addr_len);
		cout << "Accepting Connection: " << s_sock << " On port: " << port << '\n';
		if (s_sock < 0){
			exit(2);
		}
		if(fork()==0){ //child
			close(master_sock);
			socket_write(s_sock,"WELCOME\r\n");
			string s,upper_s;
			while(true){
				socket_write(s_sock,"Please Enter a command\n> ");
				s=socket_read(s_sock);
				cout << "Read String: '" << s << "'\n";
				s=remove_beginning_whitespace(s);
				s=remove_newlines(s);
				cout << "'" << s << "'\n";
				upper_s=capitalize(s);
				if(upper_s=="EXIT") break;
				else if(upper_s=="DISPLAY")  socket_write(s_sock,"Displaying board!\n");
				else if(upper_s	=="EASY" || upper_s=="MEDIUM" || upper_s=="HARD") socket_write(s_sock,"Set difficulty!\n");
				else if(upper_s=="UNDO") socket_write(s_sock,"undo\n");
				else if(upper_s=="HUMAN-AI") socket_write(s_sock,"Human game!\n");
				else if(upper_s.substr(0,5)=="AI-AI"){
					upper_s=upper_s.substr(6); //This will remove "AI-AI "
					string server=get_word(upper_s);
					int port= atoi(get_word(upper_s).c_str());
					string difficulty1=get_word(upper_s);
					string difficulty2=upper_s;
					string return_str="AI-AI game on " + server + "on port " + to_string(port) + " as " + difficulty1 + " vs " + difficulty2 + "\n";
					socket_write(s_sock, return_str);
				}
				else if(s[0]==';')  socket_write(s_sock,"comment\n");
				else{
					if(make_move(s)){
						socket_write(s_sock,"Successfully made move!\n");
					}
					else{
						socket_write(s_sock,"Failed to make move!\n");
					}
				}
			}
			close(s_sock);
			exit(0);
		}
		close(s_sock);
	}
}

void play_game(int sock){
	char buff[512];
	write(sock, "Success!\n>", 12);
	int size=read(sock,buff,512);
	//cout << "Received: " << buff;
	write(sock, "Received: ",10);
	write(sock,buff,size);
	write(sock,"Game Play Functionality Verified, Bye!\r\n",42);
}

bool make_move(string s){
	cout << "Making move: " << s <<'\n';
	//if(move(s)) return true;
	//else return false;
	return true;
}

int Server::socket_write(int sock,string msg){
	if (msg.length() >= MAX_MESSAGE) {
		cerr << "Message too long for Channel!\n";
		return -1;
	}
	const char * s = msg.c_str();
	write(sock, s, strlen(s)+1);
	return 0;
}

string Server::socket_read(int sock){
	char buf[MAX_MESSAGE];
	int size=read(sock, buf, MAX_MESSAGE);
	cout << "READ SIZE: " << size << '\n';
	string s = buf;
	return s;
}
