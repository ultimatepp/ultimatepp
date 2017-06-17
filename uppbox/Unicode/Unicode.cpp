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

void Output(const String& id, const word *data, int count)
{
	LOG("word " << id << "[] = {");
	for(int i = 0; i < count; i++) {
		if(i % 512 == 0)
			VppLog() << '\t';
		VppLog() << Format("0x%04X,", data[i]);
		if(i % 512 == 511)
			VppLog() << "\n";
	}
	LOG("};");
}

void Output(const String& id, const byte *data, int count)
{
	LOG("byte " << id << "[] = {");
	for(int i = 0; i < count; i++) {
		if(i % 512 == 0)
			VppLog() << '\t';
		VppLog() << Format("0x%02X,", data[i]);
		if(i % 512 == 511)
			VppLog() << "\n";
	}
	LOG("};");
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
	Vector<dword> despec;
	Vector<dword> catcode, cat;
	Vector<dword> lettercode, letter;
	Vector<dword> uppercode, lowercode;
	Vector<dword> isupper, islower; // only those that do not have uppercode/lowercode
	Vector<dword> rtl;
	VectorMap<String, int> dmap;
	int composed_upper_count = 0;
	int composed_failed = 0;
	Buffer<word> fast_upper(2048, 0);
	Buffer<word> fast_lower(2048, 0);
	Buffer<byte> fast_ascii(2048, 0);
	Buffer<byte> fast_info(2048, 0);
	for(int i = 0; i < 2048; i++)
		fast_upper[i] = fast_lower[i] = i;
	while(!in.IsEof()) {
		Vector<String> h = Split(in.GetLine(), ';', false);
//		DUMP(h);
//		DUMPC(h);
		if(h.GetCount() > 14) {
			dword upper = ScanHex(h[12]);
			dword lower = ScanHex(h[13]);
			dword cde = ScanInt(h[0], NULL, 16);
			String cat = h[2];
			if(h[4] == "R")
				rtl.Add(cde);
//			kind.FindAdd(h[2]);
			Vector<String> decomb = Split(h[5], ' ');
			if(decomb.GetCount() && *decomb[0] == '<') {
				dmap.GetAdd(decomb[0], 0)++;
				decomb.Remove(0);
				despec.Add(cde);
			}
			dword first = decomb.GetCount() ? ScanHex(decomb[0]) : 0;
			if(decomb.GetCount() > 1 ||
			   decomb.GetCount() && cde >= 2048 && first != cde && first < 128) // for ToAscii...
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
			dword other = Nvl(ScanInt(Nvl(h[12], h[13]), NULL, 16));
			if(other) {
			//	DUMP(other);
			//	DUMP(h);
				ercode.Add(cde);
				er.Add(other);
			}
			
			if(cde < 0x800) {
				fast_upper[cde] = word(upper ? upper : cde);
				fast_lower[cde] = word(lower ? lower : cde);
				fast_ascii[cde] = byte(cde < 128 ? cde : first < 128 ? first : 0);
				byte w = 0;
				if(cat == "Ll")
					w |= 1;
				if(cat == "Lu")
					w |= 2;
				if(*cat == 'L')
					w |= 4;
				fast_info[cde] = w;
			}
			else {
				if(*cat == 'L') {
					letter.Add(cde);
				}
				DUMPHEX(cde);
				if(cde == 0x1D7CA)
					LOG("HERE");
				if(!upper && !lower || cde == 0x1E9E) { // Capital sharp S does is not considered capital of lower case sharp s
					if(cat == "Lu")
						isupper.Add(cde);
					if(cat == "Ll")
						islower.Add(cde);
					DUMP(cat);
				}
			}
			if(upper)
				if(cde >= 0x800 || upper >= 0x800) {
//					DUMP(cat);
//					DUMPHEX(cde);
//					DUMPHEX(upper);
					lowercode.Add(cde);
					uppercode.Add(upper);
				}
		}
	}
	
	DUMPM(dmap);

	DUMP(code.GetCount());
	DUMP(lowercode.GetCount());
	
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

	DDUMP(letter.GetCount());
	data.Add(letter.GetCount());
	data.Append(letter);
	data.Add(islower.GetCount());
	data.Append(islower);
	data.Add(isupper.GetCount());
	data.Append(isupper);
	data.Add(despec.GetCount());
	data.Append(despec);

	Output("unicode_info__", ZCompress(Encode(data)));

	Output("unicode_fast_upper__", fast_upper, 2048);
	Output("unicode_fast_lower__", fast_lower, 2048);
	Output("unicode_fast_ascii__", fast_ascii, 2048);
	Output("unicode_fast_info__", fast_info, 2048);
}
