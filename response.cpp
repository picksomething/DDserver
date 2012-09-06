
#include"dd.h"
#include"request.h"

string getfiletype( string  );

void writein( int client, string file ){
	char buf[ LINEBUF ];
	int n;
	ifstream f( file.c_str(), ifstream::binary | ifstream::in );
	while( !f.eof() ){
		f.read( buf, LINEBUF );
		n = f.gcount();
		write( client, buf, n );
	}
}

string inttostring( int n ){
	string r;
	stringstream ss( stringstream::in | stringstream::out );
	ss << n;
	ss >> r;
	return r;
}

// 200 OK 
void Request::send_r_200( string file, int filesize ){
	string res;
	res += "HTTP/1.1 200 OK\r\n";
	res += "Connection: close\r\n";
	res += "Content-Length: "+ inttostring(filesize) +"\r\n";
	res += "Content-Type: " + getfiletype( file );
	res += "\r\n\r\n";
	write( sock, res.c_str(), res.length() );
	if( file != "" ) writein( sock, file );
}

// 404 Not Found
void Request::send_r_404( void ){
	char buf[ LINEBUF ];
	char html[] = "<html><head><title>404 Not Found</title></head>"
				  "<body><h1> 404 Not Found </h1>"
				  "The requested URL is not found on this server!"
				  "</body></html>";

	sprintf( buf, "HTTP/1.1 404 Not Found\r\n"
				  "Connection: close\r\n"
				  "Content-Type: text/html\r\n"
				  "Content-Length: %d\r\n\r\n%s", strlen(html), html );

	write( sock, buf, strlen(buf) );
}

// 414 Request Too Long
void Request::send_r_414( void ){
	char buf[ LINEBUF ];
	char html[] = "<html><head><title>414 Request URI Too Long</title></head>"
				  "<body><h1> 414 Request URI Too Long </h1>"
				  "The requested URL is too long on this server!"
				  "</body></html>";

	sprintf( buf, "HTTP/1.1 414 Request Too Long\r\n"
				  "Connection: close\r\n"
				  "Content-Type: text/html\r\n"
				  "Content-Length: %d\r\n\r\n%s", strlen(html), html );

	write( sock, buf, strlen(buf) );
}

// 408 Request Timeout
void Request::send_r_408( void ){
	char buf[ LINEBUF ];
	char html[] = "<html><head><title>408 Request Timeout</title></head>"
				  "<body><h1> 408 Request Timeout </h1>"
				  "You're TIME OUT!"
				  "</body></html>";

	sprintf( buf, "HTTP/1.1 408 Request Timeout\r\n"
				  "Connection: close\r\n"
				  "Content-Type: text/html\r\n"
				  "Content-Length: %d\r\n\r\n%s", strlen(html), html );
	write( sock, buf, strlen(buf) );
}
// 501 Not Implemented
void Request::send_r_501( void ){
	char buf[ LINEBUF ];
	char html[] = "<html><head><title>501 Not Implemented</title></head>"
				  "<body><h1> 501 Not Implemented </h1>"
				  "We haven't implemented this!"
				  "</body></html>";

	sprintf( buf, "HTTP/1.1 501 Not Implement\r\n"
				  "Connection: close\r\n"
				  "Content-Type: text/html\r\n"
				  "Content-Length: %d\r\n\r\n%s", strlen(html), html );
	write( sock, buf, strlen(buf) );
}

// 505 Http Version Not Supported
void Request::send_r_505( void ){
	char buf[ LINEBUF ];
	char html[] = "<html><head><title>505 Version Not Supported</title></head>"
				  "<body><h1> 505 Version Not Supported </h1>"
				  "HTTP version 1.1 is only supported!"
				  "</body></html>";

	sprintf( buf, "HTTP/1.1 505 Version Not Supported\r\n"
				  "Connection: close\r\n"
				  "Content-Type: text/html\r\n"
				  "Content-Length: %d\r\n\r\n%s", strlen(html), html );
	write( sock, buf, strlen(buf) );
}

// 301 Move Permenatly
void Request::send_r_301( string location ){
	string res;

	res += "HTTP/1.1 301 Move Permanently\r\n";
	res += "Connection: close\r\n";
	res += "Location: http://"+host+location+"\r\n\r\n";
	write( sock, res.c_str(), res.length()  );
}

string getfiletype( string file ){
	size_t pos = file.find_last_of(".");
	if( pos == file.length() )
		return "application/octet-stream";
	string type = file.substr(pos+1);
	// text
	if( type == "html" || type == "htm" ) return "text/html";
	if( type == "css" ) return "text/css";
	if( type == "txt" ) return "text/txt";
	if( type == "c" ) return "text/plain";
	// images
	if( type == "gif" ) return "image/gif";
	if( type == "jpg" ) return "image/jpeg";
	if( type == "bmp" ) return "image/bmp";
	if( type == "ico" ) return "image/x-icon";
	if( type == "png" ) return "image/png";
	// applications
	if( type == "js"  ) return "application/x-javascript";
	if( type == "pdf" ) return "application/pdf";
	if( type == "doc" ) return "application/msword";
	if( type == "ppt" ) return "application/vnd.ms-powerpoint";
	//video & audio 
	if( type == "mp3" ) return "audio/mpeg";
	if( type == "wav" ) return "audio/x-wav";
	if( type == "avi" ) return "vido/x-msvideo";
	if( type == "mov" ) return "video/quicktime";
	if( type == "mpg" ) return "video/mpeg";
	
	return "application/octet-stream";
}

