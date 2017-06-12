#include <Core/Core.h>

using namespace Upp;

void Output(const String& id, const String& data)
{
	VppLog() << "byte " << id << "[] = { // " << data.GetCount() << " bytes";
	for(int i = 0; i < data.GetCount(); i++) {
		if((i & 1023) == 0) {
			VppLog() << "\n\t";
		}
		VppLog() << Format("0x%02X,", (byte)data[i]);
	}
	VppLog() << "\n};\n";
}

void Delta(Vector<dword>& data)
{
	for(int i = data.GetCount() - 1; i > 0; i--)
		data[i] -= data[i - 1] - 127;
}

String Encode(const Vector<dword>& data)
{
	DDUMP(data.GetCount());
	String r;
	dword prev = 0;
	int l = 0;
	for(dword v : data) {
		dword val = v - prev + 127;
		if(val < 255)
			r.Cat(val);
		else {
			r.Cat(255);
			r.Cat(val);
			r.Cat(val >> 8);
			r.Cat(val >> 16);
			r.Cat(val >> 24);
			l++;
		}
		prev = v;
	}
	DDUMP(l);
	DDUMP(r.GetCount());
	return r;
}

Vector<dword> Decode(const String& data)
{
	DDUMP(data.GetCount());
	Vector<dword> r;
	dword prev = 0;
	StringStream ss(data);
	while(!ss.IsEof()) {
		byte ch = ss.Get();
		dword val;
		if(ch < 255) {
			val = prev + ch - 127;
		}
		else {
			val = ss.Get();
			val |= ss.Get() << 8;
			val |= ss.Get() << 16;
			val |= ss.Get() << 24;
			val = prev + val - 127;
		}
		r.Add(val);
		prev = val;
	}
	return r;
}

dword ScanHex(const char *s)
{
	return Nvl(ScanInt(s, NULL, 16));
}

String Hex(dword d)
{
	return FormatIntHexUpper(d);
}

enum {
	UNICODE_CATEGORY_Cc, UNICODE_CATEGORY_Cf, UNICODE_CATEGORY_Cn, UNICODE_CATEGORY_Co,
	UNICODE_CATEGORY_Cs, UNICODE_CATEGORY_LC, UNICODE_CATEGORY_Ll, UNICODE_CATEGORY_Lm,
	UNICODE_CATEGORY_Lo, UNICODE_CATEGORY_Lt, UNICODE_CATEGORY_Lu, UNICODE_CATEGORY_Mc,
	UNICODE_CATEGORY_Me, UNICODE_CATEGORY_Mn, UNICODE_CATEGORY_Nd, UNICODE_CATEGORY_Nl,
	UNICODE_CATEGORY_No, UNICODE_CATEGORY_Pc, UNICODE_CATEGORY_Pd, UNICODE_CATEGORY_Pe,
	UNICODE_CATEGORY_Pf, UNICODE_CATEGORY_Pi, UNICODE_CATEGORY_Po, UNICODE_CATEGORY_Ps,
	UNICODE_CATEGORY_Sc, UNICODE_CATEGORY_Sk, UNICODE_CATEGORY_Sm, UNICODE_CATEGORY_So,
	UNICODE_CATEGORY_Zl, UNICODE_CATEGORY_Zp, UNICODE_CATEGORY_Zs
};

CONSOLE_APP_MAIN
{
	FileIn in(GetDataFile("UnicodeData.txt"));
	Vector<dword> code, comb[3], er, ercode;
	Vector<dword> catcode, cat;
	Vector<dword> lettercode, letter;
	VectorMap<String, int> catcount;
	Index<String> kind;
	kind.Add("Cc");
	kind.Add("Cf");
	kind.Add("Cn");
	kind.Add("Co");
	kind.Add("Cs");
	kind.Add("LC");
	kind.Add("Ll");
	kind.Add("Lm");
	kind.Add("Lo");
	kind.Add("Lt");
	kind.Add("Lu");
	kind.Add("Mc");
	kind.Add("Me");
	kind.Add("Mn");
	kind.Add("Nd");
	kind.Add("Nl");
	kind.Add("No");
	kind.Add("Pc");
	kind.Add("Pd");
	kind.Add("Pe");
	kind.Add("Pf");
	kind.Add("Pi");
	kind.Add("Po");
	kind.Add("Ps");
	kind.Add("Sc");
	kind.Add("Sk");
	kind.Add("Sm");
	kind.Add("So");
	kind.Add("Zl");
	kind.Add("Zp");
	kind.Add("Zs");
	while(!in.IsEof()) {
		Vector<String> h = Split(in.GetLine(), ';', false);
//		DUMP(h);
//		DUMPC(h);
		if(h.GetCount() > 14) {
			dword lower = ScanHex(h[13]);
			dword upper = ScanHex(h[14]);
			dword cde = ScanInt(h[0], NULL, 16);
//			kind.FindAdd(h[2]);
			Vector<String> decomb;
			decomb.AppendRange(FilterRange(Split(h[5], ' '), [] (const String& x) { return x[0] != '<'; }));
			if(decomb.GetCount() > 1) {
				// DLOG(h[0] << " -> " << comb[0] << ' ' << comb[1]);
				code.Add(cde);
				decomb.Add("0");
				comb[0].Add(ScanInt(decomb[0], NULL, 16));
				comb[1].Add(ScanInt(decomb[1], NULL, 16));
				comb[2].Add(ScanInt(decomb[2], NULL, 16));
			}
			if(cde > 2048) {
				if(*h[2] == 'L') {
					catcode.Add(cde);
					cat.Add(cde);
				}
			}
			catcount.GetAdd(h[2], 0)++;
			catcode.Add(cde);
			cat.Add(max(kind.Find(h[2]), 0));
			dword other = Nvl(ScanInt(Nvl(h[12], h[13]), NULL, 16));
			if(other) {
			//	DUMP(other);
			//	DUMP(h);
				ercode.Add(cde);
				er.Add(other);
			}
			
			dword t[MAX_DECOMPOSED];
			UnicodeDecompose(cde, t);
			dword composed_upper = 0;
			dword composed_lower = 0;
			if(t[0] < 128 && t[1]) {
				t[0] = ToUpper((char)t[0]);
				composed_upper = UnicodeCompose(t, 2);
				t[0] = ToLower((char)t[0]);
				composed_lower = UnicodeCompose(t, 2);
			}
			if(upper)
				if(composed_upper = upper)
					DLOG("Composed upper " << Hex(cde) << " -> " << Hex(composed_upper));
				else
					DLOG("Composed upper failed " << cde);
		}
	}
/*
	DDUMP(code.GetCount());
	
	Vector<dword> data;
	data.Add(code.GetCount());
	data.Append(code);
	data.Append(comb[0]);
	data.Append(comb[1]);
	DUMP(comb[0]);
	DUMP(comb[2]);
	data.Append(comb[2]);

	DDUMP(letter.GetCount());
	DUMPC(letter);
	DUMP(ZCompress(Encode(letter)).GetCount());
	DUMP(ZCompress(Encode(lettercode)).GetCount());

	DUMP(cat.GetCount());
	DUMP(ZCompress(Encode(cat)).GetCount());
	DUMP(ZCompress(Encode(catcode)).GetCount());
	
	Output("unicode_info__", ZCompress(Encode(data)));
*/	
	DUMPM(catcount);
	DUMP(Sum(catcount));
#if 0
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
#endif
}
