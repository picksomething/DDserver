
#include "dd.h"

void err_exit(const char *s ){
	perror(s);
	exit(1);
}

void get_config( int argc, char *argv[]){
	int opt;
	while( (opt=getopt(argc,argv,"p:r:h")) != -1 ){
		switch( opt ){
			case 'r': 
				server_root.assign( optarg, optarg+strlen(optarg) );
				break;
			case 'p':
				sscanf( optarg, "%d", &server_port );
				break;
			case 'h':
			default:
				puts("Usage: dd [-p port] [-r docroot] [-h]");
				exit(0);
		}
	}
	
}

int Accept( int fd, struct sockaddr *sa, socklen_t * p ){
	int client;
	while( true ){
		client = accept( fd, sa, p );
		if( client < 0 ){
			if( errno == ECONNABORTED || errno == EINTR )
				continue;
			else
				err_exit("accept failed");
		}
		return client;
	}
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
