#include "CtrlPropCommon.h"

VectorMap<String, int> BidirProps(Ctrl& c)
{
	VectorMap<String, int> is;

	Value vs;
	Props<Ctrl>::Get(c, "listset", vs);
	if(!vs.Is<ValueArray>()) return is;
	ValueArray vas = vs;
	for(int i = 0; i < vas.GetCount(); i++)
	{
		int& m = is.GetAdd(vas.Get(i), 0);
		m |= 1;
	}

	Value vg;
	Props<Ctrl>::Get(c, "listget", vg);
	if(!vg.Is<ValueArray>()) return is;
	ValueArray vag = vg;
	for(int i = 0; i < vag.GetCount(); i++)
	{
		int& m = is.GetAdd(vag.Get(i), 0);
		m |= 2;
	}

	return is;
}
