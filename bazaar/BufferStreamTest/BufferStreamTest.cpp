#include "BufferStreamTest.h"

CONSOLE_APP_MAIN
{
	BufferStream vs;

	String ss = "Hallo";

	vs % ss;
	for(int i = 0; i < 1000; i++)
		vs % i;
	
	Vector<byte> vb;
	vb = vs.GetResult();
	
	BufferStream vsd(vb);
	
	String s;
	vsd % s;
	ASSERT(s == ss);
	
	for(int i = 0; i < 1000; i++)
	{
		int ii;
		vsd % ii;
		ASSERT(ii == i);		
		int a = 0;	
	}
}

