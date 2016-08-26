#include "CtrlLib.h"


namespace Upp {

Size  LNGCtrl::GetMinSize() const
{
	int c = EditField::GetStdHeight();
	int d = 9 * c / 4;
	return Size(2 * d + 2, hi.GetMinSize().cy);
}

Value LNGCtrl::GetData() const
{
	return ~lo;
}

void  LNGCtrl::SetData(const Value& v)
{
	int l = v;
	hi <<= l & LNG_(0xff, 0xff, 0, 0);
	LoadLo();
	lo <<= l;
	Update();
}

void LNGCtrl::LoadLo() {
	lo.Clear();
	Index<int> h;
	int hv = ~hi;
	int set = 0;
	if(hv == 0) {
		lo.Add(0, "--");
		lo <<= 0;
		return;
	}
	lo.Add(hv, "");
	for(const int *l = GetAllLanguages(); *l; l++)
		if((*l & LNG_(0xff, 0xff, 0, 0)) == hv) {
			h.FindAdd(*l);
			if(!set)
				set = *l;
		}
	Vector<int> q = h.PickKeys();
	Sort(q);
	for(int i = 0; i < q.GetCount(); i++)
		lo.Add(q[i], LNGAsText(q[i]).Mid(3));
	lo <<= set;
}

void LNGCtrl::Hi()
{
	LoadLo();
	UpdateAction();
}

void LNGCtrl::Lo()
{
	UpdateAction();
}

LNGCtrl::LNGCtrl() {
	int c = EditField::GetStdHeight();
	int d = 9 * c / 4;
	Add(hi.LeftPos(0, d).VSizePos());
	Add(lo.LeftPos(d + 2, d).VSizePos());

	Index<int> h;
	for(const int *l = GetAllLanguages(); *l; l++)
		h.FindAdd(*l & LNG_(0xff, 0xff, 0, 0));
	Vector<int> q = h.PickKeys();
	Sort(q);
	hi.Add(0, "--");
	for(int i = 0; i < q.GetCount(); i++)
		hi.Add(q[i], LNGAsText(q[i]));
	hi <<= THISBACK(Hi);
	lo <<= THISBACK(Lo);
}

LNGCtrl::~LNGCtrl()
{
}

}
