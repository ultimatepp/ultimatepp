#include "PropEdit.h"

PropEditCtrl::PropEditCtrl()
{
	CtrlLayout(*this);
	ac.AddColumn("Property");
	ac.AddColumn("Value");
	//ac.WhenUpdateRow = THISBACK(OnUpdateRow);
}

void PropEditCtrl::Updated()
{
	if(ctrl && sctrl == ctrl)
	{
		for(int i = 0; i < ac.GetCount(); i++)
		{
			Value v;
			int ii = am.Find(ac.Get(i, 0));
			if(ii<0) continue;
			if(!am[ii].get(v)) continue;
			ac.Set(i, 1, v);
		}
		return;
	}
	sctrl = ctrl;
	ac.Clear();
	vsav.Clear();
	am.Clear();	
	if(!ctrl) return;

	Ctrl& e = *ctrl;

	//init local accessor map
	//prefer the dynamic access
	//fall back to the static accessors from Props<>
	GetAccessorMapI* ami = dynamic_cast<GetAccessorMapI*>(&e);
	if(ami) am <<= ami->GetAccessorMap();
	else if(!Props<Ctrl>::SetupAccessorMap(e, am)) return;

	for(int i = 0, k = 0; i < am.GetCount(); i++)
	{
		String tag = am.GetKey(i);
		ValueAccessor& a = am[i];
		Value v;

		if(a.get && !a.get(v)) //if cant get a gettable
			continue;

		ac.Set(k, 0, tag);
		ac.Set(k, 1, v); //forwarded to controls when specified

		if(a.get && a.set)	
		{
			Tuple2<bool, Value>& tv = vsav.Add(tag);
			tv.a = false; tv.b = v; //save old values
		
			Ctrl* pc;
			if(!v.IsNull())
			{
				pc = DefaultValueEditor(v);
				{
					//LogPosCtrl needs instance to live show changes
					LogPosCtrl* ple = dynamic_cast<LogPosCtrl*>(pc);
					if(ple) ple->Set(e);
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
		else
		{
			ASSERT(0); //Property without accessors
			++k;	
		}
	}
	ac.Layout();
	//ac.UpdateRefresh();
}

void PropEditCtrl::OnUpdateRow()
{
	if(!ctrl) return;
	if(!ac.IsCursor()) return; //FIXME Option Focus issue 
	int ii = am.Find(ac.Get(0));
	if(ii<0) return;
	am[ii].set(ac.Get(1));
	vsav.GetAdd(ac.Get(0)).a = true; //dirty
	Action();
	ctrl->UpdateActionRefresh(); //propagate user action
}

void PropEditCtrl::Undo()
{
	if(!ctrl) return;
	for(int i = 0; i < vsav.GetCount(); i++)
		if(vsav[i].a)
		{
			int ii = am.Find(vsav.GetKey(i));
			if(ii<0) continue;
			am[ii].set(vsav[i].b);				                 
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
