#include "Server.h"
#include "Reversi.h"

#define MAX_MESSAGE 512 //max read size
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
		cout << "Accepting Connection: " << s_sock << " On port: " << port << '\n';
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
	socket_write(sock,"WELCOME\r\n");

	string s,upper_s;
	while(true){
		// read incoming command fron client
		socket_write(sock,"> ");
		s=socket_read(sock);
		//cout << "Read String: '" << s << "'\n";
		s=remove_beginning_whitespace(s);
		s=remove_newlines(s);
		//cout << "'" << s << "'\n";
		upper_s=capitalize(s);

		// for now, we ignore all difficulty commands
		if(upper_s=="EXIT")
			break;
		if(upper_s=="WHITE")
			game.set_current_player('w');
		if(upper_s=="BLACK")
			game.set_current_player('b');
		else if(upper_s=="DISPLAY") {
			string send_string = "OK\n\n";
			send_string += game.get_state_string();
			socket_write(sock, send_string);
		}
		/*else if(upper_s	=="EASY" || upper_s=="MEDIUM" || upper_s=="HARD")
			socket_write(sock,"Set difficulty!\n");*/
		else if(upper_s=="UNDO") {
			if(game.undo())
				socket_write(sock,"OK\n");
			else
				socket_write(sock,"ILLEGAL\n");
		}
		/*else if(upper_s=="HUMAN-AI")
			socket_write(sock,"Human game!\n");*/
		/*else if(upper_s.substr(0,5)=="AI-AI"){
			upper_s = upper_s.substr(6); //This will remove "AI-AI "
			string server = get_word(upper_s);
			int port = atoi(get_word(upper_s).c_str());
			string difficulty1 = get_word(upper_s);
			string difficulty2 = upper_s;
			string return_str = "AI-AI game on " + server + "on port "to_string(port);
			return_str += " as " + difficulty1 + " vs " + difficulty2 + "\n";
			socket_write(sock, return_str);
		}*/
		else if(s[0]==';')
			;// do nothing?
		else if(game.make_move(s)) {
			string send_string = "OK\n";

			if(!game.is_game_over()) {
				send_string += "\n";
				game.make_random_move();
				send_string += game.get_previous_move() + "\n";
				send_string += game.get_state_string();
			}

			socket_write(sock, send_string);
		}
		else
			socket_write(sock,"ILLEGAL\n");
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
	cout << "READ SIZE: " << size << '\n';
	string s = buf;
	return s;
}
