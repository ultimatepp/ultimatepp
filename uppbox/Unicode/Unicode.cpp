#include <Core/Core.h>

using namespace Upp;

void Delta(Vector<dword>& data)
{
	for(int i = data.GetCount() - 1; i > 0; i--)
		data[i] -= data[i - 1] - 127;
}

CONSOLE_APP_MAIN
{
	FileIn in(GetDataFile("UnicodeData.txt"));
	Vector<dword> code, comb1, comb2, er, ercode;
	while(!in.IsEof()) {
		Vector<String> h = Split(in.GetLine(), ';', false);
		if(h.GetCount() > 14) {
			dword cde = ScanInt(h[0], NULL, 16);
			DDUMPC(h);
			Vector<String> comb;
			comb.AppendRange(FilterRange(Split(h[5], ' '), [] (const String& x) { return x[0] != '<'; }));
			if(comb.GetCount() > 1) {
				DLOG(h[0] << " -> " << comb[0] << ' ' << comb[1]);
				code.Add(cde);
				comb1.Add(ScanInt(comb[0], NULL, 16));
				comb2.Add(ScanInt(comb[0], NULL, 16));
			}
			dword other = Nvl(ScanInt(Nvl(h[12], h[13]), NULL, 16));
			if(other) {
				DUMP(other);
				DUMP(h);
				ercode.Add(cde);
				er.Add(other);
			}
		}
	}
	
	Delta(code);
	Delta(comb1);
	Delta(comb2);

	{
		String h;
		h.Cat((const char *)code.begin(), 4 * code.GetCount());
		h.Cat((const char *)comb1.begin(), 4 * code.GetCount());
		h.Cat((const char *)comb2.begin(), 4 * code.GetCount());
		
		DUMPC(comb1);
		
		DUMP(h.GetCount());
		DUMP(ZCompress(h).GetCount());
	}

	{
		DDUMP(ercode.GetCount());
		DDUMP(ercode);
		Delta(ercode);
		DDUMP(ercode);
		Delta(er);
		
		String h;
		h.Cat((const char *)ercode.begin(), 4 * ercode.GetCount());
		h.Cat((const char *)er.begin(), 4 * er.GetCount());

		DUMP(h.GetCount());
		DUMP(ZCompress(h).GetCount());
	}
}
