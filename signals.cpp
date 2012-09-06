
#include"dd.h"
#include<signal.h>

void handle_sigsegv( int dummy ){
	cerr << "Caught SIGSEGV: quiting" << endl;
	exit(1);
	//err_exit("
}

void handle_sigint( int dummy ){
	cerr << "Caught SIGINT: continue " << endl;
//	exit(1);
}

void handle_sigterm( int dummy ){
	cerr << "Caught SIGTERM: exit" << endl;
	exit( 1 );
}

void handle_sigpipe( int dummy ){
	cerr << "Caugit SIGPIPE: exit" << endl;
	exit( 1 );
}


void handle_sigchld( int dummy ){
	cerr << "Caught SIGSEGV: exit" << endl;
	exit( 1 );
}

void handle_sigabrt( int dummy ){
	cerr << "Caught SIGABRT: fuck ,what happend?" << endl;
}

void set_signals( void ){
	struct sigaction oct;
	sigset_t *p;
	
	p = &oct.sa_mask;
	oct.sa_flags = 0;
	sigemptyset( p );
	sigaddset( p, SIGINT );
	sigaddset( p, SIGSEGV );
	sigaddset( p, SIGTERM );
	sigaddset( p, SIGPIPE );
	sigaddset( p, SIGCHLD );
	sigaddset( p, SIGABRT );

	oct.sa_handler = handle_sigint;
	sigaction( SIGINT, &oct, NULL );

	oct.sa_handler = handle_sigsegv;
	sigaction( SIGSEGV, &oct, NULL );

	oct.sa_handler = handle_sigterm;
	sigaction( SIGTERM, &oct, NULL );

	oct.sa_handler = handle_sigpipe;
	sigaction( SIGPIPE, &oct, NULL );

	oct.sa_handler = handle_sigchld;
	sigaction( SIGCHLD, &oct, NULL );

	oct.sa_handler = handle_sigabrt;
	sigaction( SIGABRT, &oct, NULL );
}
