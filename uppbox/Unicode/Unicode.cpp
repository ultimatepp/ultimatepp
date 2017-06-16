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
	Vector<dword> uppercode, lowercode;
	Vector<dword> rtl;
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
	int composed_upper_count = 0;
	int composed_failed = 0;
	while(!in.IsEof()) {
		Vector<String> h = Split(in.GetLine(), ';', false);
//		DUMP(h);
//		DUMPC(h);
		if(h.GetCount() > 14) {
			dword lower = ScanHex(h[13]);
			dword upper = ScanHex(h[14]);
			dword cde = ScanInt(h[0], NULL, 16);
			if(h[4] == "R")
				rtl.Add(cde);
//			kind.FindAdd(h[2]);
			Vector<String> decomb;
			decomb.AppendRange(FilterRange(Split(h[5], ' '), [] (const String& x) { return x[0] != '<'; }));
			dword first;
			if(decomb.GetCount() > 1 ||
			   decomb.GetCount() && (first = ScanHex(decomb[0])) != cde && first < 128) // for ToAscii...
			{
				// DLOG(h[0] << " -> " << comb[0] << ' ' << comb[1]);
				code.Add(cde);
				decomb.Add("0");
				decomb.Add("0");
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
			
			if(cde >= 0x7ff && upper) {
				lowercode.Add(cde);
				uppercode.Add(upper);
			}
		}
	}

	DDUMP(composed_upper_count);
	DDUMP(composed_failed);
	DUMP(code.GetCount());
	DUMP(lowercode.GetCount());
	
	Index<dword> cx(pick(code));
	Vector<dword> ocomb[3];
	ocomb[0] = pick(comb[0]);
	ocomb[1] = pick(comb[1]);
	ocomb[2] = pick(comb[2]);

	code.Clear();
	comb[0].Clear();
	comb[1].Clear();
	comb[2].Clear();
	
	for(int i = 0; i < cx.GetCount(); i++) {
		code.Add(cx[i]);
		for(int ci = 0; ci < 3; ci++)
			comb[ci].Add(ocomb[ci][i]);
		
		if(ocomb[1][i]) {
			int q = cx.Find(ocomb[0][i]);
			if(q >= 0) {
				DUMPHEX(cx[i]);
				Vector<int> n;
				int ti = 0;
				for(int ci = 0; ci < 3 && ocomb[ci][q]; ci++)
					n.Add(ocomb[ci][q]);
				for(int ci = 1; ci < 3 && ocomb[ci][i]; ci++)
					n.Add(ocomb[ci][i]);
				DUMP(n);
				ASSERT(ti < 4);
				code.Add(cx[i]);
				for(int ci = 0; ci < 3; ci++)
					comb[ci].Add(i < n.GetCount() ? n[i] : 0);
			}
		}
	}
	
	
	Vector<dword> data;
	data.Add(code.GetCount());
	data.Append(code);
	data.Append(comb[0]);
	data.Append(comb[1]);
	data.Append(comb[2]);
	data.Add(lowercode.GetCount());
	data.Append(lowercode);
	data.Append(uppercode);
	
	DDUMP(rtl[0]);
	DDUMP(rtl.GetCount());
	
	data.Add(rtl.GetCount());
	data.Append(rtl);
/*
	DDUMP(letter.GetCount());
	DUMPC(letter);
	DUMP(ZCompress(Encode(letter)).GetCount());
	DUMP(ZCompress(Encode(lettercode)).GetCount());

	DUMP(cat.GetCount());
	DUMP(ZCompress(Encode(cat)).GetCount());
	DUMP(ZCompress(Encode(catcode)).GetCount());
*/
	Output("unicode_info__", ZCompress(Encode(data)));
	
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
