#include<iostream>
#include<fstream>
#include<sstream>
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

using namespace std;

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
extern string server_root;
extern int setsockoptflag;
extern int server_sock;

/* request */
#include"request.h"

/* signals */
extern void set_signals( void );
