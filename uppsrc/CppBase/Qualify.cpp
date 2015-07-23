#include "CppBase.h"

NAMESPACE_UPP

#define LLOG(x)     // DLOG(x)
#define LTIMING(x)  // RTIMING(x)

bool DoQualify(ScopeInfo& nf, const String& type, const String& usings, String& qt);

bool Qualify0(ScopeInfo& nf, const String& type, const String& usings, String& qt)
{ // Qualify single type based on scoping information
	const Vector<String>& nd = nf.GetScopes(usings);
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
			ScopeInfo hnf(nf);
			hnf.NoBases();
			String qn;
			String qs = type.Mid(0, q - 1);
			if(IsDigit(*qs)) {
				qt = type;
				return true;
			}
			if(DoQualify(hnf, qs, usings, qn)) {
				String tp = type.Mid(q + 1);
				if(nf.base.Find(qn) >= 0) {
					qt = qn + "::" + tp;
					return true;
				}
				int scopei = nf.base.Find(qn);
				if(scopei >= 0) {
					ScopeInfo nnf(nf.base, scopei);
					const Vector<String>& bs = nnf.GetBases();
					for(int i = 0; i < bs.GetCount(); i++) {
						qt = bs[i] + tp;
						if(nf.base.Find(qt) >= 0)
							return true;
					}
				}
			}
			if(nf.base.Find(qs) >= 0) {
				qt = qs;
				return true;
			}
			qt = type;
			if(nf.base.Find(qt) >= 0) // e.g. std::string
				return true;
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
	return Qualify0(nf, type.Mid(q + 1), usings, qt);
}

bool DoQualify(ScopeInfo& nf, const String& type, const String& usings, String& qt)
{
	LTIMING("Qualify");
	int q = nf.cache.Find(type);
	if(q >= 0) {
		qt = nf.cache[q];
		return true;
	}
	LTIMING("Qualify0");
	if(!Qualify0(nf, type, usings, qt))
		return false;
	nf.cache.Add(type, qt);
	return true;
}

String DoQualify(ScopeInfo& nf, const String& type, const String& usings)
{
	String qt;
	return DoQualify(nf, type, usings, qt) ? qt : type;
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

inline void Qualify(String& r, ScopeInfo& nf, const char *b, const char *s, const String& usings)
{
	String type(b, s);
	if(type.GetCount() == 0 || type == s_const ||
	   type == s_int || type == s_double || type == s_char ||
	   type == s_long || type == s_unsigned || type == s_struct || type == s_class ||
	   type == s_float) {
		r << type;
		return;
	}
	r << DoQualify(nf, type, usings);
}

String QualifyIds(ScopeInfo& nf, const String& k, const String& usings, bool all)
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
/*			if(all) {
				if(iscid(*r.Last()))
					r << ' ';
				ScopeInfo nnf(nf.GetScope(), nf.base);
				Qualify(r, nnf, b, s, usings);
			}
			else*/
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
					Qualify(r, nf, b, s, usings);
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

String Qualify(const CppBase& base, const String& scope, const String& type, const String& usings)
{
	ScopeInfo nf(base, scope);
	return QualifyIds(nf, type, usings, true);
}

String QualifyKey(const CppBase& base, const String& scope, const String& type, const String& usings)
{
	ScopeInfo nf(base, scope);
	return QualifyIds(nf, type, usings, false);
}

void QualifyTypes(CppBase& base, const String& scope, CppItem& m)
{
	ScopeInfo nf(base, scope);
	m.qtype = QualifyIds(nf, m.type, m.using_namespaces, true);
	m.qptype = QualifyIds(nf, m.ptype, m.using_namespaces, true);
}

void QualifyPass1(CppBase& base)
{ // Qualify types
	LTIMING("QualifyPass1");
	for(int ni = 0; ni < base.GetCount(); ni++) {
		Array<CppItem>& n = base[ni];
		ScopeInfo nf(base, ni);
		for(int i = 0; i < n.GetCount(); i++) {
			CppItem& m = n[i];
			if(m.kind == NAMESPACE)
				base.namespaces.FindAdd(base.GetKey(ni));
			if(m.IsType() && m.qualify) {
				m.qualify = false;
				m.qtype = QualifyIds(nf, m.type, m.using_namespaces, true); // type of item, now qualified (probaly already was)
				m.qptype = QualifyIds(nf, m.ptype, m.using_namespaces, true); // base classes
				m.qitem = m.item; // name of type (struct) item is already qualified
			}
		}
	}
}

void QualifyPass2(CppBase& base)
{ // Qualify function parameters
	LTIMING("QualifyPass2");
	for(int ni = 0; ni < base.GetCount(); ni++) {
		Array<CppItem>& n = base[ni];
		ScopeInfo nf(base, ni);
		for(int i = 0; i < n.GetCount(); i++) {
			CppItem& m = n[i];
			if(m.uname.GetCount() == 0 && m.name.GetCount())
				m.uname = ToUpper(m.name);
			if(!m.IsType() && m.qualify) {
				m.qualify = false;
				m.qtype = QualifyIds(nf, m.type, m.using_namespaces, true);
				m.qptype = QualifyIds(nf, m.ptype, m.using_namespaces, true);
				m.qitem = m.IsCode() ? QualifyIds(nf, m.item, m.using_namespaces, false)
				                     : m.item;
			}
		}
	}
}

void Qualify(CppBase& base)
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
	if(c5 != base.types_md5) { // set of types changed, requalify everything
		base.types_md5 = c5;
		for(int ni = 0; ni < base.GetCount(); ni++) {
			Array<CppItem>& n = base[ni];
			for(int i = 0; i < n.GetCount(); i++)
				n[i].qualify = true;
		}
	}
	base.namespaces.Clear();
	QualifyPass1(base);
	QualifyPass2(base);
}

END_UPP_NAMESPACE
