#if 0

#include <Core/Core.h>

#include <plugin/lz4/lz4.h>

using namespace Upp;

// Test/Test FoooFooo

#define LLOG(x)

int Hash(const char *s)
{
//	return Peek32le(s) % 8191;
//	return memhash(s, 4) & 8191;
	dword h = Peek32le(s); return (((h >> 23) - (h >> 15) - (h >> 7) - h)) & 8191;
//	return (2654435761U * Peek32le(s)) & 8191;
}

int Literals;
int LiteralsLen;
int Matches;
int BigMatches;
int BigLiterals;
int FarOffsets;

String LZFCompress(const char *s, int length, String& literals)
{
	RTIMING("LZFCompress");

	String out;

	const char *end = s + length;
	
	const char *hash_table[8192];
	
	for(int i = 0; i < 8192; i++)
		hash_table[i] = s;
	
	const char *literal_ptr = s;
	int literal_len = min(length, 4);
	s += literal_len;
	
	for(;;) {
		const char *hptr;
		for(;;) {
			int hash = Hash(s);
			
			hptr = hash_table[hash];

			LLOG(AsCString(hptr, hptr + 4) << ' ' << AsCString(s, s + 4) << ' ' << hash);
	
			if(s - hptr < 0xffff && Peek32le(hash_table[hash]) == Peek32le(s))
				break;
			literal_len++;
			hash_table[Hash(s - 3)] = s - 3;
			s++;
			if(s >= end) {
				word ctrl = (min(literal_len, 3) << 14);		
				out.Cat((char *)&ctrl, 2); // Poke16le		
				int lln = literal_len;
				if(lln >= 3) {
					lln -= 3;
					do {
						out.Cat(min(lln, 255));
						lln -= 255;
					}
					while(lln > 0);
				}
				
				out.Cat(literal_ptr, literal_len);
				return out;
			}
		}
		
		int match_len = 4;
		while(hptr + match_len < s && s[match_len] == hptr[match_len])
			match_len++;
		LLOG("Literals (" << literal_len << "): " << AsCString(String(literal_ptr, literal_len)));
		LLOG("Match (" << match_len << "): " << AsCString(String(hptr, match_len)));
		
		int offset = s - hptr;
		
		Matches++;
		
		int ml = match_len - 4;

		// 54321098 76543210
		// LLMMMOOO OOOOOOOO L-literal len, O-offset, M-match len
		word ctrl = (offset >= 0x700 ? 0x700 | LOBYTE(offset) : offset)
		            | (min(ml, 7) << 11)
		            | (min(literal_len, 3) << 14);

		out.Cat((char *)&ctrl, 2); // Poke16le

		if(offset >= 0x700) {
			FarOffsets++;
			out.Cat(HIBYTE(offset));
		}
		
		int l = ml;
		if(l >= 7) {
			l -= 7;
			do {
				BigMatches++;
				out.Cat(min(l, 255));
				l -= 255;
			}
			while(l > 0);
		}
		
		int lln = literal_len;
		if(lln >= 3) {
			lln -= 3;
			do {
				BigLiterals++;
				out.Cat(min(lln, 255));
				lln -= 255;
			}
			while(lln > 0);
		}
		
		LiteralsLen += literal_len;
		Literals += !!literal_len;
		
		out.Cat(literal_ptr, literal_len);
		
		literals.Cat(literal_ptr, literal_len);
		
		const char *e = s + match_len;
		while(s < e) {
			hash_table[Hash(s - 3)] = s - 3;
			s++;
		}
		
		literal_ptr = s;
		literal_len = 0;
	}
}

#endif
