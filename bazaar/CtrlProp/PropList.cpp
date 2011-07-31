#include "PropList.h"

PropListCtrl::PropListCtrl()
{
	CtrlLayout(*this);
}

void PropListCtrl::Reload()
{
	V::Reload();
	Ctrl& e = Get();

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
	Add(exit.RightPos(0, 64).BottomPos(0, 20));
	exit.SetLabel("Close");
	exit <<= THISBACK(Acceptor);
	//cancel <<= THISBACK(Rejector);
}
