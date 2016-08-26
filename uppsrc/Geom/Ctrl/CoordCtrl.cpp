#include "GeomCtrl.h"

namespace Upp {

EditDegree::EditDegree()
{
	AddFrame(spin);
	spin.inc.WhenRepeat = spin.inc.WhenPush = THISBACK(OnInc);
	spin.dec.WhenRepeat = spin.dec.WhenPush = THISBACK(OnDec);
}

void EditDegree::OnInc()
{
	double a = GetData();
	a = floor(Nvl(a) + 1.5);
	if(a >= Nvl(GetMax(), +360.0))
		a = Nvl(GetMin(), -360.0);
	SetData(a);
	Action();
}

void EditDegree::OnDec()
{
	double a = GetData();
	a = ceil(Nvl(a) - 1.5);
	if(a <= Nvl(GetMin(), -360.0))
		a = Nvl(GetMax(), +360.0);
	SetData(a);
	Action();
}

bool EditDegree::Key(dword key, int repcnt)
{
	if(key == K_ALT_UP)
	{
		OnInc();
		return true;
	}
	if(key == K_ALT_DOWN)
	{
		OnDec();
		return true;
	}
	return EditValue<double, ConvertDegree>::Key(key, repcnt);
}

}
