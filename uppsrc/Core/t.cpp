#include "Core.h"

namespace Upp {

#define TFILE <Core/Core.t>
#include <Core/t.h>

static StaticMutex slng;

static int sIdLen(const char *txt)
{
	const char *s;
	for(s = txt; *s; s++)
		if(*s == '\a' && s[1] != '\a')
			break;
	return (int)(s - txt);
}

static bool sIdEq(const char *a, const char *b)
{
	for(;;) {
		if((*a == '\0' || *a == '\a' && a[1] != '\a') &&
		   (*b == '\0' || *b == '\a' && b[1] != '\a'))
			return true;
		if(*a != *b)
			return false;
		a++;
		b++;
	}
}

struct CharS : Moveable<CharS> {
	const char *s;

	hash_t GetHashValue() const             { return memhash(s, sIdLen(s)); }
	bool operator==(const CharS& b) const   { return sIdEq(s, b.s); }
};

struct LngRec : Moveable<LngRec> {
	int         lang;
	const char *text;
};

struct LngModule {
	String                            name;
	VectorMap<CharS, Vector<LngRec> > map;
};

static Array<LngModule>& sMod()
{
	static Array<LngModule> m;
	return m;
}

const char *GetENUS(const char *txt)
{
	for(const char *s = txt; *s; s++) {
		if(*s == '\v') {
			if(s[1] == '\v')
				s++;
			else
				return s + 1;
		}
		if(*s == '\a') {
			if(s[1] == '\a')
				s++;
			else
				return s + 1;
		}
	}
	return txt;
}

const char *GetENUSc(const char *txt)
{
	txt = GetENUS(txt);
	String r;
	bool cv = false;
	for(const char *s = txt; *s; s++)
		if(s[0] == '\v' && s[1] == '\v') {
			r.Cat('\v');
			s++;
			cv = true;
		}
		else
		if(s[0] == '\a' && s[1] == '\a') {
			r.Cat('\a');
			s++;
			cv = true;
		}
		else
			r.Cat(*s);
	return cv ? PermanentCopy(r) : txt;
}

void AddModule(const LngEntry__* l, const char *name)
{
	Mutex::Lock __(slng);
	Array<LngModule>& ma = sMod();
	LngModule& m = ma.Add();
	m.name = name;
	Vector<LngRec> *lr = NULL;
	while(l->lang) {
		if(l->lang == 1) {
			CharS ids;
			ids.s = l->text;
			lr = &m.map.GetAdd(ids);
			LngRec& r = lr->Add();
			r.lang = LNG_('E','N','U','S');
			r.text = GetENUSc(l->text);
		}
		else
		if(l->text && *l->text && lr) {
			LngRec& r = lr->Add();
			r.lang = l->lang;
			r.text = l->text;
		}
		l++;
	}
}

// ----------------------------------------------------

static const LngRec *sFindLngRec(const char *id, int lang, dword mask)
{
	CharS ids;
	ids.s = id;
	lang &= mask;
	Array<LngModule>& ma = sMod();
	for(int i = 0; i < ma.GetCount(); i++) {
		LngModule& m = ma[i];
		int q = m.map.Find(ids);
		if(q >= 0) {
			const Vector<LngRec>& r = m.map[q];
			for(int i = 0; i < r.GetCount(); i++)
				if(int(r[i].lang & mask) == lang)
					return &r[i];
		}
	}
	return NULL;
}

static const LngRec *sFindLngRec(const char *id, int lang)
{
	const LngRec *r;
	r = sFindLngRec(id, lang, LNGC_(0xff, 0xff, 0xff, 0xff, 0));
	if(r)
		return r;
	r = sFindLngRec(id, lang, LNGC_(0xff, 0xff, 0x00, 0x00, 0));
	return r;
}

struct ZoneAlloc {
	Vector<char *> zsmall, zbig;
	char *ptr, *lim;

	char *Alloc(int sz);
	void  Clear();

	ZoneAlloc()  { ptr = lim = NULL; }
	~ZoneAlloc() { Clear(); }
};

void ZoneAlloc::Clear()
{
	for(int i = 0; i < zsmall.GetCount(); i++)
		delete[] zsmall[i];
	for(int i = 0; i < zbig.GetCount(); i++)
		delete[] zbig[i];
	zsmall.Clear();
	zbig.Clear();
	ptr = lim = NULL;
}

char *ZoneAlloc::Alloc(int sz)
{
	if(sz >= 1024)
		return zbig.Add() = new char[sz];
	if(ptr + sz >= lim) {
		zsmall.Add() = ptr = new char[4096];
		lim = ptr + 4096;
	}
	char *s = ptr;
	ptr += sz;
	return s;
}

template<>
inline hash_t GetHashValue(const char * const &s)
{
	return GetPtrHashValue(s);
}

int main_current_lang;

static Index<int>& sLangIndex()
{
	static Index<int> m;
	return m;
}

static Array< VectorMap<const char *, const char *> >& sLangMap()
{
	static Array< VectorMap<const char *, const char *> > m;
	return m;
}

VectorMap<const char *, const char *> *sMainCurrentLangMapPtr;
thread_local VectorMap<const char *, const char *> *sCurrentLangMapPtr;

VectorMap<const char *, const char *>& sCurrentLangMap()
{
	if(sCurrentLangMapPtr)
		return *sCurrentLangMapPtr;
	if(sMainCurrentLangMapPtr)
		return *sMainCurrentLangMapPtr;
	return sLangMap().At(0);
}

static Array< VectorMap<String, String> >& sSLangMap()
{
	static Array< VectorMap<String, String> > m;
	return m;
}

VectorMap<String, String>          *sMainCurrentSLangMapPtr;
thread_local VectorMap<String, String> *sCurrentSLangMapPtr;

VectorMap<String, String>& sCurrentSLangMap()
{
	if(sCurrentLangMapPtr)
		return *sCurrentSLangMapPtr;
	if(sMainCurrentSLangMapPtr)
		return *sMainCurrentSLangMapPtr;
	return sSLangMap().At(0);
}

thread_local int thread_current_lang;

int GetCurrentLanguage()
{
	return thread_current_lang ? thread_current_lang : main_current_lang;
}

void SetCurrentLanguage(int lang)
{
	{
		Mutex::Lock __(slng);
		thread_current_lang = lang;
		int ii = sLangIndex().FindAdd(lang);
		sCurrentLangMapPtr = &sLangMap().At(ii);
		sCurrentSLangMapPtr = &sSLangMap().At(ii);
		if(Thread::IsMain())
		{
			main_current_lang = lang;
			sMainCurrentSLangMapPtr = sCurrentSLangMapPtr;
			sMainCurrentLangMapPtr = sCurrentLangMapPtr;
		}
		static int n = 1;
		if(ii > n) { // protected against too many language/charset switches
			n = 2 * n;
			Single<ZoneAlloc>().Clear();
			for(int i = 0; i < sLangIndex().GetCount(); i++) {
				sSLangMap()[i].Clear();
				sLangMap()[i].Clear();
			}
		}
		SyncLngInfo__();
	}
	SetDateFormat(t_("date-format\a%2:02d/%3:02d/%1:4d"));
	SetDateScan(t_("date-scan\amdy"));
	SetDateFilter(t_("date-filter\aA/\a .-"));
}

const char *t_GetLngString_(const char *id)
{
	VectorMap<const char *, const char *>& map = sCurrentLangMap();
	int q = map.Find(id);
	if(q >= 0)
		return map[q];
	const LngRec *r = sFindLngRec(id, GetCurrentLanguage());
	if(r) {
		int dch = GetLNGCharset(GetCurrentLanguage());
		if(dch == CHARSET_UTF8) {
			map.Add(id, r->text);
			return r->text;
		}
		String text = ToCharset(dch, r->text, CHARSET_UTF8);
		char *q = Single<ZoneAlloc>().Alloc(text.GetLength() + 1);
		strcpy(q, ~text);
		map.Add(id, q);
		return q;
	}

	const char *txt = GetENUSc(id);
	map.Add(id, txt);
	return txt;
}

const char *t_GetLngString(const char *id)
{
	Mutex::Lock __(slng);
	return t_GetLngString_(id);
}

String GetLngString_(int lang, const char *id)
{
	if(!lang)
		lang = GetCurrentLanguage();
	const LngRec *r = sFindLngRec(id, lang);
	if(r) {
		int dch = GetLNGCharset(lang);
		if(dch == CHARSET_UTF8)
			return r->text;
		else
			return ToCharset(dch, r->text, CHARSET_UTF8);
	}
	return GetENUSc(id);
}

String GetLngString(int lang, const char *id)
{
	Mutex::Lock __(slng);
	return GetLngString_(lang, id);
}

String GetLngString(const char *id)
{
	Mutex::Lock __(slng);
	VectorMap<String, String>& map = sCurrentSLangMap();
	int q = map.Find(id);
	if(q >= 0)
		return map[q];
	String s = GetLngString_(GetCurrentLanguage(), id);
	map.Add(id, s);
	return s;
}

Index<int> GetLngSet()
{
	Mutex::Lock __(slng);
	Index<int> ndx;
	Array<LngModule>& ma = sMod();
	for(int i = 0; i < ma.GetCount(); i++) {
		LngModule& m = ma[i];
		for(int j = 0; j < m.map.GetCount(); j++) {
			Vector<LngRec>& lr = m.map[j];
			for(int k = 0; k < lr.GetCount(); k++)
				ndx.FindAdd(lr[k].lang);
		}
	}
	return ndx;
}

Index<int> GetLngSet(const String& module)
{
	Mutex::Lock __(slng);
	Index<int> ndx;
	Array<LngModule>& ma = sMod();
	for(int i = 0; i < ma.GetCount(); i++) {
		LngModule& m = ma[i];
		if (m.name != module)
			continue;
		
		for (int j = 0; j < m.map.GetCount(); ++j) {
			Vector<LngRec>& lr = m.map[j];
			for(int k = 0; k < lr.GetCount(); k++)
				ndx.FindAdd(lr[k].lang);
		}
	}
	
	return ndx;
}

void    SaveLngFile(FileOut& out, int lang, int lang2)
{
	Mutex::Lock __(slng);
	out << "LANGUAGE " << AsCString(LNGAsText(lang)) << ";\r\n";
	Array<LngModule>& ma = sMod();
	for(int i = 0; i < ma.GetCount(); i++) {
		LngModule& m = ma[i];
		out << "// " << m.name << "\r\n";
		for(int j = 0; j < m.map.GetCount(); j++) {
			String id = m.map.GetKey(j).s;
			if (lang2)
				out << "// " << AsCString(GetLngString_(lang2, id), 60, "\t", ASCSTRING_SMART) << "\r\n";
			out << AsCString(id, 70) << ",\r\n"
			    << "\t" << AsCString(GetLngString_(lang, id), 60, "\t", ASCSTRING_SMART)
			    << ";\r\n\r\n";
		}
	}
}

void LngSetAdd(const char *id, int lang, const char *txt, bool addid)
{
	Mutex::Lock __(slng);
	CharS ids;
	ids.s = PermanentCopy(id);
	String text = ToCharset(CHARSET_UTF8, txt, GetLNGCharset(lang));
	lang = SetLNGCharset(lang, CHARSET_UTF8);
	Array<LngModule>& ma = sMod();
	for(int i = 0; i < ma.GetCount(); i++) {
		LngModule& m = ma[i];
		int q = m.map.Find(ids);
		if(q >= 0) {
			char *t = PermanentCopy(text);
			Vector<LngRec>& r = m.map[q];
			for(int i = 0; i < r.GetCount(); i++)
				if(r[i].lang == lang) {
					r[i].text = t;
					return;
				}
			LngRec& rec = r.Add();
			rec.lang = lang;
			rec.text = t;
			return;
		}
	}
	if(addid) {
		if(ma.GetCount() == 0)
			ma.Add();
		LngRec& r = ma.Top().map.Add(ids).Add();
		char *t = PermanentCopy(text);
		strcpy(t, text);
		r.lang = lang;
		r.text = t;
	}
}

bool   LoadLngFile(const char *file)
{
	String data = LoadFile(file);
	CParser p(data);
	try {
		while(!p.IsEof()) {
			p.PassId("LANGUAGE");
			int lang = LNGFromText(p.ReadString());
			if(!lang)
				return false;
			p.PassChar(';');
			while(p.IsString()) {
				String id = p.ReadString();
				p.PassChar(',');
				LngSetAdd(id, lang, p.ReadString(), false);
				p.PassChar(';');
			}
		}
	}
	catch(CParser::Error) {
		return false;
	}
	return true;
}

}
