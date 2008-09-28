#include "CppBase.h"

NAMESPACE_UPP

#define LLOG(x)
#define LTIMING(x)  // RTIMING(x)

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
	s % natural % at % tparam % param % pname % tname % ctname % type % ptype % virt % key;
}

END_UPP_NAMESPACE
