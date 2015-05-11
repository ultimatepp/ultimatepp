#include "CppBase.h"

NAMESPACE_UPP

#define LLOG(x)
#define LTIMING(x)  // RTIMING(x)

ScopeInfo::ScopeInfo(const CppBase& base, int scopei)
	: scopei(scopei), base(base)
{
	LTIMING("ScopeInfo(const CppBase& base, int scopei)");
	Init();
}

ScopeInfo::ScopeInfo(int scopei, const CppBase& base)
	: scopei(scopei), base(base)
{
	LTIMING("ScopeInfo(int scopei, const CppBase& base)");
	Init();
}

ScopeInfo::ScopeInfo(const CppBase& base, const String& scope)
	: scopei(base.Find(scope)), base(base)
{
	LTIMING("ScopeInfo(const CppBase& base, const String& scope)");
	Init();
}

ScopeInfo::ScopeInfo(const ScopeInfo& f)
	: base(f.base)
{
	LTIMING("ScopeInfo copy contructor");
	scopes <<= f.scopes;
	bvalid = nvalid = false;
	scopei = f.scopei;
}

void ScopeInfo::Init()
{
	bvalid = nvalid = false;
}

void ScopeInfo::Bases(int i, Vector<int>& g)
{ // recursively retrieve all base classes
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
							String h = String(b, q);
							int q = h.Find('<');
							if(q >= 0)
								h.Trim(q);
							h = TrimBoth(h);
							int nq = base.Find(h);
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

const Vector<String>& ScopeInfo::GetBases()
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
			b = pick(bb);
		}
		for(int i = 0; i < bi.GetCount(); i++)
			baselist.Add(base.GetKey(bi[i]) + "::");
	}
	return baselist;
}

const Vector<String>& ScopeInfo::GetScopes(const String& usings_)
{
	LTIMING("GetScopes");
	if(!nvalid || usings != usings_) {
		usings = usings_;
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
		Vector<String> h = Split(usings, ';');
		for(int i = 0; i < h.GetCount(); i++)
			scopes.Add(h[i] + "::");
	}
	return scopes;
}

END_UPP_NAMESPACE
