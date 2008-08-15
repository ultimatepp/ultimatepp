#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

#include "GridBase.h"

int ItemRect::sortCol;
int ItemRect::sortMode;

/*---------------------------------------------------*/

void Item::SetCtrl(Ctrl * newCtrl)
{
	ctrl = newCtrl;
	ctrl->Hide();
	ctrl->WantFocus();
	//ctrl->SetFrame(NullFrame());
	ctrl->SetFrame(BlackFrame());
}

void Item::DropCtrl()
{
	ctrl = NULL;
}

VectorMap<Id, int> *ItemRect::aliases = NULL;

/*---------------------------------------------------*/

ItemRect& ItemRect::Alias(Id _id)
{
	aliases->SetKey(id, _id);
	return *this;
}

ItemRect& ItemRect::Alias(const char * s)
{
	aliases->SetKey(id, s);
	return *this;
}

ItemRect& ItemRect::Name(String &s)
{
	(*items)[0][id].val = s;
	return *this;
}

ItemRect& ItemRect::Name(const char * s)
{
	(*items)[0][id].val = s;
	return *this;
}

ItemRect& ItemRect::Size(int n)
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

ItemRect& ItemRect::Width(int n)
{
	return Size(n);
}

ItemRect& ItemRect::Height(int n)
{
	return Size(n);
}

ItemRect& ItemRect::Min(int n)
{
	if(hidden)
		return *this;
	min = n;
	if(size < min)
		size = min;
	return *this;
}

ItemRect& ItemRect::Max(int n)
{
	if(hidden)
		return *this;
	max = n;
	if(size > max)
		size = max;
	return *this;
}

ItemRect& ItemRect::Fixed(int n)
{
	return Min(n).Max(n);
}

ItemRect& ItemRect::Edit(Ctrl &ctrl)
{
/*	Item &it = (*items)[0][id];
	it.SetCtrl(&ctrl);
*/	ctrl.Hide();
	ctrl.SetFrame(BlackFrame());
	(*edits)[id].ctrl = &ctrl;
	parent->AddChild(&ctrl);
	return *this;
}


void ItemRect::ChangeSortMode(bool idsort)
{
	if(++sortmode > 2) sortmode = (int) !idsort;
}

ItemRect& ItemRect::SetConvert(Convert &c)
{
	(*edits)[id].convert = &c;
	return *this;
}

ItemRect& ItemRect::NoConvertion()
{
	convertion = false;
	return *this;
}

ItemRect& ItemRect::Default(Value v)
{
	defval = v;
	return *this;
}

ItemRect& ItemRect::SetFormat(const char *fmt)
{
	//(*items)[0][id].convert = &c;
	return *this;
}

ItemRect& ItemRect::SetDisplay(GridDisplay &gd)
{
	(*items)[0][id].display = &gd;
	return *this;
}

void ItemRect::Clear()
{
	fnt = StdFont();
	bg = Null;
	fg = Null;
}

void ItemRect::Serialize(Stream &s)
{
	s % align;
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
  ctrl->WantFocus();
}

ItemRect& ItemRect::Option()
{
	return Ctrls(MakeOption).CtrlAlignHorzCenter(15);
}


END_UPP_NAMESPACE
