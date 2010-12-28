#include "ValueCtrl.h"

Ctrl* DefaultValueEditor(int vtype)
{
	String t;
	Ctrl* p = NULL;
	switch(vtype)
	{
		case INT_V: p = new EditInt(); break;
		case DOUBLE_V: p = new EditDouble(); break;
		case STRING_V: p = new EditString(); break;
		case DATE_V: p = new DropDate(); break;
		case TIME_V: p = new DropTime(); break;
		case WSTRING_V: p = new RichTextCtrl(); break;
		case INT64_V: p = new EditInt64(); break;
		case BOOL_V: { Option* _p = new Option(); _p->ClickFocus(); p = _p; } break;
		
		case COLOR_V: p = new ColorPusher(); break;
		//case FONT_V: p = new FontPusher(); break; //FIXME

		case LOGPOS_V: p = new LogPosCtrl(); break;
		case VALUE_V: p = new ValuePacker; break;
		case VALUEARRAY_V:  p = new ValueArrayCtrl(); break;
		
		case VOID_V: p = new ValueCtrl(); break;

		case VALUEMAP_V: t = "VALUEMAP_V"; break;
		
		case ERROR_V: t = "ERROR_V"; break;
		case UNKNOWN_V: t = "UNKNOWN_V"; break;
		default: break;
	}
	if(!p)
	{
		StaticText* _p = new StaticText();
		_p->SetText(t);
		p = _p;
	}
	return p;
}

Ctrl* DefaultValueEditor(const Value& v)
{
	Ctrl* p = DefaultValueEditor(v.GetType());
	p->SetData(v);
	return p;
}

void ValuePopUp::SetType(int _vt)
{
	if(_vt == vtype) return;
	v = Value(); //reset
	if(_vt==VOID_V) //prevent loop
	{
		if(pc) return;
		_vt = STRING_V; //default, if no editor yet
	}
	pc = DefaultValueEditor(_vt);
	vtype = _vt;
	ASSERT(pc);	
	Add(pc->HSizePos().VSizePos(0,20));
	pc->WhenAction = THISBACK(DataAction);
	WhenTypeChange;
}

void ValuePopUp::Updated()
{
	int _vt = v.GetType();
	if(_vt != vtype && !v.IsNull())
		SetType(_vt);
	if(type.GetData() != vtype)
	   type.SetData(vtype);
	pc->SetData(v);
}

void ValuePopUp::TypeAction()
{
	int t = type.GetData();
	SetType(t);
	UpdateAction();
}

ValuePopUp::ValuePopUp()
	: vtype(-1)
{
	CtrlLayout(*this);
	SetFrame(BlackFrame());

	type.Add(int(VOID_V), "VOID_V (reset data only)");
	type.Add(int(INT_V), "INT_V");
	type.Add(int(DOUBLE_V), "DOUBLE_V");
	type.Add(int(STRING_V), "STRING_V");
	type.Add(int(DATE_V), "DATE_V");
	type.Add(int(TIME_V), "TIME_V");
	type.Add(int(WSTRING_V), "WSTRING_V");
	type.Add(int(INT64_V), "INT64_V");
	type.Add(int(BOOL_V), "BOOL_V");
	type.Add(int(COLOR_V), "COLOR_V");
	type.Add(int(LOGPOS_V), "LOGPOS_V");
	//type.Add(int(VALUE_V), "VALUE_V");
	type.Add(int(VALUEARRAY_V), "VALUEARRAY_V");
	type <<= THISBACK(TypeAction);

	ok <<= THISBACK(Acceptor);
	cancel <<= THISBACK(Rejector);
	
	SetType(VOID_V);
}

ValueCtrl::ValueCtrl() 
	: push(false)
{
	IgnoreMouse(false);
	SetFrame(BlackFrame());

	vp.WhenSelect = THISBACK(AcceptDrop);
	vp.WhenCancel = THISBACK(CloseDrop);
	vp.WhenAction = THISBACK(ActionDrop);
	
	Update();
}

void ValueCtrl::LeftDown(Point p, dword keyflags)
{
	if(!HasFocus()) SetFocus();
	Drop();	
}

void ValueCtrl::Drop()
{
	if(push) return;
	push = true;
	vs = v;
	vp.PopUp(this, v);
}

void ValueCtrl::CloseDrop()
{
	push = false;
	if(v != vs) {
		v = vs;
		UpdateAction();
	}
}

void ValueCtrl::AcceptDrop()
{
	push = false;
	v = vp.GetData();
	UpdateAction();
}

void ValueCtrl::ActionDrop()
{
	v = vp.GetData();
	UpdateAction();
}

void ValueCtrl::Updated()
{
	if(v.IsNull())
		SetText("#Nil#");
	else
		SetText(v.ToString());
}
