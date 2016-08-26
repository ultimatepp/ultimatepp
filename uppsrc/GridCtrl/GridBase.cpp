#include "GridCtrl.h"

namespace Upp {

int GridCtrl::ItemRect::sortCol;
int GridCtrl::ItemRect::sortMode;

/*---------------------------------------------------*/

void GridCtrl::Item::SetCtrl(Ctrl& c, bool owned)
{
	ctrl = &c;
	ctrl->Hide();
	ctrl->SetFrame(NullFrame());
	ctrl->WantFocus();
	ctrl_flag = IC_INIT | IC_MANUAL;
	if(owned)
		ctrl_flag |= IC_OWNED;
}

void GridCtrl::Item::ClearCtrl()
{
	ctrl = NULL;
}

void GridCtrl::Item::SetDisplay(GridDisplay& gd)
{
	display = &gd;
}

void GridCtrl::Item::NoDisplay()
{
	display = NULL;
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
	if(hidden)
		size = 0;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Name(const char * s)
{
	(*items)[0][id].val = s;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Size(int n, bool hv)
{
	hidden = n == 0;
	
	if(!hidden)
	{
		n = hv ? Ctrl::HorzLayoutZoom(n) : Ctrl::VertLayoutZoom(n);
		if(n < min) { n = min; ismin = true; }
		if(n > max) { n = max; ismax = true; }
	}

	size = nsize = n;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Width(int n)
{
	return Size(n, true);
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Height(int n)
{
	return Size(n, false);
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
	n = Ctrl::VertLayoutZoom(n);
	return Min(n).Max(n);
}

GridCtrl::ItemRect& GridCtrl::ItemRect::FixedAuto()
{
	int n = GetTextSize((String)(*items)[0][id].val, StdFont()).cx + 15;
	return Min(n).Max(n);
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Edit(Ctrl &ctrl, bool b)
{
	if(b)
	{
		ctrl.Hide();
		ctrl.SetFrame(NullFrame());
		(*edits)[id].ctrl = &ctrl;
		parent->holder.AddChild(&ctrl);
	}
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::EditInsert(bool b /* = true*/)
{
	edit_insert = b;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::EditUpdate(bool b /* = true*/)
{
	edit_update = b;
	return *this;
}

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
	ctrl.Create<Option>().ShowLabel(false);
	ctrl->SetData(0);
	ctrl->WantFocus();
}

static void MakeThreeStateOption(One<Ctrl>& ctrl)
{
	ctrl.Create<Option>().ThreeState().ShowLabel(false);
	ctrl->SetData(0);
	ctrl->WantFocus();
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Ctrls(Callback1<One<Ctrl>&> _factory)
{
	if(!(*edits)[id].factory)
		++parent->genr_ctrls;
	(*edits)[id].factory = _factory;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::NoCtrls()
{
	(*edits)[id].factory.Clear();
	--parent->genr_ctrls;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Option()
{
	return Ctrls(MakeOption).CtrlAlignHorzPos().CtrlAlignVertPos();
}

GridCtrl::ItemRect& GridCtrl::ItemRect::ThreeStateOption()
{
	return Ctrls(MakeThreeStateOption).CtrlAlignHorzPos().CtrlAlignVertPos();
}

GridCtrl::Item& GridCtrl::Item::Editable(bool b)
{
	editable = b;
	return *this;
}

GridCtrl::Item& GridCtrl::Item::NoEditable()
{
	return Editable(false);
}

GridCtrl::ItemRect& GridCtrl::ItemRect::Editable(bool b)
{
	editable = b;
	return *this;
}

GridCtrl::ItemRect& GridCtrl::ItemRect::NoEditable()
{
	return Editable(false);
}

Value GridCtrl::ItemRect::ExtractValue(int r, int c) const
{
	const Value& v = (*items)[r][c].val;
	return IsType<AttrText>(v) ? Value(ValueTo<AttrText>(v).text) : v;
}

}
