#include "GridCtrl.h"

NAMESPACE_UPP

int GridCtrl::ItemRect::sortCol;
int GridCtrl::ItemRect::sortMode;

/*---------------------------------------------------*/

void GridCtrl::Item::SetCtrl(Ctrl * newCtrl)
{
	ctrl = newCtrl;
	ctrl->Hide();
	ctrl->WantFocus();
	ctrl->SetFrame(NullFrame());
	//ctrl->SetFrame(BlackFrame());
}

void GridCtrl::Item::DropCtrl()
{
	ctrl = NULL;
}

VectorMap<Id, int> *GridCtrl::ItemRect::aliases = NULL;

/*---------------------------------------------------*/

GridCtrl::ItemRect& GridCtrl::ItemRect::Alias(Id _id)
{
	aliases->SetKey(id, _id);
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Alias(const char * s)
{
	aliases->SetKey(id, s);
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Name(String &s)
{
	(*items)[0][id].val = s;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Hidden(bool b)
{
	hidden = b;
	size = 0;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Name(const char * s)
{
	(*items)[0][id].val = s;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Size(int n)
{
	hidden = n == 0;

	if(!hidden)
	{
		if(n < min) { n = min; ismin = true; }
		if(n > max) { n = max; ismax = true; }
	}
	else
		n = 0;

	size = nsize = n;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Width(int n)
{
	return Size(n);
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Height(int n)
{
	return Size(n);
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Min(int n)
{
	if(hidden)
		return *this;
	min = n;
	if(size < min)
		size = min;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Max(int n)
{
	if(hidden)
		return *this;
	max = n;
	if(size > max)
		size = max;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Fixed(int n)
{
	return Min(n).Max(n);
}

GridCtrl::ItemRect& GridCtrl::ItemRect::FixedAuto()
{
	return Fixed(GetTextSize((String)(*items)[0][id].val, StdFont()).cx + 15);
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Edit(Ctrl &ctrl)
{
	ctrl.Hide();
	ctrl.SetFrame(BlackFrame());
	//ctrl.SetFrame(NullFrame());
	(*edits)[id].ctrl = &ctrl;
	parent->holder.AddChild(&ctrl);
	return *this;
}

//GridCtrl::ItemRect& GridCtrl::ItemRect::EditConvert(Ctrl &ctrl)
//{
//	return Edit(ctrl).SetConvert(ctrl);
//}

void GridCtrl::ItemRect::ChangeSortMode(bool idsort)
{
	if(++sortmode > 2) sortmode = (int) !idsort;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::SetConvert(Convert &c)
{
	(*edits)[id].convert = &c;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::NoConvert()
{
	(*edits)[id].convert = NULL;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::NoConvertion()
{
	convertion = false;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Default(Value v)
{
	defval = v;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Index(bool b)
{
	Size(0);
	index = b;
	return *this;
}

String GridCtrl::ItemRect::GetName() const
{
	return (*items)[0][id].val;
}

Id GridCtrl::ItemRect::GetAlias() const
{
	return aliases->GetKey(id);
}

GridCtrl::ItemRect& GridCtrl::ItemRect::SetFormat(const char *fmt)
{
	//(*items)[0][id].convert = &c;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::SetDisplay(GridDisplay &gd)
{
	//(*items)[0][id].display = &gd;
	display = &gd;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::IgnoreDisplay()
{
	ignore_display = true;
	return *this;
}

void GridCtrl::ItemRect::Clear()
{
	fnt = StdFont();
	bg = Null;
	fg = Null;
}

void GridCtrl::ItemRect::Serialize(Stream &s)
{
	s % balign % halign % calign;
	s % fg % bg;
	s % fnt;
	s % id;
	s % size;
	s % prop;
	s % hidden;
}

static void MakeOption(One<Ctrl>& ctrl)
{
	ctrl.Create<Option>();
	ctrl->SetData(0);
	ctrl->WantFocus();
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Option()
{
	return Ctrls(MakeOption).CtrlAlignHorzCenter(CtrlsImg::O0().GetSize().cx);
}

GridCtrl::Item& GridCtrl::Item::Editable(bool b)
{
	editable = b;
	//SyncCtrls(id);
	return *this;
}

GridCtrl::Item& GridCtrl::Item::NoEditable()
{
	return Editable(false);
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Editable(bool b)
{
	editable = b;
	parent->SyncCtrls(id);
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::NoEditable()
{
	return Editable(false);
}

END_UPP_NAMESPACE
