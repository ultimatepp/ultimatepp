#include "PropList.h"

NAMESPACE_UPP

PropListCtrl::PropListCtrl()
{
	CtrlLayout(*this);
}

void PropListCtrl::Updated()
{
	gl.SetData(Null);
	sl.SetData(Null);
	if(!ctrl) return;

	Ctrl& e = *ctrl;

	Value v;
	bool b;
	String t;

	b = Props<Ctrl>::Get(e, "listset", v);
	ValueArray vs = v;
	t << "Set Properties: (" << vs.GetCount() << ")\n" << v;
	gl.SetData(t);

	v = Value();
	b = Props<Ctrl>::Get(e, "listget", v);
	t.Clear();
	ValueArray vg = v;
	t << "Get Properties: (" << vg.GetCount() << ")\n";
	{
		for(int i = 0; i < vg.GetCount(); i++)
		{
			String s = vg.Get(i);
			Value v;
			t << s << " = ";
			b = Props<Ctrl>::Get(e, s, v);
			if(b) t << v;
			else t << "##ERR##";
			t << "\n";
		}
	}
	sl.SetData(t);
}

PropList::PropList()
{
	SetRect(plc.GetRect());
	Add(plc.HSizePos().VSizePos(0,20));
}

END_UPP_NAMESPACE
