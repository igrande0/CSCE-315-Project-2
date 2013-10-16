#include "Server.h"
#include "Reversi.h"
#include "AI.h"

#define MAX_MESSAGE 512 //max read size
#define EASY 10
#define MEDIUM 25
#define HARD 50
using namespace std;

// Private Members
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
	
// Members
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
		//cout << "Accepting Connection: " << s_sock << " On port: " << port << '\n';
		if (s_sock < 0){
			exit(2);
		}
		if(fork()==0){ //child
			close(master_sock);
			play_game(s_sock);
		}
		close(s_sock);
	}
}

void Server::play_game(int sock){
	Reversi game;
	AI ai;
	char user_color='u';
	char ai_color='u'; //u for unknown
	char game_type='u'; // ==r for random ==e,m,h for easy/medium/hard ==u for unknown
	string ai_move; //AI will be passed the entire board so it can play the game out as needed and return its current move.
	socket_write(sock,"WELCOME\r\n");
	//cout << "wrote to socket\n";
	string s,upper_s;
	while(true){
		// read incoming command from client
		// socket_write(sock,"> ");
		s=socket_read(sock);
		//cout << "Read String: '" << s << "'\n";
		s=remove_beginning_whitespace(s);
		s=remove_newlines(s);
		//cout << "'" << s << "'\n";
		upper_s=capitalize(s);
		//cout << "Received: '" << upper_s << "'\n";
		if(upper_s=="EXIT")
			break;
		else if(upper_s=="WHITE"){
			if(game.set_current_player('w')) {
				user_color = 'w';
				ai_color = 'b';
				socket_write(sock,"OK\n");
			}
			else
				socket_write(sock,"ILLEGAL\n");
		}
		else if(upper_s=="BLACK"){
			if(game.set_current_player('b')) {
				user_color = 'b';
				ai_color = 'w';
				socket_write(sock,"OK\n");
			}
			else
				socket_write(sock,"ILLEGAL\n");
		}
		else if(upper_s=="DISPLAY") {
			string send_string = "OK\n\n";
			send_string += game.get_state_string();
			socket_write(sock, send_string);
		}
		else if(upper_s	=="EASY" || upper_s=="MEDIUM" || upper_s=="HARD" || upper_s=="RANDOM_GAME")
			if(upper_s=="EASY") game_type='e';
			else if(upper_s=="MEDIUM") game_type=='m';
			else if(upper_s=="HARD") game_type='h';
			else game_type='r';
			socket_write(sock,"OK\n");
		else if(upper_s=="UNDO") {
			//if(game_type=='u' || user_color=='u' || ai_color=='u'){ unknown so don't allow move...}
			if(game.undo())
				socket_write(sock,"OK\n");
			else
				socket_write(sock,"ILLEGAL\n");
		}
		else if(upper_s=="HUMAN-AI"){
			socket_write(sock,"OK\n");
		}
		else if(upper_s.substr(0,5)=="AI-AI"){
			bool error=false;
			int AI_sock;
			string move, opponent_move;
			upper_s = upper_s.substr(6); //This will remove "AI-AI "
			string server = get_word(upper_s);
			int port = atoi(get_word(upper_s).c_str());
			string difficulty1 = get_word(upper_s);
			string difficulty2 = upper_s;
			
			struct sockaddr_in sin;
			memset(&sin,0,sizeof(sin));
			sin.sin_family=AF_INET;
			sin.sin_port=port;
			string host=server;
			if(struct hostent * phe=gethostbyname(host.c_str())){
				memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
			}
			else if ((sin.sin_addr.s_addr=inet_addr(host.c_str())) == INADDR_NONE){
				error=true;
			}
			if(!AI_sock=socket(AF_INET, SOCK_STREAM,0)){
				error=true;
			}
			if(!connect(sock, (struct sockaddr *)&sin, sizeof(sin))){
				error=true;
			}
			string s;
			if(error){
					sock_write(sock,"ILLEGAL\n");
					sock_write(sock, ";Command is valid, but unable to connect with specified server\n");
			}
			else{
				sock_write(sock,"OK\n");
				sock_write(AI_sock,"HUMAN-AI");
				sock_write(sock,"HUMAN-AI\n");
				sock_write(sock,sock_read(AI_sock));
				sock_write(AI_sock,"WHITE");
				sock_write(sock,"WHITE\n");
				sock_write(sock,sock_read(AI_sock));
				sock_write(AI_sock,"HARD");
				sock_write(sock,"HARD\n");
				sock_write(sock,sock_read(AI_sock));
				while(true){
					move=ai.make_move(game,HARD);
					game.make_move(move);
					sock_write(AI_sock,move);
					sock_write(sock,move);
					opponent_move=sock_read(AI_sock);    //Note string parsing will have to be done here.
					/*****************
					Regex for [a-j][1-8]
					might be multiple moves
					in opponent_move so push all
					the regex matches into vector
					and then pop out later.
					*****************/
					game.make_move(opponent_move);
					sock_write(sock,opponent_move);
				}
			}
		}
		else if(s[0]==';')
			;// do nothing?
		else if(game.make_move(s)) {
			string send_string = "OK\n";
			//if(game_type=='u' || user_color=='u' || ai_color=='u'){ unknown so don't allow move...}
			if(!game.is_game_over()) {
				if(game.get_num_moves() > 0) {
					send_string += "\n";
					if(game_type=='e'){
						ai_move=ai.make_move(game,EASY);
						game.make_move(ai_move);
					}
					else if(game_type=='m'){
						ai_move=ai.make_move(game,MEDIUM);
						game.make_move(ai_move);
					}
					else if(game_type=='h'){
						ai_move=ai.make_move(game,HARD);
						game.make_move(ai_move);
					}
					else{
						game.make_random_move();
					}
					send_string += game.get_previous_move() + "\n";
					send_string += game.get_state_string();
					
					while(game.get_current_player() == ai_color){
						send_string += "\n";
						if(game_type=='s'){
							ai_move=ai.make_move(game);
							game.make_move(ai_move);
						}
						else{
							game.make_random_move();
						}
						send_string += game.get_previous_move() + "\n";
						send_string += game.get_state_string();
					}
				}
				else{
					send_string += "\n";
					send_string += game.get_previous_move() + "\n";
					send_string += game.get_state_string();
				}
			}

			socket_write(sock, send_string);
		}
		else{
			socket_write(sock,"ILLEGAL\n");
			//cout << "move is... " << s << '\n';
		}
	}
	close(sock);
	exit(0);
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
	//cout << "READ SIZE: " << size << '\n';
	string s = buf;
	return s;
}
