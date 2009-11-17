#include <Core/Core.h>

#define LLOG(x)

using namespace Upp;

struct Sfx : Moveable <Sfx> {
	struct Letter : public Bits {
		bool           neg;

		Letter() { neg = false; }
	};
	Vector<Letter> cond;
	int            len;
	String         text;
};

struct Sfxl : Moveable <Sfxl> {
	bool cross;
	Vector <Sfx> sfx;
	Sfxl() { cross = true; }
};

template struct Moveable<Sfx::Letter>;

VectorMap< int, Sfxl > suffix;
VectorMap< int, Vector<String> > prefix;

bool Match(const String& w, const Sfx& f)
{
	if(w.GetLength() < f.cond.GetCount())
		return false;
	const char *s = w.End() - f.cond.GetCount();
	for(int i = 0; i < f.cond.GetCount(); i++) {
		const Sfx::Letter& l = f.cond[i];
		if(l.Get((byte)s[i]) == l.neg) return false;
	}
	return true;
}

CONSOLE_APP_MAIN
{
	if(CommandLine().GetCount() != 1) {
		Cout() << "Usage: ConvertDic <file.dic file.aff>\n";
		exit(1);
	}
	String d = CommandLine()[0];
	SetDefaultCharset(CHARSET_ISO8859_2);
	byte outcharset = CHARSET_UTF8;       // CHANGED !!!

	FileIn in(d + ".aff");
	in.GetLine();
	in.GetLine();
	prefix.Add(0).Add();
	while(!in.IsEof()) {
		String ln = in.GetLine();
		int q = ln.Find('#');
		if(q >= 0)
			ln.Trim(q);
		Vector<String> p = Split(ln, ' ');
		if(p.GetCount()) {
			if(p[0] == "PFX" && p.GetCount() == 5)
				prefix.GetAdd(p[1][0]).Add(p[3]);
			if(p[0] == "SFX") {
				if(p.GetCount() == 5) {
					Sfx& f = suffix.GetAdd(p[1][0]).sfx.Add();
					if(p[2][0] == '0')
						f.len = 0;
					else
						f.len = p[2].GetLength();
					f.text = p[3];
					if(f.text == "0")
						f.text.Clear();
					const char *s = p[4];
					if(*s != '.') {
						while(*s) {
							if(*s == '[') {
								Sfx::Letter& l = f.cond.Add();
								s++;
								if(*s == '^') {
									l.neg = true;
									s++;
								}
								while(*s && *s != ']')
									l.Set((byte)*s++);
								s++;
							}
							else
								f.cond.Add().Set((byte)*s++);
						}
					}
				}
				if(p.GetCount() == 4 && isdigit(p[3][0]) && p[2][0] == 'N')
					suffix.GetAdd(p[1][0]).cross = false;
			}
		}
	}

	FileOut out(d + ".txt");
	in.Open(d + ".dic");
	in.GetLine();
	while(!in.IsEof()) {
		String w = in.GetLine();
		LLOG(w);
		String aff;
		int q = w.Find('/');
		if(q >= 0) {
			aff = w.Mid(q + 1);
			w.Trim(q);
		}
		Vector<int> pi;
		Vector<int> si;
		for(const char *s = aff; *s; s++) {
			int q = suffix.Find(*s);
			if(q >= 0)
				si.Add(q);
			q = prefix.Find(*s);
			if(q >= 0)
				pi.Add(q);
		}
		pi.Add(0);

		for(int i = 0; i < pi.GetCount(); i++) {
			Vector<String>& p = prefix[pi[i]];
			for(int i = 0; i < p.GetCount(); i++) {
				String pfx = p[i];
				LLOG("\tPrefix: " << pfx);
				out.PutLine(ToCharset(outcharset, pfx + w));
				LLOG("> " << pfx + w);
				for(int i = 0; i < si.GetCount(); i++) {
					Sfxl& u = suffix[si[i]];
					if(u.cross || pfx.IsEmpty())
						for(int i = 0; i < u.sfx.GetCount(); i++) {
							const Sfx& f = u.sfx[i];
							if(Match(w, f) && w.GetLength() > f.len) {
								LLOG("\tSuffix: " << f.text);
								String o = pfx + w.Mid(0, w.GetLength() - f.len) + f.text;
								LLOG("> " << o);
								out.PutLine(ToCharset(outcharset, o));
							}
						}
				}
			}
		}
	}
}
