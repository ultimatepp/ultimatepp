#if 1 // Now broken after optimization

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

force_inline char *OutLen(char *out, int len, int maxn, int& stat)
{
	if(len >= maxn) {
		len -= maxn;
		while(len >= 255) {
			*out++ = 255;
			len -= 255;
			stat++;
		}
		*out++ = len;
		stat++;
	}
	return out;
}

int LZFCompress(char *out, const char *s, int length)
{
	RTIMING("LZFCompress");
	const char *out0 = out;
	const char *end = s + length;
	
	const char *hash_table[8192];
	const char *hash_table8[8192];
	
	for(int i = 0; i < 8192; i++)
		hash_table8[i] = hash_table[i] = s;
	
	const char *literal_ptr = s;
	int literal_len = min(length, 8);
	s += literal_len;
	
	for(;;) {
		const char *hptr;

		int match_len;
		for(;;) {
			{ LTIMING("Lookup");
			word hash4 = Hash(s);
			hptr = hash_table8[CombineHash(hash4, Hash(s + 4)) & 8191];

//			LLOG(AsCString(hptr, hptr + 4) << ' ' << AsCString(s, s + 4) << ' ' << hash);
	
			if(s - hptr < 0xffff && Peek64le(hptr) == Peek64le(s)) {
				match_len = 8;
				break;
			}
			
			hptr = hash_table[hash4 & 8181];

//			LLOG(AsCString(hptr, hptr + 4) << ' ' << AsCString(s, s + 4) << ' ' << hash);
	
			if(s - hptr < 0xffff && Peek32le(hptr) == Peek32le(s)) {
				match_len = 4;
				break;
			}
			}
			
			LTIMING("Hashing1");
			literal_len++;
			word hash4 = Hash(s - 3);
			hash_table[hash4 & 8191] = s - 3;
			hash_table8[CombineHash(Hash(s - 7), hash4) & 8191] = s - 7;
			s++;
			if(s >= end - 8) {
				*out++ = 0xf0 | max(literal_len, 15);
				out = OutLen(out, literal_len, 15, BigLiterals);
				SVO_MEMCPY(out, literal_ptr, literal_len);
				return out + literal_len - out0;
			}
		}
		
		{ LTIMING("MatchLen");
		while(hptr + match_len < s && s[match_len] == hptr[match_len])
			match_len++;
		}

		LLOG("Literals (" << literal_len << "): " << AsCString(String(literal_ptr, literal_len)));
		LLOG("Match (" << match_len << "): " << AsCString(String(hptr, match_len)));
		
		int offset = s - hptr;
		
		if(literal_len) {
			Literals++;
			LiteralsLen += literal_len;
			LTIMING("literal cat");
			*out++ = 0xf0 | max(literal_len, 15);
			out = OutLen(out, literal_len, 15, BigLiterals);
			SVO_MEMCPY(out, literal_ptr, literal_len);
			out += literal_len;
		}
		
		Matches++;
		
		int ml = match_len - 4;

		{
			LTIMING("Out match");
			word ctrl = (offset >= 0xf00 ? 0xf00 | (byte)offset : offset)
			            | (min(ml, 14) << 13);
	
			{
				Poke16le(out, ctrl);
				out += 2;
				out = OutLen(out, ml, 14, BigMatches);
			}
	
			if(offset >= 0xf00) {
				FarOffsets++;
				*out++ = HIBYTE(offset);
			}
		}
		
//		literals.Cat(literal_ptr, literal_len);
		
		LTIMING("Hashing 2");
		const char *e = s + match_len;
		while(s < e) {
			word hash4 = Hash(s - 3);
			hash_table[hash4 & 8191] = s - 3;
			hash_table8[CombineHash(Hash(s - 7), hash4) & 8191] = s - 7;
			s++;
		}
		
		literal_ptr = s;
		literal_len = 0;
	}
}

#endif
