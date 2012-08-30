#include"dd.h"

struct request * get_request( int client ){
	struct request * p = malloc( sizeof(struct request) );
	char buf[ LINEBUF * 10 ], *at;
	int  n, len;
	
	p->sock = client;
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
	/* text */
	if( strcmp( file+i, ".html" ) == 0 ) strcpy( save, "text/html" );
	if( strcmp( file+i, ".c" ) == 0 ) strcpy( save, "text/plain" );
	if( strcmp( file+i, ".css" ) == 0 ) strcpy( save, "text/plain" );
	if( strcmp( file+i, ".htm" ) == 0 ) strcpy( save, "text/html" );
	/* application */
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
	char buf[ 100 ];
	FILE * fp;
	
	fp = fopen( file, "r" );
	while( !feof(fp) ){
		fgets( buf, 100, fp);
		send( client, buf, strlen(buf), 0 );
	}
}

void process_get( struct request *p ){
	char file[ LINEBUF ], filetype[ 50 ];;
	char buf[ LINEBUF ];
	struct stat fileinfo;

	
	strcpy( file, server_root );
	if( p->url[ strlen(p->url)-1 ] == '/' )
		p->url[ strlen(p->url)-1 ] = '\0';
	strcpy( file + strlen(file), p->url );
	

	lstat( file, &fileinfo );
	if( S_ISREG( fileinfo.st_mode )  ){ /* found file */
			
		strcpy( buf, "HTTP/1.1 200 OK\r\nConnection: close\r\n" );
		send( p->sock, buf, strlen(buf), 0 );
		
		sprintf( buf, "Content-Length: %d\r\n", (int)fileinfo.st_size );
		send( p->sock, buf, strlen(buf), 0 );
		sprintf( buf, "Content-type: %s\r\n\r\n",getfiletype(file, filetype));
		send( p->sock, buf, strlen(buf), 0 );
		filesend( p->sock, file );
	}
	else if( S_ISDIR( fileinfo.st_mode ) ){
		strcat( p->url, "/index.html" );
		process_get( p );
		return;
	}
	else{
		p->status = 404;
		send_response( p );
	}

}
void process_head( struct request *p ){
}
