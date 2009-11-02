#include <Core/Core.h>

#define LLOG(x)           // LOG(x)
#define LLOGHEXDUMP(a, b) // LOGHEXDUMP(a, b)

using namespace Upp;

byte                    charset;
int                     vocn[256];
Vector<String>          voc;
VectorMap<int, String>  line;

int LineCode(const String& us)
{
	WString s = FromUtf8(us);
	return ToLower(ToAscii(s[0]), CHARSET_DEFAULT) +
	       (ToLower(ToAscii(s[1]), CHARSET_DEFAULT) << 8) +
	       (s.GetCount() < 3 ? 127 : (ToLower(ToAscii(s[2]), CHARSET_DEFAULT) << 16));
}

bool Contains(const String& a, const String& b)
{
	for(int i = 0; i + b.GetLength() <= a.GetLength(); i++)
		if(memcmp(~a + i, ~b, b.GetLength()) == 0) return true;
	return false;
}

struct LengthOrder
{
	bool operator()(const String& a, const String& b) const
	{
		return a.GetLength() > b.GetLength();
	}
};

struct NoCaseOrder
{
	bool operator()(const String& a, const String& b) const
	{
		String la = ToAscii(a);
		String lb = ToAscii(b);
		int q = SgnCompare(ToLower(ToAscii(a)), ToLower(ToAscii(b)));
		if(q)
			return q < 0;
		q = SgnCompare(ToLower(a), ToLower(b));
		if(q)
			return q < 0;
		return a < b;
	}
};

void Make()
{
	FileIn in(CommandLine()[0]);
	if(!in) {
		Cout() << "Unable to open " << CommandLine()[0] << " for reading\n";
		SetExitCode(1);
		return;
	}
	FileOut out(CommandLine()[1]);
	if(!out) {
		Cout() << "Unable to open " << CommandLine()[1] << " for writing\n";
		SetExitCode(1);
		return;
	}
	SetDefaultCharset(CHARSET_UTF8);
	Vector<String> w;
	Index<int> alphabet;
	String maxl;
	int    maxlen = 0;
	while(!in.IsEof()) {
		String l = in.GetLine();
		if(l.GetLength() > maxlen) {
			maxlen = l.GetLength();
			maxl = l;
		}
		if(FromUtf8(l).GetLength() > 1) {
			w.Add(l);
			for(const char *s = l; s < l.End(); s++)
				alphabet.FindAdd((byte)*s);
		}
	}
	
	if(alphabet.GetCount() > 250) {
		Cout() << "Alphabet is too big - only 250 different codepoints allowed.";
		SetExitCode(1);
		return;
	}
	
	Cout() << "Words loaded, now sorting\n";
	
	ASSERT(maxlen < 64);
	
	LLOG("Maximal length:" << maxlen << "  " << maxl);

	Sort(w, NoCaseOrder());
	
	Cout() << "Sorted, now gathering voc candidates\n";

// ------------------
	
	VectorMap<String, int> part;
	int dict = 0;
	int i = 0;
	while(i < w.GetCount()) {
		int linecode = LineCode(w[i]);
		String prevw;
		LLOG("line " << ~ToLower(w[i].Mid(0, 3)));
		while(i < w.GetCount() && LineCode(w[i]) == linecode) {
			String ww = w[i];
			int j;
			for(j = 0; j < prevw.GetLength(); j++)
				if(ww[j] != prevw[j]) break;
			if(j >= dict)
				dict = j + 1;
			for(int l = 2; l < ww.GetLength() - 1; l++)
				for(int q = j; q + l <= ww.GetLength(); q++)
					part.GetAdd(ww.Mid(q, l), 0)++;
			prevw = ww;
			i++;
		}
	}
	
	Cout() << "Creating voc\n";

	int dcount = 256 - dict;
	LLOG("dict: " << dict);
	LLOG("dict size: " << dcount);
	LLOG(" alphabet:" << alphabet.GetCount());
	LLOG(" combinations: " << dcount - alphabet.GetCount());
	
	for(i = 0; i < alphabet.GetCount(); i++)
		voc.Add(String(alphabet[i], 1));

	Vector<int> value;

	for(i = 0; i < part.GetCount(); i++)
		value.Add() = part[i] * (part.GetKey(i).GetLength() - 1);

	while(voc.GetCount() + dict < 256) {
		int m = 0;
		int mi = 0;
		int i;
		for(i = 0; i < part.GetCount(); i++)
			if(value[i] > m) {
				m = value[i];
				mi = i;
			}
		if(m <= 0) break;
		String v = part.GetKey(mi);
		vocn[voc.GetCount()] = value[mi];
		voc.Add(v);
		LLOG("Adding " << v << " [" << v.GetCount() << "] value:" << value[mi] << " count:" << part[mi]);
		for(i = 0; i < part.GetCount(); i++) {
			if(Contains(part.GetKey(i), v))
				value[i] -= v.GetLength() * part[i];
			if(Contains(v, part.GetKey(i)))
				value[i] -= part.GetKey(i).GetLength() * part[i];
		}
		value[mi] = 0;
	}
	
	int sum = 0;
	for(i = 0; i < voc.GetCount(); i++) {
		sum += vocn[i];
		LLOG(vocn[i] << "  " << voc[i]);
	}
	LLOG("Total " << sum);

// ------------------
	Sort(voc, LengthOrder());

	i = 0;
	while(i < w.GetCount()) {
		int linecode = LineCode(w[i]);
		String& ln = line.GetAdd(linecode);
		LLOG("---- Line " << ToLower(ToAscii(~w[i].Mid(0, 3))));
		String prevw;
		bool next = false;
		while(i < w.GetCount() && LineCode(w[i]) == linecode) {
			String ww = w[i];
			int j;
			for(j = 0; j < prevw.GetLength(); j++)
				if(ww[j] != prevw[j]) break;
			if(next)
				ln.Cat(j);
			LLOG(j << "\t" << w[i]);
			next = true;
			const char *s = ~ww + j;
			while(*s) {
				int i;
				for(i = 0; i < voc.GetCount(); i++) {
					if(memcmp(s, voc[i], voc[i].GetLength()) == 0) {
						LLOG("  " << s << " " << voc[i]);
						ln.Cat(i + dict);
						s += voc[i].GetLength();
						break;
					}
				}
				ASSERT(i < voc.GetCount());
			}
			prevw = ww;
			i++;
		}
		LLOGHEXDUMP(~ln, ln.GetCount());
	}
	int l = 0;
	for(i = 0; i < line.GetCount(); i++) {
		line[i].Cat(0);
		l += line[i].GetLength();
	}
	out.Put(CHARSET_UTF8);
	out.Put(0);
	out.Put(dict);
	for(i = 0; i < voc.GetCount(); i++) {
		out.Put(voc[i]);
		out.Put(0);
	}
	for(i = 0; i < line.GetCount(); i++) {
		out.PutL(line.GetKey(i));
		out.PutL(line[i].GetLength());
		out.Put(line[i]);
	}
}

CONSOLE_APP_MAIN
{
	if(CommandLine().GetCount() != 2) {
		Cout() << "Usage: MakeSpellScd <inputfile> <outputfile>\n";
		exit(1);
	}
	Make();
	Cout() << "* Finished\n";
}
