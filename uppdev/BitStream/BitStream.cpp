#include <Core/Core.h>

using namespace Upp;

class BitsOut {
	Stream *stream;

	byte  buffer;
	int   left;
	
	void  Out()                          { stream->Put(buffer); left = 8; buffer = 0; }
	void  Flush()                        { if(left != 8) { Out(); } }

public:
	void Start(Stream& s)                { Finish(); stream = &s; left = 8; buffer = 0; }
	void Finish()                        { if(stream) Flush(); stream = NULL; }
	
	void Put(dword w, int bits);
	void Put(bool bit)                   { Put(bit, 1); }
	
	BitsOut()                            { stream = NULL; }
	BitsOut(Stream& s)                   { Start(s); }
	~BitsOut()                           { Finish(); }
};

void BitsOut::Put(dword w, int bits)
{
	ASSERT(bits >= 0 && bits <= 32);
	while(bits) {
		int q = min(left, bits);
		bits -= q;
		left -= q;
		buffer |= w >> bits << left;
		if(left == 0)
			Out();
	}
}

class BitsIn {
	Stream *stream;

	byte  buffer;
	int   left;
	
	void  In()                           { buffer = stream->Get(); left = 8; }

public:
	void Start(Stream& s)                { stream = &s; left = 0; }
	
	dword Get(int bits);
	bool  Get()                          { return Get(1); }
	
	BitsIn()                             { stream = NULL; }
	BitsIn(Stream& s)                    { Start(s); }
};

dword BitsIn::Get(int bits)
{
	dword result = 0;
	while(bits) {
		if(left == 0)
			In();
		int q = min(bits, left);
		left -= q;
		bits -= q;
		result = (result << q) | ((buffer >> left) & (0xff << q >> 8));
	}
	return result;
}

CONSOLE_APP_MAIN
{
	StringStream ss;
	BitsOut out;
	out.Start(ss);
	out.Put(0x123, 12);
	out.Put(1234, 12);
	out.Put(12345, 15);
	out.Put(4, 3);
	out.Put(1);
	out.Put(0);
	out.Put(1);
	out.Put(1);
	out.Finish();
	ss.Seek(0);
	BitsIn in(ss);
	DUMP(in.Get(12));
	DUMP(in.Get(12));
	DUMP(in.Get(15));
	DUMP(in.Get(3));
}
