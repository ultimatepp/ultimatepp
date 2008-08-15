#include "e011.h"

unsigned int SEP1_ReadVar(unsigned int n) {
	char buff[2], buff1[16];
	unsigned int i, j;

	for(j=0; j<3; j++) {
		port.ClearReadBuffer();
		port.write( STX );
		port.write( 3 );
		port.write( 'R' );
		port.write( 'r' );
		port.write( (char)n );

		i = port.readString(buff, 2);
    if(i == 2) {
    	if(buff[0] == STX) {
		    i = port.readString(buff1, buff[1]);
    		if(i == buff[1]) {
    			i = (unsigned char)buff1[0] * 256;
    			i += (unsigned char)buff1[1];
    		 	return i;
    		}
	  	}
		}
	}
	return 0;
}

unsigned int SEP1_ReadFlash(unsigned int n) {
	char buff[2], buff1[16];
	unsigned int i, j;

	for(j=0; j<3; j++) {
		port.ClearReadBuffer();
		port.write( STX );
		port.write( 3 );
		port.write( 'R' );
		port.write( 'e' );
		port.write( (char)n );

		i = port.readString(buff, 2);
    if(i == 2) {
    	if(buff[0] == STX) {
		    i = port.readString(buff1, buff[1]);
    		if(i == buff[1]) {
    			i = (unsigned char)buff1[0] * 256;
    			i += (unsigned char)buff1[1];
    		 	return i;
    		}
	  	}
		}
	}
	return 0;
}

unsigned int SEP1_ReadFile(unsigned int n) {
	char buff[2], buff1[16];
	unsigned int i, j;

	for(j=0; j<3; j++) {
		port.ClearReadBuffer();
		port.write( STX );
		port.write( 3 );
		port.write( 'R' );
		port.write( '*' );
		port.write( (char)n );

		i = port.readString(buff, 2);
    if(i == 2) {
    	if(buff[0] == STX) {
		    i = port.readString(buff1, buff[1]);
    		if(i == buff[1]) {
    			i = (unsigned char)buff1[0] * 256;
    			i += (unsigned char)buff1[1];
    		 	return i;
    		}
	  	}
		}
	}
	return 0;
}

void SEP1_WriteVar(unsigned int f, unsigned int v) {

	if(port.IsOpen()) {
		port.ClearWriteBuffer();
		port.write( STX );
		port.write( 5 );
		port.write( 'W' );
		port.write( 'r' );
		port.write( (char)f );
		port.write( (char)(v >> 8) );
		port.write( (char)v );
	}
	return;
}

void SEP1_WriteFlash(unsigned int f, unsigned int v) {

	if(port.IsOpen()) {
		port.ClearWriteBuffer();
		port.write( STX );
		port.write( 5 );
		port.write( 'W' );
		port.write( 'e' );
		port.write( (char)f );
		port.write( (char)(v >> 8) );
		port.write( (char)v );
	}
	return;
}

void SEP1_WriteFile(unsigned int f, unsigned int v) {

	if(port.IsOpen()) {
		port.ClearWriteBuffer();
		port.write( STX );
		port.write( 5 );
		port.write( 'W' );
		port.write( '*' );
		port.write( (char)f );
		port.write( (char)(v >> 8) );
		port.write( (char)v );
	}
	return;
}

int SEP1_GetMode(void) {
	int j;

	for(j=0; j<3; j++) {
		port.ClearReadBuffer();
		port.write( STX );
		port.write( 3 );
		port.write( 'R' );
		port.write( 'm' );
		port.write( 0xff );

		char buff[8], buff1[8];
		int i;
		i = port.readString(buff, 2);
    if(i == 2) {
    	if(buff[0] == STX) {
		    i = port.readString(buff1, buff[1]);
    		if(i == buff[1]) {
    			i = (unsigned char)buff1[0] * 256;
    			i += (unsigned char)buff1[1];
    		 	return i;
    		}
	  	}
		}
	}
	return -1;
}

void SEP1_PutMode(int m) {

	if(port.IsOpen()) {
		port.ClearWriteBuffer();
		port.write( STX );
		port.write( 5 );
		port.write( 'W' );
		port.write( 'm' );
		port.write( 0xff );
		port.write( (char)(m >> 8) );
		port.write( (char)m );
	}
	return;
}

void SEP1_EscCmd(const String& s) {

	port.ClearWriteBuffer();
	port.write( STX );
	port.write( s.GetLength()+1 );
	port.write( ESC );
	port.writeString( ~s, s.GetLength() );

	return;
}
