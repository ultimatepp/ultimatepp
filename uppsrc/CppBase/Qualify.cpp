#include "CppBase.h"

NAMESPACE_UPP

#define LLOG(x)
#define LTIMING(x)  // RTIMING(x)

bool DoQualify(Nestfo& nf, const String& type, String& qt);

bool Qualify0(Nestfo& nf, const String& type, String& qt)
{
	const Vector<String>& nd = nf.GetNests();
	if(nd.GetCount()) {
		LTIMING("First test");
		qt = nd[0] + type;
		if(nf.base.Find(qt) >= 0)
			return true;
	}
	if(nf.GetNest() >= 0) {
		int q = type.ReverseFind(':');
		if(q > 0) {
			LTIMING("Qualifying qualification");
			Nestfo hnf(nf);
			hnf.NoBases();
			String qn;
			if(DoQualify(hnf, type.Mid(0, q - 1), qn)) {
				int nesti = nf.base.Find(qn);
				if(nesti >= 0) {
					String tp = type.Mid(q + 1);
					Nestfo nnf(nf.base, nesti);
					const Vector<String>& bs = nnf.GetBases();
					for(int i = 0; i < bs.GetCount(); i++) {
						qt = bs[i] + tp;
						if(nf.base.Find(qt) >= 0)
							return true;
					}
				}
			}
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
		LTIMING("Testing nests");
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

bool DoQualify(Nestfo& nf, const String& type, String& qt)
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

String DoQualify(Nestfo& nf, const String& type)
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

inline void Qualify(String& r, Nestfo& nf, const char *b, const char *s, byte& qual)
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

String QualifyIds(Nestfo& nf, const String& k, bool all, byte& qual)
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
				Nestfo nnf(nf.GetNest(), nf.base);
				Qualify(r, nnf, b, s, qual);
			}
			else
				r.Cat(b, s);
		}
		else
		if(iscid(c)) {
			const char *b = s++;
			while(*s == ':' || iscid(*s)) s++;
			if(iscid(*r.Last()))
				r << ' ';
			if(all)
				Qualify(r, nf, b, s, qual);
			else
				r.Cat(b, s);
		}
		else {
			if(c == '(')
				all = true;
			r.Cat(c);
			s++;
			while(*s == ' ') s++;
		}
	}
	return r;
}

String Qualify(const CppBase& base, const String& nest, const String& type)
{
	Nestfo nf(base, nest);
	byte dummy = 2;
	return QualifyIds(nf, type, true, dummy);
}

String QualifyKey(const CppBase& base, const String& nest, const String& type)
{
	Nestfo nf(base, nest);
	byte dummy = 2;
	return QualifyIds(nf, type, false, dummy);
}

void QualifyTypes(CppBase& base, const String& nest, CppItem& m)
{
	Nestfo nf(base, nest);
	m.qtype = QualifyIds(nf, m.type, true, m.qualify_type);
	m.qptype = QualifyIds(nf, m.ptype, true, m.qualify_param);
}

void QualifyPass1(CppBase& base)
{
	LTIMING("QualifyPass1");
	for(int ni = 0; ni < base.GetCount(); ni++) {
		CppNest& n = base[ni];
		Nestfo nf(base, ni);
		for(int i = 0; i < n.GetCount(); i++) {
			CppItem& m = n.item[i];
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
			}
		}
	}
}

void QualifyPass2(CppBase& base)
{
	LTIMING("QualifyPass2");
	for(int ni = 0; ni < base.GetCount(); ni++) {
		CppNest& n = base[ni];
		Nestfo nf(base, ni);
		Index<int> rem;
		String prev_type, prev_qtype;
		byte   prev_qualify = 0;
		for(int i = 0; i < n.GetCount(); i++) {
			CppItem& m = n.item[i];
			if(m.serial != base.serial && !m.IsType()) {
				m.serial = base.serial;
				if(m.qualify_type) {
					if(m.type == prev_type) {
						m.qtype = prev_qtype;
						m.qualify_type = prev_qualify;
					}
					else {
						m.qualify_type = false;
						m.qtype = QualifyIds(nf, m.type, true, m.qualify_type);
						prev_type = m.type;
						prev_qtype = m.qtype;
						prev_qualify = m.qualify_type;
					}
				}
				if(m.qualify_param) {
					m.qualify_param = false;
					m.qptype = QualifyIds(nf, m.ptype, true, m.qualify_param);
					if(m.IsCode()) {
						String k = n.key[i];
						String r = QualifyIds(nf, m.key, false, m.qualify_param);
						if(k != r) {
							LTIMING("Key adjustment");
							int q = n.key.Find(r);
							if(q >= 0 && q != i)
								if(m.decla) {
									m.pos.Append(n.item[q].pos);
									rem.FindAdd(q);
									n.key.Set(i, r);
								}
								else {
									n.item[q].pos.Append(m.pos);
									rem.FindAdd(i);
								}
							else
								n.key.Set(i, r);
						}
					}
				}
			}
		}
		LTIMING("Key removal");
		Vector<int> rm = rem.PickKeys();
		Sort(rm);
		n.Remove(rm);
	}
}

void   Qualify(CppBase& base)
{
	Md5Stream md5;
	Vector<int> no = GetSortOrder(base.nest.GetKeys());
	for(int q = 0; q < base.GetCount(); q++) {
		int ni = no[q];
		md5 << base.GetKey(ni);
		const CppNest& n = base.nest[ni];
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
