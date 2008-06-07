#include "CppBase.h"

NAMESPACE_UPP

#ifdef _MSC_VER
#pragma inline_depth(255)
#pragma optimize("t", on)
#endif

#define LLOG(x)
#define LTIMING(x)  // RTIMING(x)

void CppWordsHash::AddWord(const String& s)
{
	int i = GetHashValue(s) & 127;
	w[i >> 5] |= 1 << (i & 31);
}

void CppWordsHash::AddWords(const char *s)
{
	CParser p(s);
	while(p)
		if(p.IsId())
			AddWord(p.ReadId());
		else
			p.SkipTerm();
}

CppWordsHash AllCppWords()
{
	CppWordsHash h;
	h.SetAll();
	return h;
}

CppItem& CppNest::GetAdd(const String& _key, const String& _name)
{
	int i = key.Find(_key);
	if(i < 0) {
		key.Add(_key);
		name.Add(_name);
		return item.Add();
	}
	return item[i];
}

bool CppBase::IsType(int i) const
{
	return GetKey(i) != "::";
}

Nestfo::Nestfo(const CppBase& base, int nesti)
	: nesti(nesti), base(base)
{
	Init();
}

Nestfo::Nestfo(int nesti, const CppBase& base)
	: nesti(nesti), base(base)
{
	Init();
}

Nestfo::Nestfo(const CppBase& base, const String& nest)
	: nesti(base.Find(nest)), base(base)
{
	Init();
}

Nestfo::Nestfo(const Nestfo& f)
	: base(f.base)
{
	nests <<= f.nests;
	bvalid = nvalid = false;
	nesti = f.nesti;
}

void Nestfo::Init()
{
	bvalid = nvalid = false;
}

void Nestfo::Bases(int i, Vector<int>& g)
{
	LTIMING("GetBases");
	if(base.IsType(i)) {
		const CppNest& n = base.nest[i];
		for(int i = 0; i < n.GetCount(); i++) {
			const CppItem& im = n[i];
			if(im.IsType()) {
				const char *q = im.qptype;
				const char *b = q;
				for(;;) {
					if(*q == ';' || *q == '\0') {
						if(b < q) {
							int nq = base.Find(String(b, q));
							if(nq >= 0)
								g.Add(nq);
						}
						if(*q == '\0')
							return;
						q++;
						b = q;
					}
					else
						q++;
				}
			}
		}
	}
}

const Vector<String>& Nestfo::GetBases()
{
	if(!bvalid) {
		bvalid = true;
		baselist.Clear();
		if(nesti < 0)
			return baselist;
		Vector<int> b;
		Index<int> bi;
		Bases(nesti, b);
		while(b.GetCount()) {
			Vector<int> bb;
			for(int i = 0; i < b.GetCount(); i++) {
				int q = b[i];
				if(bi.Find(q) < 0) {
					bi.Add(q);
					Bases(b[i], bb);
				}
			}
			b = bb;
		}
		for(int i = 0; i < bi.GetCount(); i++)
			baselist.Add(base.GetKey(bi[i]) + "::");
	}
	return baselist;
}


const Vector<String>& Nestfo::GetNests()
{
	if(!nvalid) {
		nvalid = true;
		nests.Clear();
		if(nesti < 0)
			return nests;
		String nn = base.GetKey(nesti);
		while(nn.GetCount()) {
			if(nn[0] == ':' && nn.GetCount() == 2) {
				nests.Add(nn);
				return nests;
			}
			nests.Add(nn + "::");
			int q = nn.ReverseFind(':');
			nn.Trim(max(0, q - 1));
		}
		nests.Add("::");
	}
	return nests;
}

String Qualify0(Nestfo& nf, const String& type)
{
	if(IsNull(type) || type == "const" ||
	   type == "int" || type == "double" || type == "long" || type == "char" || type == "void")
		return type;
	const Vector<String>& nd = nf.GetNests();
	if(type[0] == ':') {
		if(nf.base.Find(type) >= 0)
			return type;
	}
	else
	if(nd.GetCount()) {
		LTIMING("First test");
		String qt = nd[0] + type;
		if(nf.base.Find(qt) >= 0)
			return qt;
	}
	if(nf.GetNest() >= 0) {
		int q = type.ReverseFind(':');
		if(q >= 0) {
			LTIMING("Qualifying qualification");
			Nestfo hnf(nf);
			hnf.NoBases();
			String qn = Qualify(hnf, type.Mid(0, max(q - 1, 0)));
			if(qn[0] != ':')
				return type;
			int nesti = nf.base.Find(qn);
			if(nesti < 0)
				return type;
			String tp = type.Mid(q + 1);
			Nestfo nnf(nf.base, nesti);
			const Vector<String>& bs = nnf.GetBases();
			for(int i = 0; i < bs.GetCount(); i++) {
				String qt = bs[i] + tp;
				if(nf.base.Find(qt) >= 0)
					return qt;
			}
		}
		else {
			const Vector<String>& bs = nf.GetBases();
			for(int i = 0; i < bs.GetCount(); i++) {
				String qt = bs[i] + type;
				if(nf.base.Find(qt) >= 0)
					return qt;
			}
		}
	}
	if(type[0] != ':') {
		LTIMING("Testing nests");
		for(int i = 1; i < nd.GetCount(); i++) {
			String qt = nd[i] + type;
			if(nf.base.Find(qt) >= 0)
				return qt;
		}
	}
	return type;
}

String Qualify(Nestfo& nf, const String& type)
{
	int q = nf.cache.Find(type);
	if(q >= 0)
		return nf.cache[q];
	String x = Qualify0(nf, type);
	nf.cache.Add(type, x);
	return x;
}

String QualifyIds(Nestfo& nf, const String& k, CppWordsHash& w)
{
	String r;
	CParser p(k);
	Vector<String> empty;
	while(p) {
		if(p.IsChar2(':', ':')) {
			String t;
			while(p.Char2(':', ':')) {
				t << "::";
				if(p.IsId()) {
					String id = p.ReadId();
					w.AddWord(id);
					t << id;
				}
			}
			Nestfo nnf(nf.GetNest(), nf.base);
			t = Qualify(nnf, t);
			if(iscid(*r.Last()) && iscid(*t))
				r << ' ';
			r << t;
		}
		else
		if(p.IsId()) {
			String t = p.ReadId();
			w.AddWord(t);
			while(p.Char2(':', ':')) {
				t << "::";
				if(p.IsId()) {
					String id = p.ReadId();
					w.AddWord(id);
					t << id;
				}
			}
			t = Qualify(nf, t);
			if(iscid(*r.Last()) && iscid(*t))
				r << ' ';
			r << t;
		}
		else {
			int c = p.GetChar();
			r.Cat(c);
			p.Spaces();
		}
	}
	return r;
}

String Qualify(const CppBase& base, const String& nest, const String& type)
{
	CppWordsHash dummy;
	Nestfo nf(base, nest);
	return QualifyIds(nf, type, dummy);
}

String QualifyKey(const CppBase& base, const String& nest, const String& type)
{
	CppWordsHash dummy;
	Nestfo nf(base, nest);
	return QualifyIds(nf, type, dummy);
}

void QualifyTypes(Nestfo& nf, CppItem& m)
{
	m.qtype = QualifyIds(nf, m.type, m.words);
	m.qptype = QualifyIds(nf, m.ptype, m.words);
}

void QualifyTypes(CppBase& base, const String& nest, CppItem& m)
{
	Nestfo nf(base, nest);
	QualifyTypes(nf, m);
}

void QualifyPass1(CppBase& base, const CppWordsHash& words)
{
	LTIMING("Qualify1");
	for(int ni = 0; ni < base.GetCount(); ni++) {
		CppNest& n = base[ni];
		Nestfo nf(base, ni);
		for(int i = 0; i < n.GetCount(); i++) {
			CppItem& m = n.item[i];
			if((m.words.IsAll() || (m.words & words)) && m.IsType()) {
				m.words.Clear();
				QualifyTypes(nf, m);
			}
		}
	}
}

void QualifyPass2(CppBase& base, const CppWordsHash& words)
{
	LTIMING("Qualify2");
	for(int ni = 0; ni < base.GetCount(); ni++) {
		CppNest& n = base[ni];
		Nestfo nf(base, ni);
		Index<int> rem;
		for(int i = 0; i < n.GetCount(); i++) {
			CppItem& m = n.item[i];
			if((m.words.IsAll() || (words & m.words)) && !m.IsType()) {
				m.words.Clear();
				QualifyTypes(nf, m);
				if(m.IsCode()) {
					String k = n.key[i];
					String r = QualifyIds(nf, m.key, m.words);
					if(k != r) {
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
		Vector<int> rm = rem.PickKeys();
		Sort(rm);
		n.Remove(rm);
	}
}

void   Qualify(CppBase& base, const CppWordsHash& words)
{
	LTIMING("Qualify");
	QualifyPass1(base, words);
	QualifyPass2(base, words);
}

void Remove(CppBase& base, const Vector<String>& pf)
{
	int ni = 0;
	Vector<int> file;
	for(int i = 0; i < pf.GetCount(); i++)
		file.Add(GetCppFileIndex(pf[i]));
	while(ni < base.GetCount()) {
		CppNest& n = base[ni];
		Vector<int> nr;
		for(int mi = 0; mi < n.GetCount(); mi++) {
			CppItem& m = n.item[mi];
			int i = 0;
			while(i < m.pos.GetCount())
				if(FindIndex(file, m.pos[i].file) >= 0)
					m.pos.Remove(i);
				else
					i++;
			if(m.pos.GetCount() == 0)
				nr.Add(mi);
		}
		n.Remove(nr);
		if(n.GetCount() == 0)
			base.nest.Remove(ni);
		else
			ni++;
	}
}

void CppItem::Serialize(Stream& s)
{
	s % kind % access;
	s % natural % at % tparam % param % pname % tname % type % ptype % virt % key;
}

END_UPP_NAMESPACE
