#include "dd.h"

void err_exit(const char *s ){
	perror(s);
	exit(1);
}


int server_start( void ){
	int fd;
	struct sockaddr_in servaddr;

	fd = socket( AF_INET, SOCK_STREAM, 0 );	
	if( fd == -1 )
		err_exit("Can't get socket");

	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &setsockoptflag, sizeof(setsockoptflag));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(server_port);
	servaddr.sin_addr.s_addr = htonl( INADDR_ANY );

	if( bind( fd, (struct sockaddr *)&servaddr, sizeof(servaddr) ) < 0 )
		err_exit("Bind error");
	if( listen( fd, MAXCONN ) < 0 )
		err_exit("Can't listen");
	return fd;
}
