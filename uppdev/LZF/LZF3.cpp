#if 0

#include <Core/Core.h>

#include <plugin/lz4/lz4.h>

using namespace Upp;

// Test/Test FoooFooo

#define LLOG(x)     // LOG(x)
#define LTIMING(x)  // RTIMING(x)


#if 0
force_inline int Hash(const char *s)
{
//	return Peek32le(s) % 8191;
//	return memhash(s, 4) & 8191;
	dword h = Peek32le(s); return (((h >> 23) - (h >> 15) - (h >> 7) - h));
//	return (2654435761U * Peek32le(s)) & 8191;
}

force_inline int Hash8(const char *s)
{
	return CombineHash(Hash(s), Hash(s + 4));
}

force_inline int Hash16(const char *s)
{
	return CombineHash(Hash8(s), Hash8(s + 8));
}
#else
force_inline int Hash(const char *s)
{
//	return Peek32le(s) % 8191;
//	return memhash(s, 4) & 8191;
	dword h = Peek32le(s); return (((h >> 23) - (h >> 15) - (h >> 7) - h));
//	return (2654435761U * Peek32le(s)) & 8191;
}

force_inline int Hash8(const char *s)
{
	dword h = Peek32le(s) - Peek32le(s + 4);
	return (((h >> 23) - (h >> 15) - (h >> 7) - h));
}

force_inline int Hash16(const char *s)
{
	dword h = Peek32le(s) - Peek32le(s + 4) - Peek32le(s + 8) - Peek32le(s + 12);
	return (((h >> 23) - (h >> 15) - (h >> 7) - h));
}

force_inline int Hash32(const char *s)
{
	dword h = Peek32le(s) - Peek32le(s + 4) - Peek32le(s + 8) - Peek32le(s + 12) -
	          Peek32le(s + 16) - Peek32le(s + 20) - Peek32le(s + 24) - Peek32le(s + 28);
	return (((h >> 23) - (h >> 15) - (h >> 7) - h));
}
#endif

String Literals;
int LiteralsCount;
int Matches;
int Match32;
int Match16;
int Match8;
int Match4;
int BigMatches;
int BigLiterals;
int FarOffsets;
int HugeOffsets;
int Extended;
int LargeMatch;

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
	Literals.Clear();
	LiteralsCount = 0;
	Matches = 0;
	Match16 = 0;
	Match8 = 0;
	Match4 = 0;
	BigMatches = 0;
	BigLiterals = 0;
	FarOffsets = 0;
	HugeOffsets = 0;
	Extended = 0;
	LargeMatch = 0;

	const char *out0 = out;
	const char *end = s + length;
	
//	const int HASH_MASK = 2048 * 1024 - 1;
	const int HASH_MASK = 32768 * 1024 - 1;
	
	static const char *hash_table[HASH_MASK + 1];
	static const char *hash_table8[HASH_MASK + 1];
	static const char *hash_table16[HASH_MASK + 1];
	static const char *hash_table32[HASH_MASK + 1];
	
	for(int i = 0; i < HASH_MASK + 1; i++)
		hash_table32[i] = hash_table16[i] = hash_table8[i] = hash_table[i] = s;
	
	const char *literal_ptr = s;
	int literal_len = min(length, 32);
	s += literal_len;
	
	for(;;) {
		const char *hptr;

		int match_len;
		for(;;) {
			{ LTIMING("Lookup");
//			word hash4 = Hash(s);
//			hptr = hash_table8[CombineHash(hash4, Hash(s + 4)) & 8191];

//			LLOG(AsCString(hptr, hptr + 4) << ' ' << AsCString(s, s + 4) << ' ' << hash);

			hptr = hash_table32[Hash32(s) & HASH_MASK];
			if(s - hptr < 0xfffff && Peek64le(hptr) == Peek64le(s) &&
			   Peek64le(hptr + 8) == Peek64le(s + 8) &&
			   Peek64le(hptr + 16) == Peek64le(s + 16) &&
			   Peek64le(hptr + 24) == Peek64le(s + 24)) {
				Match32++;
				match_len = 32;
				break;
			}
	
			hptr = hash_table16[Hash16(s) & HASH_MASK];
			if(s - hptr < 0xfffff && Peek64le(hptr) == Peek64le(s) && Peek64le(hptr + 8) == Peek64le(s + 8)) {
				Match16++;
				match_len = 16;
				break;
			}
			
			hptr = hash_table8[Hash8(s) & HASH_MASK];
			if(s - hptr < 0xfffff && Peek64le(hptr) == Peek64le(s)) {
				Match8++;
				match_len = 8;
				break;
			}
			
//			hptr = hash_table[hash4 & 8181];
//			LLOG(AsCString(hptr, hptr + 4) << ' ' << AsCString(s, s + 4) << ' ' << hash);
	
			hptr = hash_table[Hash(s) & HASH_MASK];
			if(s - hptr < 0xfffff && Peek32le(hptr) == Peek32le(s)) {
				Match4++;
				match_len = 4;
				break;
			}
			}
			
			LTIMING("Hashing1");
			literal_len++;
			hash_table32[Hash32(s - 31) & HASH_MASK] = s - 31;
			hash_table16[Hash16(s - 15) & HASH_MASK] = s - 15;
			hash_table8[Hash8(s - 7) & HASH_MASK] = s - 7;
			hash_table[Hash(s - 3) & HASH_MASK] = s - 3;
		
//			hash_table[hash4 & 8191] = s - 3;
//			word hash4 = Hash(s - 3);
//			hash_table8[CombineHash(Hash(s - 7), hash4) & 8191] = s - 7;
			s++;
			if(s >= end - 32) {
				*out++ = 0xf0 | max(literal_len, 15);
				out = OutLen(out, literal_len, 15, BigLiterals);
				SVO_MEMCPY(out, literal_ptr, literal_len);
			//	Literals.Cat(literal_ptr, literal_len);
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

		Matches++;
		
		int ml = match_len - 4;

		LiteralsCount += literal_len > 0;

		if(literal_len == 0 && offset <= 0xfff && ml < 8) {
			*out++ = (ml << 4) & HIBYTE(offset);
			*out++ = LOBYTE(offset);
		}
		else {
			*out++ = 0x80 | (max(literal_len, 7) << 4) | max(ml, 15);
			offset -= 0xfff;
			if(offset < 0xfff0) {
				*out++ = HIBYTE(offset);
				*out++ = LOBYTE(offset);
			}
			else {
				HugeOffsets++;
				// FIX THIS
				*out++ = LOBYTE(offset);
				*out++ = HIBYTE(offset);
				*out++ = HIBYTE(offset);
			}
			Extended++;
			if(offset > 0xfff)
				FarOffsets++;
			if(ml >= 8)
				LargeMatch++;
			out = OutLen(out, literal_len, 7, BigLiterals);
			out = OutLen(out, ml, 15, BigMatches);
			SVO_MEMCPY(out, literal_ptr, literal_len);
			out += literal_len;
//			Literals.Cat(literal_ptr, literal_len);
		}

//		literals.Cat(literal_ptr, literal_len);
		
		LTIMING("Hashing 2");
		const char *e = s + match_len;
		while(s < e) {
			hash_table[Hash(s - 3) & HASH_MASK] = s - 3;
			hash_table8[Hash8(s - 7) & HASH_MASK] = s - 7;
			hash_table16[Hash16(s - 15) & HASH_MASK] = s - 15;
			hash_table32[Hash32(s - 31) & HASH_MASK] = s - 31;
//			word hash4 = Hash(s - 3);
//			hash_table[hash4 & 8191] = s - 3;
//			hash_table8[CombineHash(Hash(s - 7), hash4) & 8191] = s - 7;
			s++;
		}
		
		literal_ptr = s;
		literal_len = 0;
	}
}

#endif
