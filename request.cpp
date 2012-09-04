
#include"dd.h"
#include"request.h"

Request::Request( int s, int status ):
	sock(s),status(status){}

void Request::getRequest( void ){
	char buf[ LINEBUF * 10 ], *at;
	size_t start, end, n;
	size_t keystart, keyend, valuestart, valueend;
	string key, value;

	for( at = buf; ; at += n ){
		n = read( sock, at, LINEBUF );
		if( n < LINEBUF ) break;
	}
	at += n;

	msg.assign( buf, at );
	//提取METHOD
	start = end = 0;
	end = msg.find( ' ' , start );
	method = msg.substr( start, end - start );
	
	//提取url
	start = end + 1;
	end = msg.find( ' ', start );
	url = msg.substr( start, end - start );

	//提取HTTP版本
	start = end + 1;
	end = msg.find( '\r', start );
	version = msg.substr( start, end - start );

	keystart = end+2;
	while( msg.substr( keystart, 2 ) != "\r\n" ){

		keyend     = msg.find_first_of(": ", keystart) ;
		valuestart = msg.find_first_not_of( ": ", keyend );
		valueend   = msg.find_first_of("\r\n", valuestart );

		key = msg.substr( keystart,	keyend - keystart );
		value=msg.substr( valuestart, valueend - valuestart );
		if( key == "Host" ) host = value;
		
		keystart = valueend + 2;
	}
	cout <<host << endl;
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
/*
void filesend( int client, char * file ){
	char buf[ LINEBUF ];
	int fd,n;
	fd = open( file, O_RDONLY );
	while( (n=read(fd,buf,LINEBUF)) != 0 )
		write( client, buf, n );
}
*/
void Request::processGet( int isget ){
	string file, filetype;
	struct stat fileinfo;

	file.assign( server_root  );
	file.append( url );
	

	if( lstat( file.c_str(), &fileinfo ) < 0 )
		send_r_404();
	else if( S_ISREG( fileinfo.st_mode )  )
		send_r_200(isget?file:"", (int)fileinfo.st_size );
	else if( S_ISDIR( fileinfo.st_mode ) ){
		if( url[ url.length()-1 ] != '/' )
			send_r_301( url+"/" );
		else{
			url += "/index.html";
			processGet( isget );
		}
	}
}

