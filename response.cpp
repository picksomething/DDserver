
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

void Request::send_r_301( string location ){
	string res;

	res += "HTTP/1.1 301 Move Permanently\r\n";
	res += "Connection: close\r\n";
	res += "Location: http://"+host+location+"\r\n\r\n";
	write( sock, res.c_str(), res.length()  );
}

string getfiletype( string file ){
	return "text/html";
}

