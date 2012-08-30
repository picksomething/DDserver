#include "dd.h"

int server_port = 80; 
char server_root[ PATHBUF ] = "/var/www";
int setsockoptflag = 1;
int server_sock;

int main(void){
	int connfd;
	struct request *p = NULL;

	server_sock = server_start();
	printf("Server start running at port %d :)\n",server_port);
	while( 1  ){
		
		connfd = accept( server_sock, NULL, NULL );
		if( connfd < 0 ) 
			err_exit("accept");
		p = get_request(connfd);

		if( p->status != 0 )
			send_response( p  );
		else if( strcmp(p->method,"GET") == 0 )
			process_get( p, 1 ); /* 1:need to send entity */
		else if( strcmp(p->method,"HEAD") == 0 )
			process_get(p, 0);	 /* 0:needn't to send */
		else {
			p->status = 501; /* Method not allowed */
			send_response( p ); 
		}

		close(connfd);
	}
	
	close(server_sock);
	return 0;
}

