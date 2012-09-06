
#include "dd.h"

int server_port = 8080; 
string server_root = "/var/www";
int setsockoptflag = 1;
int server_sock;

int main( int argc, char *argv[] ){
	int connfd;
	
	get_config(argc,argv);
	set_signals();
	server_sock = server_start();
	cout << "Server start running at port " << server_port << endl;
	while( true ){
		
		connfd = Accept( server_sock, NULL, NULL );
		
		Request rqst( connfd );	
		rqst.handle();

	}

	close(server_sock);
	return 0;
}

