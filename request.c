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


void process_get( struct request *p ){
}
void process_head( struct request *p ){
}
