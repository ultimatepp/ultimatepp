#ifndef _NibblePtr_Entropy_h_
#define _NibblePtr_Entropy_h_

#include <Core/Core.h>

using namespace Upp;

struct NibblePtr {
	byte *ptr;
	byte  nibble;
	
	force_inline byte Get() {
		byte b = *ptr & nibble;
		byte r = (b | (b >> 4)) & 0xf;
		ptr += nibble & 1;
		nibble = ~nibble;
		return r;
	}

	force_inline void Put(byte c) {
		if(nibble) {
			*ptr |= c;
			ptr++;
			nibble = false;
		}
		else {
			*ptr = c << 4;
			nibble = true;
		}
	}
	
	void Set(byte *p) { ptr = p; nibble = true; }
};

String FChar(int c);

#define COMPRESS2

String Compress(const char *data, const char *end);


int LZVCompress(byte * in, byte * out, byte * heap, int len, int out_len);
int LZVDecompress (byte const *const in, byte * const out, int ilen, int len);

String LZFCompress(const char *s, int length);

#endif
