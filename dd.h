#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#include<netinet/in.h>

#define MAXCONN 88
#define URLBUF 256
#define PATHBUF 1024
#define LINEBUF 1024

/* utils */
void err_exit( const char *s );
int server_start(void);

/* main */
extern int server_port; // 0 stands any port can be ok.
extern char server_root[ PATHBUF ];
extern int setsockoptflag;
extern int server_sock;

/* request */
struct request{
	char method[10];
	char url[ PATHBUF * 10 ];
	char version[10];

	int sock,status;
	char res[ LINEBUF * 10 ];
};
struct request * get_request( int client );
void process_get( struct request *p, int isget );

/* response */
void send_response( struct request *p );
