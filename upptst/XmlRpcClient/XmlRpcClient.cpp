#include <Core/Core.h>
#include <Core/XMLRpc/XMLRpc.h>

using namespace Upp;

namespace Upp { 
	extern bool HttpRequest_Trace__;
}

void Compute( double a, Upp::String arithmeticOperator, double b );

int main() {
	Time serverTime;
	SetDateFormat( "%4d-%02d-%02d" );
	XmlRpcRequest call( "127.0.0.1:1234" );
	if( call("GetServerTime") >> serverTime ) {
		LOG( "Server Time = " << serverTime );
	}
	else {
		LOG( Upp::Format("Error: %s", call.GetError()) );
	}

	Compute( 12, "+", 12 );
	Compute( 12, "*", 12 );
	Compute( 12, "+56", 12 );
	Compute( 12, "/", 0 );

	return 0;
}

void Compute( double a, Upp::String arithmeticOperator, double b ) {
	double result = 0;
	XmlRpcRequest call( "127.0.0.1:1234" );
	if( call("Compute", a, arithmeticOperator, b) >> result ) {
		LOG( Upp::Format("%f %s %f = %f", a, arithmeticOperator, b, result) );
	}
	else {
		LOG( Upp::Format("Error: %s", call.GetError()) );
	}
}
