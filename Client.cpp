#include <iostream>
#include<sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include<signal.h>
#include<vector>
#include <iomanip>
#include <netdb.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <string.h>



#define MAX_MESSAGE 512

using namespace std;

string remove_beginning_whitespace(string);
string remove_newlines(string);
string capitalize(string);

int socket_write(int sock,string msg){
	if (msg.length() >= MAX_MESSAGE) {
		cerr << "Message too long for Channel!\n";
		return -1;
	}
	const char * s = msg.c_str();
	write(sock, s, strlen(s)+1);
	return 0;
}

string socket_read(int sock){
	char buf[MAX_MESSAGE];
	int size=read(sock, buf, MAX_MESSAGE);
	cout << "READ SIZE: " << size << '\n';
	string s = buf;
	return s;
}

int main(int argc, char * argv[]) {
	int c;
	unsigned short port(50000);
	string host_name("localhost");
	while ((c=getopt(argc, argv, "h:p:")) != -1){  //option handling... coordinator takes arguments
		switch(c){
			case 'h':
				host_name=optarg;
				break;
			case 'p':
				port=atoi(optarg);
				break;
			case '?':
				if (optopt == 'b' || optopt == 's') fprintf (stderr, "Option '-%c' requires an argument.\n", optopt);
				else fprintf(stderr, "Unknown option '\\x%x'.\n", optopt);
				return 1;
			default:
				abort ();
		}
	}
	struct sockaddr_in sin;
	memset(&sin,0,sizeof(sin));
	sin.sin_family=AF_INET;
	sin.sin_port=port;
	string host=host_name;
	if(struct hostent * phe=gethostbyname(host.c_str())) memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
	else if ((sin.sin_addr.s_addr=inet_addr(host.c_str())) == INADDR_NONE) cout << "error1";
	int sock=socket(AF_INET, SOCK_STREAM,0);
	connect(sock, (struct sockaddr *)&sin, sizeof(sin));
	string s;
	while(true){
		cout << socket_read(sock) << "\n>";
		getline(cin, s);
		if(socket_write(sock,s) == -1){
			cout << "Connection terminated be server... Exiting\n";
			break;
		}
		if( capitalize(remove_beginning_whitespace(remove_newlines(s)))=="EXIT"){
			cout << "Exiting game!\n";
			close(sock);
			break;
		}
	}
}

string remove_beginning_whitespace(string s){
	int i=0;
	while(s[i]==' '){
		i++;
	}
	return s.substr(i);
}

string remove_newlines(string s){
	if(s[s.size()-1]=='\n' || s[s.size()-1]=='\r'){
		s=s.substr(0,s.size()-2);
		return remove_newlines(s);
	}
	return s;
}

string capitalize(string s){
	int i=0;
	while(i<s.size()){
		s[i]=toupper(s[i]);
		i++;
	}
	return s;
}