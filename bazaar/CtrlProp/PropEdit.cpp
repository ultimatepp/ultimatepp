#include "PropEdit.h"

PropEditCtrl::PropEditCtrl()
{
	CtrlLayout(*this);
	ac.AddColumn("Property");
	ac.AddColumn("Value");
	ac.AddColumn("Reload").Ctrls(THISBACK(ReloadFactory));
	ac.ColumnWidths("3 6 1");
	//ac.WhenUpdateRow = THISBACK(OnUpdateCurrentRow); only invoked after cursor leaves the edited row, but we need live
}

void PropEditCtrl::ReloadFactory(int i, One<Ctrl>& o)
{
	Button* b = new Button();
	b->SetImage(CtrlImg::Toggle);
	b->WhenAction = THISBACK1(ReloadAction, i);
	o.Attach(b);
}

void PropEditCtrl::ReloadAction(int i)
{
	int ii = am.Find(ac.Get(i, 0));
	if(ii<0) return;
	Value v;
	if(!am[ii].get(v)) return;
	ac.Set(i, 1, v);
}

void PropEditCtrl::Updated()
{
	for(int i = 0; i < ac.GetCount(); i++)
		ReloadAction(i);
}

void PropEditCtrl::UpdateCtrl()
{
	ac.Clear();
	am.Clear();	
	if(!ctrl) return;

	//init local accessor map
	//prefer the dynamic access
	//fall back to the static accessors from Props<>
	GetAccessorMapI* ami = dynamic_cast<GetAccessorMapI*>(ctrl);
	if(ami) am <<= ami->GetAccessorMap();
	else if(!Props<Ctrl>::SetupAccessorMap(*ctrl, am)) return;

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
			Ctrl* pc;
			if(!v.IsNull())
			{
				pc = DefaultValueEditor(v);
				{
					//LogPosCtrl needs instance infos to live show changes
					if(LogPosCtrl* ple = dynamic_cast<LogPosCtrl*>(pc))
						ple->Set(*ctrl);
				}
			}
			else pc = new ValueCtrl();
			(*pc) <<= THISBACK1(OnUpdateRow, k); //for live editing, unlike WhenUpdateRow
			ac.SetCtrl(k, 1, pc); //owned
			++k;
		}
		else if(a.get)
		{
			StaticText* pc = new StaticText();
			pc->SetText(AsString(v));
			ac.SetCtrl(k, 1, pc); //owned
			++k;
		}
		else if(a.set)
		{
			//FIXME needs to know which type
			//meanwhile, we let user choose
			ValueCtrl* pc = new ValueCtrl();
			(*pc) <<= THISBACK1(OnUpdateRow, k); //for live editing, unlike WhenUpdateRow
			ac.SetCtrl(k, 1, pc); //owned
			++k;
		}
		else
		{
			ASSERT(0); //Property without accessors
		}
	}
	ac.Layout();
	//ac.UpdateRefresh();
}

void PropEditCtrl::OnUpdateRow(int i)
{
	am[i].set(ac.Get(i, 1));
	Action();
	if(!ctrl) return;
	ctrl->UpdateActionRefresh(); //propagate user action
}

void PropEditCtrl::OnUpdateCurrentRow()
{
	if(!ac.IsCursor()) return; //FIXME Option Focus issue 
	int ii = am.Find(ac.Get(0));
	if(ii<0) return;
	OnUpdateRow(ii);	
}

//

PropEdit::PropEdit()
	: WCK(WCK::OKCANCEL)
{
	SetRect(pec.GetRect());
	Add(pec.HSizePos().VSizePos(0,20));
	//WhenDeactivate = THISBACK1(RejectBreak, int(IDCANCEL));
}

void PropEdit::Serialize(Stream& s)
{
	const AccessorMap& am = pec.GetAccessorMap();
	if(s.IsStoring())
	{
		for(int i = 0; i < am.GetCount(); i++)
		{
			const ValueAccessor& a = am[i];
			Value v;
			if(a.get && !a.get(v)) continue; //if cant get a gettable
			if(!a.get || !a.set) continue;
			RLOG("Storing: " << i << " => " << v);
			s % i % v;
		}
		int i = -1;
		s % i; //end marker
	}
	if(s.IsLoading())
	{
		int i = -1;
		do
		{
			int i = -1;
			s % i;
			if(i<0) break;
			ASSERT(i<=am.GetCount());

			Value v, _v;
			s % _v;
			
			const ValueAccessor& a = am[i];
			if(a.get && !a.get(v)) { ASSERT(0); continue; }//if cant get a gettable
			if(!a.get || !a.set) { ASSERT(0); continue; }

			RLOG("Loading: " << i << " => " << v);
			
			if(v != _v)
			{
				RLOG("setting: " << _v);
				a.set(_v);
			}
		}
		while(1);
	}
}

