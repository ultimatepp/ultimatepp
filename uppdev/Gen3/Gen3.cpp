#include "gen.h"

void Circuit::Evaluate(byte *value) const
{
	byte newvalue[COUNT];
	for(int i = 0; i < COUNT; i++) {
		const Nand& n = nand[i];
		newvalue[i] = ~(value[n.i1] & value[n.i2]);
	}
	memcpy(value, newvalue, COUNT);
}

void Circuit::SetRandom()
{
	for(int i = 0; i < COUNT; i++) {
		Nand& n = nand[i];
		n.i1 = (word)Random() % COUNT;
		n.i2 = (word)Random() % COUNT;
	}
}

void Breed(Circuit& d, const Circuit& a, const Circuit& b)
{
	for(int i = 0; i < COUNT; i++)
		d.nand[i] = Random() & 1 ? a.nand[i] : b.nand[i];
}

int Compute(const Circuit& c, byte *out, int outcount, const byte *in, int incount, int limit = 100)
{
	byte value[COUNT];
	memset(value, 0, COUNT);
	int w = 0;
	for(int i = 0; i < incount; i++)
		for(int q = 0x80; q; q >>= 1)
			value[w++] = !!(q & in[i]);

	memset(out, 0, outcount);	
	for(int ii = 0; ii < limit; ii++) {
		c.Evaluate(value);
		Buffer<byte> nout(outcount, 0);
		int r = w;
		for(int i = 0; i < outcount; i++) {
			nout[i] = 0;
			for(int q = 0x80; q; q >>= 1)
				if(value[r++])
					nout[i] |= q;
		}
		if(memcmp(out, ~nout, outcount) == 0)
			return ii;
		memcpy(out, ~nout, outcount);
	}
	return limit;
}

int Test(Circuit& c, bool print)
{
	int bad = 0;
	for(int x = 0; x < 8; x++) {
		for(int y = 0; y < 8; y++) {
			byte in[2];
			in[0] = x;
			in[1] = y;
			byte out;
			Compute(c, &out, 1, in, 2);
			if(print)
				LOG((int)x << '+' << (int)y << '=' << (int)out);
			bad += x + y != out;
		}
	}
	return bad;
}

CONSOLE_APP_MAIN
{
	Circuit h;
	for(;;) {
		h.SetRandom();
		int b = Test(h, false);
		DDUMP(b);
		if(b != 63)
			break;
	}
	Test(h, true);
}
