#include "CtrlPropEdit.h"

VectorMap<String, int> BidirProps(Ctrl& c)
{
	VectorMap<String, int> is;

	Value vs;
	GetProperty(c, "listset", vs);
	if(!vs.Is<ValueArray>()) return is;
	ValueArray vas = vs;
	for(int i = 0; i < vas.GetCount(); i++)
	{
		int& m = is.GetAdd(vas.Get(i), 0);
		m |= 1;
	}

	Value vg;
	GetProperty(c, "listget", vg);
	if(!vg.Is<ValueArray>()) return is;
	ValueArray vag = vg;
	for(int i = 0; i < vag.GetCount(); i++)
	{
		int& m = is.GetAdd(vag.Get(i), 0);
		m |= 2;
	}

	return is;
}

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

	b = GetProperty(e, "listset", v);
	ValueArray vs = v;
	t << "Set Properties: (" << vs.GetCount() << ")\n" << v;
	gl.SetData(t);

	v = Value();
	b = GetProperty(e, "listget", v);
	t.Clear();
	ValueArray vg = v;
	t << "Get Properties: (" << vg.GetCount() << ")\n";
	{
		for(int i = 0; i < vg.GetCount(); i++)
		{
			String s = vg.Get(i);
			Value v;
			t << "{" << s << "} = ";
			b = GetProperty(e, s, v);
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

	VectorMap<String, int> is = BidirProps(e);
	
	int k = 0;
	for(int i = 0; i < is.GetCount(); i++)
	{
		String tag = is.GetKey(i);
		int& m = is[i];

		Value v;
		if(m&2)
		{
			if(!GetProperty(e, tag, v))
				continue;
		}
		ac.Set(k, 0, tag);
		ac.Set(k, 1, v); //forwarded to controls
	
		switch(m)
		{
			case 3: //bidir
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
				pc->WhenAction = THISBACK(OnUpdateRow);

				ac.SetCtrl(k, 1, pc); //owned
				++k;
			}
				break;
			case 2: //getter only
			{
				StaticText* pc = new StaticText();
				pc->SetText(AsString(v));

				ac.SetCtrl(k, 1, pc); //owned
				++k;
			}
				break;
			case 1: //setter only
			{
				//FIXME needs to know which type
				//meanwhile, we let user choose
				ValueCtrl* pc = new ValueCtrl();
				pc->WhenAction = THISBACK(OnUpdateRow);

				ac.SetCtrl(k, 1, pc); //owned
				++k;
			}
				break;
			default:
				break;
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
		if(!GetProperty(e, ac.Get(i, 0), v)) continue;
		ac.Set(i, 1, v);
	}
}

void PropEditCtrl::OnUpdateRow()
{
	if(IsEmpty()) return;
	if(!ac.IsCursor()) return; //FIXME Option Focus issue 
	SetProperty(Get(), ac.Get(0), ac.Get(1));
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
				SetProperty(Get(), vsav.GetKey(i), vsav[i].b);
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
