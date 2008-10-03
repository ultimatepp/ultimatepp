#include "CppBase.h"

NAMESPACE_UPP

#define LLOG(x)
#define LTIMING(x)  // RTIMING(x)

Nestfo::Nestfo(const CppBase& base, int nesti)
	: nesti(nesti), base(base)
{
	LTIMING("Nestfo(const CppBase& base, int nesti)");
	Init();
}

Nestfo::Nestfo(int nesti, const CppBase& base)
	: nesti(nesti), base(base)
{
	LTIMING("Nestfo(int nesti, const CppBase& base)");
	Init();
}

Nestfo::Nestfo(const CppBase& base, const String& nest)
	: nesti(base.Find(nest)), base(base)
{
	LTIMING("Nestfo(const CppBase& base, const String& nest)");
	Init();
}

Nestfo::Nestfo(const Nestfo& f)
	: base(f.base)
{
	LTIMING("Nestfo copy contructor");
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
	LTIMING("GetBases");
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
	LTIMING("GetNests");
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
		nests.Add("");
	}
	return nests;
}

END_UPP_NAMESPACE
