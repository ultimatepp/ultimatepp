#include "BufferStreamTest.h"



CONSOLE_APP_MAIN
{
	Vector<int> vi;
	Array<int> ai;
	
	VectorMap<unsigned, int> vmi;
	ArrayMap<unsigned, int> ami;
	
	Index<int> vii;
	ArrayIndex<int> aii;
	
	int & iv = vi.Add(123);
	int & ia = ai.Add(123);
	
	int & imv = vmi.Add(0, 123);
	int & ima = ami.Add(0, 123);
	
	int & iiv = vii.Add(123);
	int & iia = aii.Add(123);
	

	BufferStream vs;

	vs % String("Hallo");
	for(int i = 0; i < 1000; i++)
		vs % i;
	
	Vector<byte> vb;
	vb = vs.GetResult();
	
	BufferStream vsd(vb);
	
	String s;
	vsd % s;
	ASSERT(s == "Hallo");
	
	for(int i = 0; i < 1000; i++)
	{
		int ii;
		vsd % ii;
		ASSERT(ii == i);		
		int a = 0;	
	}
	
	int abc = 123;
}

