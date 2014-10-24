#if 0 // Now broken after optimization

#include <Core/Core.h>

#include <plugin/lz4/lz4.h>

using namespace Upp;

// Test/Test FoooFooo

#define LLOG(x) LOG(x)
#define LTIMING(x)  // RTIMING(x)


force_inline int Hash(const char *s)
{
//	return Peek32le(s) % 8191;
//	return memhash(s, 4) & 8191;
	dword h = Peek32le(s); return (((h >> 23) - (h >> 15) - (h >> 7) - h));
//	return (2654435761U * Peek32le(s)) & 8191;
}

int Literals;
int LiteralsLen;
int Matches;
int BigMatches;
int BigLiterals;
int FarOffsets;

int LZFCompress(char *out, const char *s, int length)
{
	char *out0 = 0;
	char *end;
	
	char *last_256[256];
	
	int literal_len = 0;
	for(;;) {
		for(;;) {
			p256 = last_256[(byte)*s];
			
			last[(byte)*s] = s;
			s++;
			if(s > end)
				return out - out0;
		}
	}
}

#endif
