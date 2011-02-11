#include "CtrlPropEdit.h"

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

void PropList::Acceptor()
{
	plc.Clear();	
	PopUpC::Acceptor();
}

//

PropEditCtrl::PropEditCtrl()
{
	CtrlLayout(*this);
	ac.AddColumn("Property");
	ac.AddColumn("Value");
	//ac.WhenUpdateRow = THISBACK(OnUpdateRow);
}

void PropEditCtrl::Visit(Ctrl& e)
{
	V::Visit(e);

	ac.Clear();
	vsav.Clear();
	am.Clear();	

	bool b = Props<Ctrl>::SetupAccessorMap(e, am);
	if(!b) return;

	int k = 0;
	for(int i = 0; i < am.GetCount(); i++)
	{
		String tag = am.GetKey(i);
		ValueAccessor& a = am[i];

		Value v;
		if(a.get)
		{
			if(!a.get(v))
				continue;
		}
		ac.Set(k, 0, tag);
		ac.Set(k, 1, v); //forwarded to controls when specified

		if(a.get && a.set)	
			{
				Tuple2<bool, Value>& tv = vsav.Add(tag);
				tv.a = false;
				tv.b = v;
			
				Ctrl* pc;
				if(!v.IsNull())
				{
					pc = DefaultValueEditor(v);
					{
						LogPosCtrl* ple = dynamic_cast<LogPosCtrl*>(pc);
						if(ple)
							ple->Set(e);
					}
				}
				else pc = new ValueCtrl();
				(*pc) <<= THISBACK(OnUpdateRow);

				ac.SetCtrl(k, 1, pc); //owned
				++k;
			}
		else
		if(a.get)
			{
				StaticText* pc = new StaticText();
				pc->SetText(AsString(v));

				ac.SetCtrl(k, 1, pc); //owned
				++k;
			}
		else
		if(a.set)
			{
				//FIXME needs to know which type
				//meanwhile, we let user choose
				ValueCtrl* pc = new ValueCtrl();
				(*pc) <<= THISBACK(OnUpdateRow);

				ac.SetCtrl(k, 1, pc); //owned
				++k;
			}
	}
	ac.Layout();
	//ac.UpdateRefresh();
}

void PropEditCtrl::Clear()
{
	V::Clear();
	ac.Clear();
	vsav.Clear();	
}

void PropEditCtrl::Reload()
{
	V::Reload();
	Ctrl& e = Get();
	for(int i = 0; i < ac.GetCount(); i++)
	{
		Value v;
		int ii = am.Find(ac.Get(i, 0));
		if(ii<0) continue;
		if(!am[ii].get(v)) continue;
		ac.Set(i, 1, v);
	}
}

void PropEditCtrl::OnUpdateRow()
{
	if(IsEmpty()) return;
	if(!ac.IsCursor()) return; //FIXME Option Focus issue 
	int ii = am.Find(ac.Get(0));
	if(ii<0) return;
	am[ii].set(ac.Get(1));
	vsav.GetAdd(ac.Get(0)).a = true; //dirty
	Action();
	Get().UpdateActionRefresh();
}

void PropEditCtrl::Restore()
{
	if(!IsEmpty())
	{
		for(int i = 0; i < vsav.GetCount(); i++)
			if(vsav[i].a)
			{
				int ii = am.Find(vsav.GetKey(i));
				if(ii<0) continue;
				am[ii].set(vsav[i].b);				                 
			}
	}
}

PropEdit::PropEdit()
{
	SetRect(pec.GetRect());
	Add(pec.HSizePos().VSizePos(0,20));
	Add(ok.RightPos(60,60).BottomPos(0,20));
	Add(cancel.RightPos(0,60).BottomPos(0,20));
	ok <<= THISBACK(Acceptor);
	cancel <<= THISBACK(Rejector);
	ok.SetLabel("OK");
	cancel.SetLabel("Cancel");
}

void PropEdit::Rejector()
{
	pec.Restore();
	pec.Clear();	
	PopUpC::Rejector();
}

void PropEdit::Acceptor()
{
	pec.Clear();	
	PopUpC::Acceptor();
}
