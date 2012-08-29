#include "dd.h"

int server_port = 80; 
char server_root[ PATHBUF ] = "/var/www";
int setsockoptflag = 1;
int server_sock;

char buf[10000];
char temp[] = "HTTP/1.1 200 OK\r\n\r\nThis page you see";
int main(void){
	int connfd;
	struct request *p = NULL;

	server_sock = server_start();
	printf("Server start running at port 80 :)\n");
	while( 1  ){
		
		connfd = accept( server_sock, NULL, NULL );
		p = get_request(connfd);
		if( p->status != 0 )
			send_response( connfd, p->status );
		if( strcmp(p->method,"GET") == 0 )
			process_get(p);
		else if( strcmp(p->method,"HEAD") == 0 )
			process_head(p);
		else 
			send_response( connfd, 0 ); // NOT IMPLEMENT

		close(connfd);
	}
	
	close(server_sock);
	return 0;
}

