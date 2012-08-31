#include"dd.h"

static void send_not_found( int client ){
	char buf[ LINEBUF ];
	char html[] = "<html>"
					"<head>"
						"<title>404 Not Found</title>"
					"</head>"
					"<body>"
					"<h1> 404 Not Found </h1>"
					"The requested URL is not found on this server!"
					"</body>"
				 "</html>";

	sprintf( buf, "HTTP/1.1 404 Not Found\r\n"
				  "Connection: close\r\n"
				  "Content-Type: text/html\r\n"
				  "Content-Length: %d\r\n\r\n%s", strlen(html), html );

	send( client, buf, strlen(buf), 0 );
}

void send_move_pmnt(struct request *p ){
	char buf[ LINEBUF ];

	sprintf( buf, "HTTP/1.1 301 Move Permanently\r\n"
				  "Connection: close\r\n"
				  "Location: http://%s%s\r\n\r\n", p->host, p->url );
	send( p->sock, buf, strlen(buf), 0 );
}

void send_response( struct request *p ){
	if( p->status == 404 )
		send_not_found( p->sock );
	if( p->status == 301 )
		send_move_pmnt( p );
}


