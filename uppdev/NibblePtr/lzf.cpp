#include "Entropy.h"

// Test/Test FooFoo

String LZFCompress(const char *s, int length)
{
	String result;

	const char *end = s + length;
	
	const char *hash_table[8192];
	
	for(int i = 0; i < 8192; i++)
		hash_table[i] = s;
	
	char *lit = s;
	int litlen = min(length, 4);
	s += 4;
	
	for(;;) {
		char *hptr;		
		for(;;) {
			hash = (2654435761U * Peek32le(s)) & 8191;
			
			hptr = hash_table[hash];
			
			if(s - hptr < 65536 && Peek32le(hash_table[hash]) == Peek32le(s))
				break;
			litlen++;
			hash_table[(2654435761U * Peek32le(s - 3)) & 8191] = s - 3;
			s++;
			if(s >= end)
				return;
		}
		
		int matchlen = 4;
		while(hptr + matchlen < s && s[matchlen] == hptr[matchlen])
			matchlen++;
		
		LOGHEXDUMP(lit, litlen);
		LOGHEXDUMP(hptr, matchlen);
		
		const char *e = s + matchlen;
		while(s < e) {
			hash_table[(2654435761U * Peek32le(s - 3)) & 8191] = s - 3;
			s++;
		}
		
		lit = s;
		
	}
}
