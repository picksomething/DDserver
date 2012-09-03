
#include "dd.h"

int server_port = 80; 
string server_root = "/var/www";
int setsockoptflag = 1;
int server_sock;

int main(void){
	int connfd;

	server_sock = server_start();

	cout << "Server start running at port " << server_port << endl;
	while( true ){

		connfd = accept( server_sock, NULL, NULL );
		if( connfd < 0 ) 
			err_exit("accept");
		Request rqst( connfd );	
		rqst.getRequest();

		close(connfd);
	}

	close(server_sock);
	return 0;
}

