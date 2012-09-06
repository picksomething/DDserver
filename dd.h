#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<ctime>

#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#include<netinet/in.h>

using namespace std;

#define MAXCONN 88
#define URL_TOO_LONG 256
#define LINEBUF 1024
#define REQ_TIMEOUT 5

/* utils */
void err_exit( const char *s );
int server_start(void);
int Accept( int, struct sockaddr *, socklen_t *);
void get_config( int , char *[] );

/* main */
extern int server_port;
extern string server_root;
extern int setsockoptflag;
extern int server_sock;

/* request */
#include"request.h"

/* signals */
extern void set_signals( void );
