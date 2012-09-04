#ifndef REQUEST_H_
#define REQUEST_H_

#include<string>
using namespace std;

class Request{
public:
	int sock,status;
	string method;
	string url;
	string version;
	string host;
	string msg;

	Request( int s, int status = 0);
	void getRequest( void );
	void processGet( int isget = 1 );

	void send_r_200( string file, int filesize );
	void send_r_404( void );
	void send_r_301( string location );
};

#endif
