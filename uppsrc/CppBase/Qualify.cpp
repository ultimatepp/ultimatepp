#include "CppBase.h"

NAMESPACE_UPP

#define LLOG(x)     // DLOG(x)
#define LTIMING(x)  // RTIMING(x)

bool DoQualify(Scopefo& nf, const String& type, String& qt);

bool Qualify0(Scopefo& nf, const String& type, String& qt)
{
	const Vector<String>& nd = nf.GetScopes();
	if(nd.GetCount()) {
		LTIMING("First test");
		qt = nd[0] + type;
		if(nf.base.Find(qt) >= 0)
			return true;
	}
	if(nf.GetScope() >= 0) {
		int q = type.ReverseFind(':');
		if(q > 0) {
			LTIMING("Qualifying qualification");
			Scopefo hnf(nf);
			hnf.NoBases();
			String qn;
			String qs = type.Mid(0, q - 1);
			if(IsDigit(*qs)) {
				qt = type;
				return true;
			}
			if(DoQualify(hnf, qs, qn)) {
				if(nf.base.Find(type) >= 0) {
					qt = type;
					return true;
				}
				int scopei = nf.base.Find(qn);
				if(scopei >= 0) {
					String tp = type.Mid(q + 1);
					Scopefo nnf(nf.base, scopei);
					const Vector<String>& bs = nnf.GetBases();
					for(int i = 0; i < bs.GetCount(); i++) {
						qt = bs[i] + tp;
						if(nf.base.Find(qt) >= 0)
							return true;
					}
				}
			}
			qt = type.Mid(q + 1);
			return true;
		}
		else {
			LTIMING("Bases");
			const Vector<String>& bs = nf.GetBases();
			for(int i = 0; i < bs.GetCount(); i++) {
				qt = bs[i] + type;
				if(nf.base.Find(qt) >= 0)
					return true;
			}
		}
	}
	if(type[0] != ':') {
		LTIMING("Testing scopes");
		for(int i = 1; i < nd.GetCount(); i++) {
			qt = nd[i] + type;
			if(nf.base.Find(qt) >= 0)
				return true;
		}
	}
	int q = type.Find(':');
	if(q < 0)
		return false;
	return Qualify0(nf, type.Mid(q + 1), qt);
}

bool DoQualify(Scopefo& nf, const String& type, String& qt)
{
	LTIMING("Qualify");
	int q = nf.cache.Find(type);
	if(q >= 0) {
		qt = nf.cache[q];
		return true;
	}
	LTIMING("Qualify0");
	if(!Qualify0(nf, type, qt))
		return false;
	nf.cache.Add(type, qt);
	return true;
}

String DoQualify(Scopefo& nf, const String& type)
{
	String qt;
	return DoQualify(nf, type, qt) ? qt : type;
}

static String s_int("int");
static String s_void("void");
static String s_double("double");
static String s_char("char");
static String s_float("float");
static String s_long("long");
static String s_const("const");
static String s_struct("struct");
static String s_class("class");
static String s_unsigned("unsigned");

inline void Qualify(String& r, Scopefo& nf, const char *b, const char *s, byte& qual)
{
	String type(b, s);
	if(type.GetCount() == 0 || type == s_const ||
	   type == s_int || type == s_double || type == s_char ||
	   type == s_long || type == s_unsigned || type == s_struct || type == s_class ||
	   type == s_float) {
		r << type;
		return;
	}
	r << DoQualify(nf, type);
}

String QualifyIds(Scopefo& nf, const String& k, bool all, byte& qual)
{
	LTIMING("QualifyIds");
	String r;
	const char *s = k;
	Vector<String> empty;
	while(*s) {
		int c = *s;
		if(c == ':') {
			const char *b = s++;
			while(*s == ':' || iscid(*s)) s++;
			if(all) {
				if(iscid(*r.Last()))
					r << ' ';
				Scopefo nnf(nf.GetScope(), nf.base);
				Qualify(r, nnf, b, s, qual);
			}
			else
				r.Cat(b, s);
		}
		else
		if(iscid(c)) {
			if(iscid(*r.Last()))
				r << ' ';
			if(s[0] == 'c' && s[1] == 'o' && s[2] == 'n' && s[3] == 's' && s[4] == 't' && !iscid(s[5])) {
				r << s_const;
				s += 5;
			}
			else
			if(s[0] == 'm' && s[1] == 'u' && s[2] == 't' && s[3] == 'a' && s[4] == 'b' && s[5] == 'l' && s[6] == 'e' && !iscid(s[7])) {
				r << "mutable";
				s += 7;
			}
			else
			if(s[0] == 'v' && s[1] == 'o' && s[2] == 'l' && s[3] == 'a' && s[4] == 't' && s[5] == 'i' && s[6] == 'l' && s[7] == 'e' && !iscid(s[8])) {
				r << "volatile";
				s += 8;
			}
			else {
				const char *b = s++;
				while(*s == ':' || iscid(*s)) s++;
				if(all)
					Qualify(r, nf, b, s, qual);
				else
					r.Cat(b, s);
			}
		}
		else {
			if(c == '(')
				all = true;
			if(c != ' ')
				r.Cat(c);
			s++;
		}
	}
	return r;
}

String Qualify(const CppBase& base, const String& scope, const String& type)
{
	Scopefo nf(base, scope);
	byte dummy = 2;
	return QualifyIds(nf, type, true, dummy);
}

String QualifyKey(const CppBase& base, const String& scope, const String& type)
{
	Scopefo nf(base, scope);
	byte dummy = 2;
	return QualifyIds(nf, type, false, dummy);
}

void QualifyTypes(CppBase& base, const String& scope, CppItem& m)
{
	Scopefo nf(base, scope);
	m.qtype = QualifyIds(nf, m.type, true, m.qualify_type);
	m.qptype = QualifyIds(nf, m.ptype, true, m.qualify_param);
}

void QualifyPass1(CppBase& base)
{
	LTIMING("QualifyPass1");
	for(int ni = 0; ni < base.GetCount(); ni++) {
		Array<CppItem>& n = base[ni];
		Scopefo nf(base, ni);
		for(int i = 0; i < n.GetCount(); i++) {
			CppItem& m = n[i];
			if(m.serial != base.serial && m.IsType()) {
				m.serial = base.serial;
				if(m.qualify_type) {
					m.qualify_type = false;
					m.qtype = QualifyIds(nf, m.type, true, m.qualify_type);
				}
				if(m.qualify_param) {
					m.qualify_param = false;
					m.qptype = QualifyIds(nf, m.ptype, true, m.qualify_param);
				}
				m.qitem = m.item;
			}
			//LLOG(base.GetKey(ni) << "." << m.item << " " << GetCppFile(m.file) << '(' << m.line << ") "
			//     << AsCString(m.natural)
			//     << " impl:" << m.impl << " kind:" << (int)m.kind << " IsType:" << m.IsType()
			//     << " type: " << m.type << " qtype:" << m.qtype << " tparam:" << m.tparam);
		}
	}
}

struct CmpCppItem {
	bool operator()(const CppItem& a, const CppItem& b) const
	{
		int q = SgnCompare(a.qitem, b.qitem);
		if(q) return q < 0;
		q = SgnCompare(a.IsType(), b.IsType());
		if(q) return q < 0;
		q = SgnCompare(a.impl, b.impl);
		if(q) return a.IsType() ? q > 0 : q < 0;
		q = SgnCompare(GetCppFile(a.file), GetCppFile(b.file));
		if(q) return q < 0;
		return a.line < b.line;
	}
};

void QualifyPass2(CppBase& base)
{
	LTIMING("QualifyPass2");
	for(int ni = 0; ni < base.GetCount(); ni++) {
		Array<CppItem>& n = base[ni];
		Scopefo nf(base, ni);
		Index<int> rem;
		bool sort = false;
		for(int i = 0; i < n.GetCount(); i++) {
			CppItem& m = n[i];
			if(m.uname.GetCount() == 0 && m.name.GetCount())
				m.uname = ToUpper(m.name);
			if(m.serial != base.serial && !m.IsType()) {
				sort = true;
				m.serial = base.serial;
				if(m.qualify_type) {
					m.qualify_type = false;
					m.qtype = QualifyIds(nf, m.type, true, m.qualify_type);
				}
				if(m.qualify_param) {
					m.qualify_param = false;
					m.qptype = QualifyIds(nf, m.ptype, true, m.qualify_param);
					m.qitem = m.IsCode() ? QualifyIds(nf, m.item, false, m.qualify_param)
					                     : m.item;
				}
			}
			LLOG(base.GetKey(ni) << "." << m.item << " -> " << m.qitem << "\n   " << GetCppFile(m.file) << '(' << m.line << ") "
			     << AsCString(m.natural)
			     << " impl:" << m.impl << " kind:" << (int)m.kind << " IsType:" << m.IsType()
			     << " qtype:" << m.qtype << " tparam:" << m.tparam);
		}
		if(sort)
			Sort(n, CmpCppItem());
	}
}

void   Qualify(CppBase& base)
{
	Md5Stream md5;
	Vector<int> no = GetSortOrder(base.GetKeys());
	for(int q = 0; q < base.GetCount(); q++) {
		int ni = no[q];
		md5 << base.GetKey(ni);
		const Array<CppItem>& n = base[ni];
		for(int i = 0; i < n.GetCount(); i++) {
			const CppItem& m = n[i];
			if(m.IsType())
				md5 << ';' << m.ptype;
		}
		md5 << '\n';
	}
	String c5 = md5.FinishString();
	if(c5 != base.serial_md5) {
		base.serial++;
		base.serial_md5 = c5;
	}
	QualifyPass1(base);
	QualifyPass2(base);
}

END_UPP_NAMESPACE
