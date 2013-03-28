#include "RichEdit.h"

NAMESPACE_UPP

#define LLOG(x) // LOG(x)

struct SpellBlock : Moveable<SpellBlock> {
	String first;
	int    offset;
	int    ctrl_len;
	int    text_len;
};

struct Speller {
	String        data;
	byte          charset;
	const char   *voc[256];
	int           dict;
	struct Line : Moveable<Line> {
		const byte *begin;
		const byte *end;
	};
	VectorMap<dword, Line> line;
	Index<WString> user;

	String            path;
	Array<SpellBlock> block;
	
	bool     SetOld(const String& data);
	void     Clear()                        { data.Clear(); path.Clear(); }
	operator bool() const                   { return !data.IsEmpty() || path.GetCount(); }
	bool     CheckOld(const WString& wstr) const;

	String Get(int offset, int len);
};

bool Speller::SetOld(const String& _data)
{
	data = _data;
	const char *s = data;
	if(s >= data.End()) {
		data.Clear();
		return false;
	}
	charset = *s++;
	s++;// reserved for prefixes
	dict = *s++;
	for(int i = 0; i < 256 - dict; i++) {
		if(s >= data.End()) {
			data.Clear();
			return false;
		}
		voc[i] = s;
		while(*s) s++;
		s++;
	}
	line.Clear();
	while(s < data.End()) {
		if(s + 8 >= data.End()) {
			data.Clear();
			return false;
		}
		int code = Peek32le(s);
		s += 4;
		int len = Peek32le(s);
		s += 4;
		Line& l = line.Add(code);
		l.begin = (const byte *)s;
		s += len;
		l.end = (const byte *)s;
	};
	return true;
}

bool Speller::CheckOld(const WString& wstr) const
{
	int len = wstr.GetLength();
	if(len == 1)
		return true;
	if(len < 64) {
		String w = FromUnicode(wstr, charset);
		String wl = FromUnicode(ToLower(wstr), charset);
		int i;
		if(len == 2) {
			w.Cat(127);
			wl.Cat(127);
		}
		i = line.Find(ToLower(wl[0], charset) +
		              (ToLower(wl[1], charset) << 8) +
		              (ToLower(wl[2], charset) << 16));
		if(i >= 0) {
			const byte *s = line[i].begin;
			const byte *e = line[i].end;
			String q;
			while(s < e)
				if(*s < dict) {
					if(q == w || q == wl)
						return true;
					q.Trim(*s++);
				}
				else {
					ASSERT(*s >= dict);
					const char *x = voc[(int)*s++ - dict];
					q.Cat(x);
				}
			if(q == w || q == wl)
				return true;
		}
	}
	return user.Find(wstr) >= 0;;
}

static String sUserFile(int lang)
{
	return ConfigFile(LNGAsText(lang) + ".usp");
}

String spell_path;

void SetSpellPath(const String& p)
{
	spell_path = p;
}

static String sZet(FileIn& in, int offset, int len)
{
	in.Seek(offset);
	return ZDecompress(in.Get(len));	
}

void DoSpellerPath(String& pp, String dir)
{
	for(;;) {
		pp << dir << ';';
		String d = GetFileFolder(dir);
		if(d == dir) break;
		dir = d;
	}
}

Speller *sGetSpeller(int lang)
{
	static ArrayMap<int, Speller> speller;
	int q = speller.Find(lang);
	if(q < 0) {
		String pp = spell_path;
		DoSpellerPath(pp, GetExeDirFile("scd"));		
		DoSpellerPath(pp, ConfigFile("scd"));
		pp << spell_path << ';' << getenv("LIB") << ';' << getenv("PATH") << ';';
		String path = GetFileOnPath(ToLower(LNGAsText(lang)) + ".udc", pp);
		if(IsNull(path))
			path = GetFileOnPath(ToLower(LNGAsText(lang)) + ".scd", pp);
		if(IsNull(path))
			return NULL;
		FileIn in(path);
		if(!in)
			return NULL;
		q = speller.GetCount();
		Speller& f = speller.Add(lang);
		FileIn user(sUserFile(lang));
		while(!user.IsEof()) {
			String s = user.GetLine();
			if(!s.IsEmpty())
				f.user.Add(FromUtf8(s));
		}
		if(in.Get() != 255)
			f.SetOld(LoadFile(path));
		else {
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
	}
	return &speller[q];
}

bool SpellWordRaw(const WString& wrd, int lang, Vector<String> *withdia)
{
	Speller *f = sGetSpeller(lang);
	if(!f)
		return true;
	if(f->data.GetCount())
		return f->CheckOld(wrd);
	String awrd = ToUpper(ToAscii(wrd).ToString());
	String t1 = ToUtf8(wrd);
	String t2 = ToUtf8(ToLower(wrd));
	for(int i = 0;; i++) {
		if(i + 1 >= f->block.GetCount() || awrd <= f->block[i + 1].first) {
			for(;;) {
				if(i >= f->block.GetCount())
					return f->user.Find(wrd) >= 0;;
				LLOG("Spell block " << i << ": " << f->block[i].first);
				const SpellBlock& b = f->block[i++];
				if(b.first > awrd) {
					LLOG("  --- end");
					return f->user.Find(wrd) >= 0;;
				}
				FileIn in(f->path);
				String ctrl = sZet(in, b.offset, b.ctrl_len);
				String text = sZet(in, b.offset + b.ctrl_len, b.text_len);
				in.Close();
				String w;
				const char *s = ctrl;
				const char *e = ctrl.End();
				const char *t = text;
				const char *te = text.End();
				while(s < e && t < te) {
					w.Trim(*s++);
					while(*t)
						w.Cat(*t++);
					if(w == t1 || w == t2)
						return true;
					if(withdia && t2 == ToLower(ToAscii(w.ToWString()).ToString()))
						withdia->Add(w);
					t++;
				}
			}
		}
	}
	return f->user.Find(wrd) >= 0;;
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
		r = SpellWordRaw(k.wrd, k.lang);
		return 1;
	}
};

static LRUCache<bool, SpellKey> speller_cache;

bool SpellWord(const WString& ws, int lang)
{
	speller_cache.Shrink(2000);
	SpellMaker m;
	m.k.lang = lang;
	m.k.wrd = ws;
	return speller_cache.Get(m);
}

bool SpellWord(const wchar *ws, int len, int lang)
{
	return SpellWord(WString(ws, len), lang);
}

void RichEdit::SpellerAdd(const WString& w, int lang)
{
	if(!SpellWord(w, w.GetCount(), lang)) {
		Speller *f = sGetSpeller(lang);
		if(f) {
			FileAppend fa(sUserFile(lang));
			fa.PutLine(ToUtf8(w));
			f->user.Add(w);
			speller_cache.Clear();
		}
	}
}

int RichEdit::fixedlang;

Bits RichEdit::SpellParagraph(const RichPara& para)
{
	int len = para.GetLength();
	Buffer<wchar> text(len);
	Buffer<int> lang(len);
	wchar *s = text;
	int *g = lang;
	for(int i = 0; i < para.GetCount(); i++) {
		const RichPara::Part& p = para[i];
		if(p.IsText()) {
			int l = p.text.GetLength();
			memcpy(s, p.text, l * sizeof(wchar));
			Fill(g, g + l, fixedlang ? fixedlang : p.format.language);
			s += l;
			g += l;
		}
		else {
			*s++ = 127;
			*g++ = 0;
		}
	}
	Bits e;
	s = text;
	wchar *end = text + len;
	while(s < end) {
		if(IsLetter(*s)) {
			const wchar *q = s;
			while(s < end && IsLetter(*s) || s + 1 < end && *s == '\'' && IsLetter(s[1]))
				s++;
			if(!SpellWord(q, int(s - q), lang[q - text]))
				e.Set(int(q - text), true, int(s - q));
		}
		else
			s++;
	}
	return e;
}

END_UPP_NAMESPACE
