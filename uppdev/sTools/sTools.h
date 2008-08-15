#ifndef _sTools_h_
#define _sTools_h_

void replaceInto( const char* marca, const char* newText, String& buffer ) ;
String SetQ( const String s ) ;
String Set2Q( const String s ) ;

String         VectorString2String( Vector<String>& v, const char * separador = "\r\n") ;
Vector<String> String2VectorString( const String& str ) ;

int enumCharSet() ;
#endif
