#include<iostream>
#include<string>
#include<cstdio>
#include<cstring>
#include<cstdlib>

#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#include<netinet/in.h>

#define MAXCONN 88
#define URLBUF 256
#define PATHBUF 1024
#define LINEBUF 1024
#define HOSTBUF 32

/* utils */
void err_exit( const char *s );
int server_start(void);

/* main */
extern int server_port;
extern char server_root[ PATHBUF ];
extern int setsockoptflag;
extern int server_sock;

/* request */
class request{
	string method;
	string url;
	string version;
	string host;
	int sock,status;
};
struct request * get_request( int client );
void process_get( struct request *p, int isget );

/* response */
void send_response( struct request *p );
