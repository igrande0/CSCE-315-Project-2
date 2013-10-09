#include <iostream>
#include <stdlib.h>
#include "Server.h"

using namespace std;

int main(int argc, char * argv[]){
	int port=50000;
	char c;
	while ((c=getopt(argc, argv, "p:")) != -1){  //option handling... coordinator takes arguments
		switch(c){
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
	Server s(port);
	cout << "Starting...\n";
	s.start();
	cout << "Ended...\n";
}