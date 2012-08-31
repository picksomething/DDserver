#include"dd.h"

struct request * get_request( int client ){
	struct request * p; 
	char buf[ LINEBUF * 10 ], *at, *key, *value;
	int  n, len;
	
	p = (struct request *)malloc( sizeof(struct request) );
	p->sock = client;
	p->status = 0;

	for( at = buf; ; at += n ){
		n = read( client, at, LINEBUF );
		if( n < LINEBUF ) break;
	}
	at += n;
	*at = '\0';

	at = buf;
	for( len = 0; at[len] != ' '; len++ );
	at[len] = '\0';
	strcpy( p->method, at );
	
	at += len+1;
	for( len = 0; at[len] != ' '; len++ );
	at[len] = '\0';
	strcpy( p->url, at );

	at += len+1;
	for( len = 0; at[len] != '\r'; len++ );
	at[len] = '\0';
	strcpy( p->version, at );
	
	key = at+len+2;
	while( *key != '\r' ){
		for( len = 0; key[len] != ':'; len++ );
		key[len] = '\0'; value = key+len+2;
		for( ; key[len] != '\r'; len++ );
		key[len] = '\0';
		if( strcmp(key,"Host") == 0 ){
			strcpy( p->host, value );
		}
		key += len+2;
	}
	/*
		TODO: process various headers.
	*/

	return p;
}

char * getfiletype( char * file, char * save ){
	
	int len; 
	int i;

	len = strlen( file );
	for( i = len - 1; i >= 0 && file[i] != '.' && file[i] != '/'; i-- );
	if( file[i] == '/' ) {
		strcpy( save, "application/download" );
		return save;
	}
	/* images */
	if( strcmp( file+i, ".gif" ) == 0 ) strcpy( save, "image/gif" );
	if( strcmp( file+i, ".jpg" ) == 0 ) strcpy( save, "image/jpeg" );
	if( strcmp( file+i, ".bmp" ) == 0 ) strcpy( save, "image/bmp" );
	if( strcmp( file+i, ".ico" ) == 0 ) strcpy( save, "image/x-icon" );
	if( strcmp( file+i, ".png" ) == 0 ) strcpy( save, "image/png" );
	/* text */
	if( strcmp( file+i, ".html" ) == 0 ) strcpy( save, "text/html" );
	if( strcmp( file+i, ".c" ) == 0 ) strcpy( save, "text/plain" );
	if( strcmp( file+i, ".css" ) == 0 ) strcpy( save, "text/css" );
	if( strcmp( file+i, ".htm" ) == 0 ) strcpy( save, "text/html" );
	/* application */
	if( strcmp( file+i, ".js" ) == 0 ) strcpy( save, "application/x-javascript" );
	if( strcmp( file+i, ".pdf" ) == 0 ) strcpy( save, "application/pdf" );
	if( strcmp( file+i, ".doc" ) == 0 ) strcpy( save, "application/msword" );
	if( strcmp( file+i, ".ppt" ) == 0 ) strcpy( save, "application/vnd.ms-powerpoint");
	/* video & audio*/
	if( strcmp( file+i, ".mp3" ) == 0 ) strcpy( save, "audio/mpeg" );
	if( strcmp( file+i, ".wav" ) == 0 ) strcpy( save, "audio/x-wav" );
	if( strcmp( file+i, ".avi" ) == 0 ) strcpy( save, "video/x-msvideo" );
	if( strcmp( file+i, ".mpg" ) == 0 ) strcpy( save, "video/mpeg" );
	if( strcmp( file+i, ".mov" ) == 0 ) strcpy( save, "video/quicktime" );

	return save;
}

void filesend( int client, char * file ){
	char buf[ LINEBUF ];
	int fd,n;
	fd = open( file, O_RDONLY );
	while( (n=read(fd,buf,LINEBUF)) != 0 )
		write( client, buf, n );
}

void process_get( struct request *p,int isget ){
	char file[ LINEBUF ], filetype[ 50 ];;
	char buf[ LINEBUF ];
	struct stat fileinfo;

	
	strcpy( file, server_root );
	strcpy( file + strlen(file), p->url );
	

	if( lstat( file, &fileinfo ) < 0 ){
		p->status = 404;
		send_response( p );
	}
	else if( S_ISREG( fileinfo.st_mode )  ){ /* found file */
			
		strcpy( buf, "HTTP/1.1 200 OK\r\nConnection: close\r\n" );
		send( p->sock, buf, strlen(buf), 0 );
		sprintf( buf, "Content-Length: %d\r\n",(int) fileinfo.st_size );
		send( p->sock, buf, strlen(buf), 0 );
		sprintf( buf, "Content-Type: %s\r\n\r\n",getfiletype(file, filetype));
		send( p->sock, buf, strlen(buf), 0 );

		if( isget ) filesend( p->sock, file );
	}
	else if( S_ISDIR( fileinfo.st_mode ) ){
		if( p->url[ strlen(p->url)-1 ] != '/' ){
			strcat( p->url, "/" );
			p->status = 301;
			send_response( p );
		}
		else{
			strcat( p->url, "/index.html" );
			process_get( p , isget );
		}
	}
}
