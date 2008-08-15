#include <CtrlLib/CtrlLib.h>
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

ItemRect& ItemRect::Width(int n)
{
	if(n < 0)
		return *this;
	size = nsize = n;
	if(size < min) min = nsize;
	if(size > max) max = nsize;
	return *this;
}

ItemRect& ItemRect::Height(int n)
{
	if(n < 0)
		return *this;
	size = nsize = n;
	if(size < min) min = nsize;
	if(size > max) max = nsize;
	return *this;
}

ItemRect& ItemRect::Min(int n)
{
	if(n < 0)
		return *this;
	min = n;
	if(size < min)
		size = min;
	return *this;
}

ItemRect& ItemRect::Max(int n)
{
	if(n < 0)
		return *this;
	max = n;
	if(size > max)
		size = max;
	return *this;
}

ItemRect& ItemRect::Fixed(bool set)
{
	fixedsize = set;
	return *this;
}

ItemRect& ItemRect::Edit(Ctrl &ctrl)
{
	Item &it = (*items)[0][id];
	it.SetCtrl(&ctrl);
	ctrl.Hide();
	ctrl.SetFrame(BlackFrame());
	parent->AddChild(&ctrl);
	return *this;
}


void ItemRect::ChangeSortMode(bool idsort)
{
	if(++sortmode > 2) sortmode = (int) !idsort;
}

ItemRect& ItemRect::SetConvert(Convert &c)
{
	(*items)[0][id].convert = &c;
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
