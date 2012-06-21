#include <conio.h>
#include <Core/Core.h>
#include <Core/XMLRpc/XMLRpc.h>

using namespace Upp;

XMLRPC_METHOD( Compute ) {
	double a, b;
	Upp::String arithmeticOperator;
	rpc >> a >> arithmeticOperator >> b;
	LOG( Upp::Format("Request: %nf %s %nf", a, arithmeticOperator, b) );

	if( arithmeticOperator.GetCount() == 1 ) {
		switch( *arithmeticOperator ) {
			case '+': {
				rpc << a + b;
				break;
			}
			case '-': {
				rpc << a - b;
				break;
			}
			case '/': {
				if( b == 0 ) {
					rpc << Upp::ErrorValue("division by zero");
				}
				else {
					rpc << a / b;
				}
				break;
			}
			case '*': {
				rpc << a * b;
				break;
			}
		}
	}
	else {
		rpc << Upp::ErrorValue("unknown operator");
	}
}

XMLRPC_METHOD( GetServerTime ) {
	LOG( "Request: GetServerTime" );
	rpc << Upp::GetSysTime();
}

int main() {
	TcpSocket rpc;
	int port = 1234;
	
	if( !rpc.Listen(port,5) ) {
		return false;
	}

	while( true ) {
		if( _kbhit() ) {
			if( _getch() == 27 ){
				break;
			}
		}

		TcpSocket http;
		http.Timeout(1000);
		if( http.Accept(rpc) ) {
			XmlRpcPerform(http,NULL);
		}
	}

	return 0;
}
