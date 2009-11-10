#include <Core/Core.h>

#define LLOG(x)    // LOG(x)
#define LTIMING(x) RTIMING(x)

using namespace Upp;

struct SpellFile : Moveable<SpellFile> {
};

String spell_path;

static StaticMutex spellmutex;

void SetSpellPath(const String& p)
{
	INTERLOCKED_(spellmutex) {
		spell_path = p;
	}
}

static String sZet(FileIn& in, int offset, int len)
{
	in.Seek(offset);
	return ZDecompress(in.Get(len));	
}

bool SpellWordRaw(int lang, const WString& wrd)
{
	LTIMING("SpellWordRaw");
	LLOG("SpellWordRaw " << wrd << ' ' << LNGAsText(lang));
	static VectorMap<int, SpellFile> spellfile;
	int q = spellfile.Find(lang);
	if(q < 0) {
		String pp;
		String dir = ConfigFile("scd");
		for(;;) {
			pp << dir << ';';
			String d = GetFileFolder(dir);
			if(d == dir) break;
			dir = d;
		}
		pp << spell_path << ';' << getenv("LIB") << ';' << getenv("PATH") << ';';
		String path = GetFileOnPath(LNGAsText(lang) + ".scd", pp);
		if(IsNull(path))
			return true;
		FileIn in(path);
		if(!in || in.Get() != 255)
			return true;
		q = spellfile.GetCount();
		SpellFile& f = spellfile.Add(lang);
		f.path = path;
		int n = in.GetL();
		LLOG("Found scd file " << path << " blocks " << n);
		if(n > 0 && n < 100000) {
			for(int i = 0; i < n; i++) {
				SpellBlock& b = f.block.Add();
				b.first = in.Get(in.Get());
				b.ctrl_len = in.GetL();
				b.text_len = in.GetL();
			}
			if(in.IsEof())
				f.block.Clear();
			else {
				int off = (int)in.GetPos();
				for(int i = 0; i < n; i++) {
					SpellBlock& b = f.block[i];
					b.offset = off;
					off += b.ctrl_len + b.text_len;
				}
			}
		}
	}
	const SpellFile& f = spellfile[q];
	String awrd = ToUpper(ToAscii(wrd).ToString());
	String t1 = ToUtf8(wrd);
	String t2 = ToUtf8(ToLower(wrd));
//	DDUMP(awrd);
	for(int i = 0;; i++) {
//		if(i + 1 < f.block.GetCount())
//			DDUMP(f.block[i + 1].first);
		if(i + 1 >= f.block.GetCount() || awrd <= f.block[i + 1].first) {
			for(;;) {
				if(i >= f.block.GetCount())
					return false;
				LLOG("Spell block " << i << ": " << f.block[i].first);
				const SpellBlock& b = f.block[i++];
				if(b.first > awrd) {
					LLOG("  --- false");
					return false;
				}
				FileIn in(f.path);
				String ctrl = sZet(in, b.offset, b.ctrl_len);
				String text = sZet(in, b.offset + b.ctrl_len, b.text_len);
				in.Close();
//				LTIMING("Search");
				String w;
				const char *s = ctrl;
				const char *e = ctrl.End();
				const char *t = text;
				const char *te = text.End();
				while(s < e && t < te) {
					w.Trim(*s++);
					while(*t)
						w.Cat(*t++);
//					DDUMP(w);
					if(w == t1 || w == t2)
						return true;
					t++;
				}
			}
		}
	}
	return false;
}

struct SpellKey : Moveable<SpellKey> {
	int     lang;
	WString wrd;
	
	unsigned GetHashValue() const { return CombineHash(lang, wrd); }
	bool operator==(const SpellKey& b) const { return lang == b.lang && wrd == b.wrd; }
};

struct SpellMaker : LRUCache<bool, SpellKey>::Maker {
	SpellKey k;
	
	SpellKey Key() const  { return k; }
	int    Make(bool& r) const {
		r = SpellWordRaw(k.lang, k.wrd);
		return 1;
	}
};

bool SpellWord(int lang, const WString& wrd)
{
	INTERLOCKED_(spellmutex) {
		static LRUCache<bool, SpellKey> cache;
		cache.Shrink(2000);
		SpellMaker m;
		m.k.lang = lang;
		m.k.wrd = wrd;
		return cache.Get(m);
	}
}

bool SpellWord(int lang, const String& wrd)
{
	return SpellWord(lang, wrd.ToWString());
}

bool Spell(const char *s)
{
	RDUMP(s);
	return SpellWord(LNG_('C','S','C','Z'), String(s));
}

CONSOLE_APP_MAIN
{
	SetSpellPath("D:\\");
	FileIn in("D:\\src.txt");
	int n = 0;
	while(!in.IsEof()) {
		String s = in.GetLine();
		if(n++ % 10000 == 0)
			Cout() << n << ' ' << s << "\n";
		if(!Spell(s)) {
			RLOG("Error: " << s);
			Cout() << "Error: " << s << '\n';
			break;
		}
		s << "asdf";
		if(Spell(s)) {
			RLOG("Error: " << s);
			Cout() << "Error: " << s << '\n';
			break;
		}
	}
}
