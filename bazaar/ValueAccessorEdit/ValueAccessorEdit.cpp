#include "ValueAccessorEdit.h"

NAMESPACE_UPP

void ValueAccessorEdit::Serialize(Stream& s)
{
	if(!tg) return;
	Value v;
	if(tg->get && !tg->get(v)) return; //if cant get a gettable
	if(!tg->get || !tg->set) return;
	if(s.IsStoring())
		s % v;
	if(s.IsLoading())
	{
		Value _v;
		s % _v;
		if(v != _v)
			tg->set(_v);
	}
}

//

//if nesseeccary, changes the editor of the accessor get/set type underneath
void ValueAccessorCtrl::UpdateTarget()
{
	oc.Clear();
	if(!tg) return;
	
	Value v;
	if(tg->get && !tg->get(v)) //if cant get a gettable
		return;

	if(tg->get && tg->set)	
	{
		DefaultValueEditor(oc, v);
	}
	else if(tg->get)
	{
		StaticText* pc = new StaticText(); pc->SetText(AsString(v)); oc = pc;
	}
	else if(tg->set)
	{
		//FIXME needs to know which type
		//meanwhile, we let user choose
		ValueCtrl* pc = new ValueCtrl(); oc = pc;
	}
	else
	{
		ASSERT(0); //Property without accessors
		return;
	}
	(*oc) <<= THISBACK(OnAction);
	Add(oc->SizePos());
}

void ValueAccessorCtrl::Updated()
{
	if(!tg) return;
	Value v;
	if(!tg->get(v)) return;
	if(oc.IsEmpty()) return;
	oc->SetData(v);
}

//

AccessorMapCtrl::AccessorMapCtrl()
	: am(NULL)
{
	CtrlLayout(*this);
	ac.AddColumn("Property");
	ac.AddColumn("Value");
	ac.AddColumn("Reload").Ctrls(THISBACK(ReloadFactory));
	ac.ColumnWidths("3 6 1");
}

void AccessorMapCtrl::ReloadFactory(int i, One<Ctrl>& o)
{
	Button* b = new Button();
	b->SetImage(CtrlImg::Toggle);
	b->WhenAction = THISBACK1(ReloadAction, i);
	o.Attach(b);
}

void AccessorMapCtrl::ReloadAction(int i)
{
	Ctrl* c = ac.GetCtrl(i, 1);
	if(!c) return;
	c->Update();
}

void AccessorMapCtrl::Updated()
{
	for(int i = 0; i < ac.GetCount(); i++)
		ReloadAction(i);
}

void AccessorMapCtrl::UpdateMap()
{
	ac.Clear();
	avae.Clear();
	if(!am) return;

	for(int i = 0; i < am->GetCount(); i++)
	{
		String tag = am->GetKey(i);
		ValueAccessor& a = (*am)[i];

		ac.Set(i, 0, tag);

		ValueAccessorEdit* ep = NULL;
		if(ValueAccessorInstance* vi = dynamic_cast<ValueAccessorInstance*>(&a))
		{
			//special ValueAccessors, need their own editor
			String q = vi->TypeOf(); //defines which editor
			
			Instancer<ValueAccessorEdit>::MapType& map = Instancer<ValueAccessorEdit>::Map();
			int ix = map.Find(q);
			if(ix>=0)
				ep = map[ix]();
		}

		if(!ep) //second chance, default
			ep = new ValueAccessorCtrl(); 

		ASSERT(ep);
		ep->SetTarget(&a);

		(*ep) <<= Proxy(WhenAction);
		ValueAccessorEdit& c = avae.Add(tag, ep);
		ac.SetCtrl(i, 1, c, false); //independant control
	}
	ac.Layout();
	//ac.UpdateRefresh();
}

void AccessorMapCtrl::Serialize(Stream& s)
{
	GuiLock __;
	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext())
		q->Serialize(s);
	for(int i = 0; i < avae.GetCount(); i++)
		avae[i].Serialize(s);
}

END_UPP_NAMESPACE
