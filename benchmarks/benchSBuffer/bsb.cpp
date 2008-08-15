#include "bsb.h"

String BenchStringBuffer(int n)
{
	StringBuffer b;
	for(int i = 0; i < n; i++)
		b.Cat(i);
	return b;
}

String BenchString(int n)
{
	String b;
	for(int i = 0; i < n; i++)
		b.Cat(i);
	return b;
}
