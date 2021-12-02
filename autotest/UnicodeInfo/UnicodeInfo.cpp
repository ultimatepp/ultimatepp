#include <Core/Core.h>

using namespace Upp;

dword ScanHex(const char *s)
{
	return Nvl(ScanInt(s, NULL, 16));
}

CONSOLE_APP_MAIN
{
//	StdLogSetup(LOG_COUT|LOG_FILE);

	FileIn in(GetDataFile("../../uppbox/Unicode/UnicodeData.txt"));

	while(!in.IsEof()) {
		Vector<String> h = Split(in.GetLine(), ';', false);
		if(h.GetCount() > 14) {
			int code = ScanInt(h[0], NULL, 16);
			if(code < 200)
				DUMPC(h);
			String cat = h[2];
			int upper = ScanHex(h[12]);
			int lower = ScanHex(h[13]);
			DUMP(code);
			DUMPHEX(code);
			DUMP(cat);
			DUMPHEX(lower);
			DUMPHEX(upper);
			DDUMPHEX(ToUpper(code));
			DDUMPHEX(ToLower(code));
			DDUMP(IsUpper(code));
			DDUMP(IsLower(code));
			DUMPHEX(ToAscii(code));
			DDUMP(IsMark(code));
			ASSERT(IsLetter(code) == (*cat == 'L'));
			if(cat == "Lu")
				ASSERT(IsUpper(code));
			if(cat == "Ll")
				ASSERT(IsLower(code));
			if(lower)
				ASSERT(ToLower(code) == lower);
			if(upper)
				ASSERT(ToUpper(code) == upper);
			ASSERT((h[4] == "R") == IsRTL(code));
			ASSERT((*cat == 'M') == IsMark(code));

			Vector<String> decomb;
			decomb.AppendRange(FilterRange(Split(h[5], ' '), [] (const String& x) { return x[0] != '<'; }));
			DDUMP(decomb);
			dword first = decomb.GetCount() ? ScanHex(decomb[0]) : 0;
			DDUMP(first);
			DDUMP((int)ToAscii(code));
			DDUMP(first);

			if(code < 128)
				ASSERT(ToAscii(code) == code);
			else
			if(first) {
				if(first < 128)
					ASSERT(ToAscii(code) == first);
				else
					ASSERT(ToAscii(code) == 32);
			}
			
			bool compat = *h[5] == '<';

			if(decomb.GetCount() > 1 ||
			   decomb.GetCount() && code >= 2048 && first != code && first < 128) // for ToAscii...
			{
				Vector<dword> comb;
				for(int i = 0; i < decomb.GetCount(); i++)
					comb.Add(ScanHex(decomb[i]));
				
				DUMP(comb);
				
				dword t[MAX_DECOMPOSED];
				int n = UnicodeDecompose(code, t);
				ASSERT(n == comb.GetCount());
				for(int i = 0; i < comb.GetCount(); i++)
					ASSERT(comb[i] == t[i]);
				
				ASSERT(comb == UnicodeDecompose(code));
				
				WString h = UnicodeDecompose(code, true);
				if(h.GetCount() > 1) {
					DUMP(h);
					ASSERT(code == UnicodeCompose(h));
				}
			}

			DLOG("============");
		}
	}
	
	for(int i = 0; i < 256; i++) {
		#define CHT(fn, type) ASSERT(fn(i) == fn((type)i));
		#define CHK(fn) CHT(fn, dword) CHT(fn, wchar) CHT(fn, char)
		
		CHK(IsLetter);
		CHK(IsUpper);
		CHK(IsLower);
		CHK(ToUpper);
		CHK(ToLower);
		CHK(ToAscii);
	}
	
	LOG("============= OK");
}
