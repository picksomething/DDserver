
#include"dd.h"
#include"request.h"

Request::Request( int s, int status ):
	sock(s),status(status){}

Request::~Request(){
	close(sock);
}

bool endswith( const char *s , const char *end ){
	if( s == NULL || end == NULL ) return false;
	int lens = strlen(s);
	int lenend = strlen(end);
	if( lenend > lens ) return false;
	return strcmp( s + lens - lenend, end ) == 0 ;
}

bool Request::getRequest( void ){
	char buf[ LINEBUF * 10 ], *at;
	size_t start, end, n;
	size_t keystart, keyend, valuestart, valueend;
	string key, value;
	
	time_t acceptend;
	time_t acceptstart = time(NULL);
	for( at = buf; ; at += n ){
		n = read( sock, at, LINEBUF );
		*(at+n) = '\0';
		acceptend = time(NULL);
		if( acceptend - acceptstart > REQ_TIMEOUT ){
			send_r_408();
			return false;
		}
		if( endswith(buf,"\r\n\r\n") ) break;
	}
	at += n;
	msg.assign( buf, at );

	//提取METHOD
	start = end = 0;
	end = msg.find( ' ' , start );
	method = msg.substr( start, end - start );
	if( method!="GET" && method!="HEAD" ){send_r_501();return false; }

	//提取url
	start = end + 1;
	end = msg.find( ' ', start );
	url = msg.substr( start, end - start );
	if( url.length() > URL_TOO_LONG ){ send_r_414();return false;}

	//提取HTTP版本
	start = end + 1;
	end = msg.find( '\r', start );
	version = msg.substr( start, end - start );
	if( version != "HTTP/1.1" ) { send_r_505();return false;}

//	cout << msg << endl;

	keystart = end+2;
	while( msg.substr( keystart ) != "\r\n" ){

		keyend     = msg.find_first_of(": ", keystart) ;
		valuestart = msg.find_first_not_of( ": ", keyend );
		valueend   = msg.find_first_of("\r\n", valuestart );

		key = msg.substr( keystart,	keyend - keystart );
		value=msg.substr( valuestart, valueend - valuestart );
		if( key == "Host" ) host = value;
		
		keystart = valueend + 2;
	}
	return true;
}

void Request::processGet( bool isget ){
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

void Request::handle( void ){
	if( getRequest() ){
		if( method == "GET" )
			processGet( true );
		if( method == "HEAD" ) 
			processGet( false);
	}
}
