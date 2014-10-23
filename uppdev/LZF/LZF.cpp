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

String LZFCompress(const char *s, int length, String& literals)
{
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

	//		LOG(AsCString(hptr, hptr + 4) << ' ' << AsCString(s, s + 4) << ' ' << hash);
	
			if(s - hptr < 0xfffff && Peek32le(hash_table[hash]) == Peek32le(s))
				break;
			literal_len++;
			hash_table[Hash(s - 3)] = s - 3;
			s++;
			if(s >= end) {
				word ctrl = (min(literal_len, 3) << 14);		
				out.Cat((char *)&ctrl, 2); // Poke16le		
				if(literal_len >= 3) {
					literal_len -= 3;
					do {
						out.Cat(min(literal_len, 255));
						literal_len -= 255;
					}
					while(literal_len > 0);
				}
				
				out.Cat(literal_ptr, literal_len);
				return out;
			}
		}
		
		int match_len = 4;
		while(hptr + match_len < s && s[match_len] == hptr[match_len])
			match_len++;
		LOG("Literals (" << literal_len << "): " << AsCString(String(literal_ptr, literal_len)));
		LOG("Match (" << match_len << "): " << AsCString(String(hptr, match_len)));
		
		int offset = s - hptr;
		
		// 54321098 76543210
		// LLMMMOOO OOOOOOOO L-literal len, O-offset, M-match len
		word ctrl = (offset >= 0x7f0 ? (0x7f | (offset >> 16)) : offset)
		            | (min(match_len, 11) << 11)
		            | (min(literal_len, 3) << 14);

		out.Cat((char *)&ctrl, 2); // Poke16le

		if(offset >= 0x7f0)
			out.Cat((char *)&offset, 2); // Poke16le
		
		int l = match_len;
		if(l - 4 >= 7) {
			l -= 7;
			do {
				out.Cat(min(l, 255));
				l -= 255;
			}
			while(l > 0);
		}
		
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

CONSOLE_APP_MAIN
{
//	String data = LoadFile("C:/u/upp.src/uppsrc/CtrlLib/ArrayCtrl.cpp");
//	String data = LoadFile(GetDataFile("LZF.cpp"));
	String l;
	String cdata = LZFCompress(~data, data.GetLength(), l);
	DDUMP(cdata.GetCount());
	DDUMP(AsCString(l, 100));
	DDUMP(l.GetCount());
	DDUMP(data.GetLength());
}
