#include <sys/socket.h>

using namespace std;

class Server{
	int port;
	int master_sock;
	
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
};