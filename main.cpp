
#include "dd.h"

int server_port = 80; 
string server_root = "/var/www";
int setsockoptflag = 1;
int server_sock;

int main(void){
	int connfd;

	Request *p = NULL;
	server_sock = server_start();

	cout << "Server start running at port " << server_port << endl;
	while( true ){

		connfd = accept( server_sock, NULL, NULL );
		
		if( connfd < 0 ) 
			err_exit("accept");
		p = get_request(connfd);
		/* 针对不同的方法进行不同响应 */
		if( p->status != 0 )
			send_response( p  );
		else if( strcmp(p->method,"GET") == 0 )
			process_get( p, 1 ); /* 1代表传送实体 */
		else if( strcmp(p->method,"HEAD") == 0 )
			process_get(p, 0);	 /* 0代表不传送 */
		else {
			p->status = 501; /* Method not allowed */
			send_response( p ); 
		}

		free(p);/* p在get_request中动态分配内存 */
		close(connfd);
	}
	close(server_sock);
	return 0;
}

