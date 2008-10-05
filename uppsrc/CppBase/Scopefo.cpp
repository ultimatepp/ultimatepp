#include "CppBase.h"

NAMESPACE_UPP

#define LLOG(x)
#define LTIMING(x)  // RTIMING(x)

Scopefo::Scopefo(const CppBase& base, int scopei)
	: scopei(scopei), base(base)
{
	LTIMING("Scopefo(const CppBase& base, int scopei)");
	Init();
}

Scopefo::Scopefo(int scopei, const CppBase& base)
	: scopei(scopei), base(base)
{
	LTIMING("Scopefo(int scopei, const CppBase& base)");
	Init();
}

Scopefo::Scopefo(const CppBase& base, const String& scope)
	: scopei(base.Find(scope)), base(base)
{
	LTIMING("Scopefo(const CppBase& base, const String& scope)");
	Init();
}

Scopefo::Scopefo(const Scopefo& f)
	: base(f.base)
{
	LTIMING("Scopefo copy contructor");
	scopes <<= f.scopes;
	bvalid = nvalid = false;
	scopei = f.scopei;
}

void Scopefo::Init()
{
	bvalid = nvalid = false;
}

void Scopefo::Bases(int i, Vector<int>& g)
{
	if(base.IsType(i)) {
		const Array<CppItem>& n = base[i];
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

const Vector<String>& Scopefo::GetBases()
{
	LTIMING("GetBases");
	if(!bvalid) {
		bvalid = true;
		baselist.Clear();
		if(scopei < 0)
			return baselist;
		Vector<int> b;
		Index<int> bi;
		Bases(scopei, b);
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

const Vector<String>& Scopefo::GetScopes()
{
	LTIMING("GetScopes");
	if(!nvalid) {
		nvalid = true;
		scopes.Clear();
		if(scopei < 0)
			return scopes;
		String nn = base.GetKey(scopei);
		while(nn.GetCount()) {
			if(nn[0] == ':' && nn.GetCount() == 2) {
				scopes.Add(nn);
				return scopes;
			}
			scopes.Add(nn + "::");
			int q = nn.ReverseFind(':');
			nn.Trim(max(0, q - 1));
		}
		scopes.Add("");
	}
	return scopes;
}

END_UPP_NAMESPACE
