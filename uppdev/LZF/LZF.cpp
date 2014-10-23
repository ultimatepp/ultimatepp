#include <Core/Core.h>

using namespace Upp;

// Test/Test FoooFooo

int Hash(const char *s)
{
//	return Peek32le(s) % 8191;
//	return memhash(s, 4) & 8191;
	dword h = Peek32le(s); return (((h >> 23) - (h >> 15) - (h >> 7) - h)) & 8191;
//	return (2654435761U * Peek32le(s)) & 8191;
}

void LZFCompress(const char *s, int length, String& literals)
{
	const char *end = s + length;
	
	const char *hash_table[8192];
	
	for(int i = 0; i < 8192; i++)
		hash_table[i] = s;
	
	const char *lit = s;
	int litlen = min(length, 4);
	s += 4;
	
	for(;;) {
		const char *hptr;
		for(;;) {
			int hash = Hash(s);
			
			hptr = hash_table[hash];

	//		LOG(AsCString(hptr, hptr + 4) << ' ' << AsCString(s, s + 4) << ' ' << hash);
	
			if(s - hptr < 65536 && Peek32le(hash_table[hash]) == Peek32le(s))
				break;
			litlen++;
			hash_table[Hash(s - 3)] = s - 3;
			s++;
			if(s >= end)
				return;
		}
		
		int matchlen = 4;
		while(hptr + matchlen < s && s[matchlen] == hptr[matchlen])
			matchlen++;
		
		LOG("Literals (" << litlen << "): " << AsCString(String(lit, litlen)));
		LOG("Match (" << matchlen << "): " << AsCString(String(hptr, matchlen)));
		
		literals.Cat(String(lit, litlen));
		
		const char *e = s + matchlen;
		while(s < e) {
			hash_table[Hash(s - 3)] = s - 3;
			s++;
		}
		
		lit = s;
		litlen = 0;
	}
}

CONSOLE_APP_MAIN
{
	String data = LoadFile(GetDataFile("LZF.cpp"));
	String l;
	LZFCompress(~data, data.GetLength(), l);
	DDUMP(AsCString(l, 100));
	DDUMP(l.GetCount());
	DDUMP(data.GetLength());
}
