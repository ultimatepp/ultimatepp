#include "CppBase.h"

NAMESPACE_UPP

#define LLOG(x)
#define LTIMING(x)  // RTIMING(x)

void CppItem::Serialize(Stream& s)
{
	s % kind % access
	  % item % name % natural % at % tparam % param % pname
	  % tname % ctname % type % ptype % virt % line % impl;
}

struct CmpItem {
	bool operator()(const CppItem& a, const String& b) const
	{
		return a.qitem < b;
	}
};

int FindItem(const Array<CppItem>& x, const String& qitem)
{
	int q = FindLowerBound(x, qitem, CmpItem());
	if(q < x.GetCount() && x[q].qitem == qitem)
		return q;
	return -1;
}

int FindNext(const Array<CppItem>& x, int i)
{
	if(i >= x.GetCount())
		return i;
	String q = x[i].qitem;
	while(i < x.GetCount() && x[i].qitem == q)
		i++;
	return i;
}

int GetCount(const Array<CppItem>& x, int i)
{
	return FindNext(x, i) - i;
}

int FindName(const Array<CppItem>& x, const String& name, int i)
{
	while(i < x.GetCount()) {
		if(x[i].name == name)
			return i;
		i++;
	}
	return -1;
}

bool CppBase::IsType(int i) const
{
	return GetKey(i).GetCount();
}

void Remove(CppBase& base, const Vector<String>& pf)
{
	int ni = 0;
	Index<int> file;
	for(int i = 0; i < pf.GetCount(); i++)
		file.Add(GetCppFileIndex(pf[i]));
	while(ni < base.GetCount()) {
		Array<CppItem>& n = base[ni];
		Vector<int> nr;
		for(int i = 0; i < n.GetCount(); i++)
			if(file.Find(n[i].file) >= 0)
				nr.Add(i);
		if(nr.GetCount() == n.GetCount())
			base.Remove(ni);
		else {
			n.Remove(nr);
			ni++;
		}
	}
}

END_UPP_NAMESPACE
