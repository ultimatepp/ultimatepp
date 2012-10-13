#include "LogPosCtrl.h"

static void sLay1(int& pos, int& r, int align, int a, int b, int sz)
{
	pos = a;
	int size = b;
	switch(align) {
	case Ctrl::CENTER:
		pos = (sz - b) / 2 + a;
		break;
	case Ctrl::RIGHT:
		pos = sz - (a + b);
		break;
	case Ctrl::SIZE:
		size = sz - (a + b);
		break;
	}
	r = pos + max(size, 0);
}

//LogPos, parent Size -> Rect

//generates a Rect from a LogPos info and its parent size
Rect LogPosPopUp::CtrlRect(Ctrl::LogPos pos, Size sz)
{
	Rect r;
	sLay1(r.left, r.right, pos.x.GetAlign(), pos.x.GetA(), pos.x.GetB(), sz.cx);
	sLay1(r.top, r.bottom, pos.y.GetAlign(), pos.y.GetA(), pos.y.GetB(), sz.cy);
	return r;
}

//same as above but zoom support
Rect LogPosPopUp::CtrlRectZ(Ctrl::LogPos pos, Size sz)
{
	Rect r = CtrlRect(pos, sz);
	r.left = HorzLayoutZoom(r.left);
	r.right = HorzLayoutZoom(r.right);
	r.top = VertLayoutZoom(r.top);
	r.bottom = VertLayoutZoom(r.bottom);
	return r;
}

Ctrl::Logc MakeLogc(int align, int a, int b, int sz)
{
	int isz = b - a;
	switch(align) {
	case Ctrl::LEFT:
		return Ctrl::PosLeft(a, isz);
	case Ctrl::RIGHT:
		return Ctrl::PosRight(sz - b, isz);
	case Ctrl::CENTER:
		return Ctrl::PosCenter(isz, a - (sz - isz) / 2);
	}
	return Ctrl::PosSize(a, sz - b);
}

//alignment info, Rect, parent Size -> LogPos

Ctrl::LogPos LogPosPopUp::MakeLogPos(int ax, int ay, const Rect& r, Size sz)
{
	return Ctrl::LogPos(MakeLogc(ax, r.left, r.right, sz.cx),
	                    MakeLogc(ay, r.top, r.bottom, sz.cy));
}

//same as above, but takes align infos from a LogPos info
Ctrl::LogPos LogPosPopUp::MakeLogPos(Ctrl::LogPos p, const Rect& r, Size sz)
{
	return MakeLogPos(p.x.GetAlign(), p.y.GetAlign(), r, sz);
}

//same as above, but generates source Rect from pos/sz
//then applying new alignment to it, taken from p
//this effectivly replaces the alignment info, keeping the rect visually in place
//which includes recalculation of a and b data
Ctrl::LogPos LogPosPopUp::MakeLogPos(Ctrl::LogPos p, const Ctrl::LogPos& pos, Size sz)
{
	Rect r = CtrlRect(pos, sz); //generate the source rect
	return MakeLogPos(p, r, sz);
}

//same as above but it does it for a specific Ctrl already
Ctrl::LogPos LogPosPopUp::MakeLogPos(Ctrl::LogPos p, const Ctrl& c)
{
	if(!c.GetParent()) return p;
	//reconvert to logpos using new align
	return MakeLogPos(p, c.GetPos(), c.GetParent()->GetSize());
}

void LogPosPopUp::Set(const Ctrl::LogPos& p)
{
	pos = p;	
	xa <<= pos.x.GetA(); xb <<= pos.x.GetB();
	
	switch(pos.x.GetAlign())
	{
		case Ctrl::LEFT:   l = true;  r = false; break;
		case Ctrl::RIGHT:  l = false; r = true; break;
		case Ctrl::SIZE:   l = true;  r = true; break;
		default:
		case Ctrl::CENTER: l = false; r = false; break;
	}
	ya <<= pos.y.GetA(); yb <<= pos.y.GetB();
	switch(pos.y.GetAlign())
	{
		case Ctrl::TOP: t    = true;  b = false; break;
		case Ctrl::BOTTOM: t = false; b = true;  break;
		case Ctrl::SIZE: t   = true;  b = true;  break;
		default:
		case Ctrl::CENTER: t = false; b = false; break;
	}
}

void LogPosPopUp::Updated()
{
	switch(pos.x.GetAlign())
	{
		case Ctrl::LEFT:   sxa.SetText("Marg L"); sxb.SetText("Size"); sxn.SetText("Left"); break;
		case Ctrl::RIGHT:  sxa.SetText("Marg R"); sxb.SetText("Size"); sxn.SetText("Right"); break;
		case Ctrl::SIZE:   sxa.SetText("Marg L"); sxb.SetText("Marg R"); sxn.SetText("HFill"); break;
		default:
		case Ctrl::CENTER: sxa.SetText("Offs");   sxb.SetText("Size"); sxn.SetText("HCenter"); break;
	}
	switch(pos.y.GetAlign())
	{
		case Ctrl::TOP:    sya.SetText("Marg U"); syb.SetText("Size"); syn.SetText("Top"); break;
		case Ctrl::BOTTOM: sya.SetText("Marg D"); syb.SetText("Size"); syn.SetText("Bottom"); break;
		case Ctrl::SIZE:   sya.SetText("Marg U"); syb.SetText("Marg D"); syn.SetText("VFill"); break;
		default:
		case Ctrl::CENTER: sya.SetText("Offs");   syb.SetText("Size"); syn.SetText("VCenter"); break;
	}
}

//obsolete
Ctrl::LogPos LogPosPopUp::Generate() const
{
	Ctrl::LogPos pos;
	int q;

	q = 0;
	q |= l ? 1 : 0;
	q |= r ? 2 : 0;
	switch(q)
	{
		case 1: pos.x.SetAlign(Ctrl::LEFT); break;
		case 2: pos.x.SetAlign(Ctrl::RIGHT); break;
		case 3: pos.x.SetAlign(Ctrl::SIZE); break;
		default:
		case 0: pos.x.SetAlign(Ctrl::CENTER); break;
	}
	pos.x.SetA(xa); pos.x.SetB(xb);

	q = 0;
	q |= t ? 1 : 0;
	q |= b ? 2 : 0;
	switch(q)
	{
		case 1: pos.y.SetAlign(Ctrl::TOP); break;
		case 2: pos.y.SetAlign(Ctrl::BOTTOM); break;
		case 3: pos.y.SetAlign(Ctrl::SIZE); break;
		default:
		case 0: pos.y.SetAlign(Ctrl::CENTER); break;
	}
	pos.y.SetA(ya); pos.y.SetB(yb);

	return pos; 
}

void LogPosPopUp::XaCB()
{
	pos.x.SetA(xa);
	WhenSizeChange();
	UpdateAction();
}
void LogPosPopUp::XbCB()
{
	pos.x.SetB(xb);
	WhenSizeChange();
	UpdateAction();
}
void LogPosPopUp::YaCB()
{
	pos.y.SetA(ya);
	WhenSizeChange();
	UpdateAction();
}
void LogPosPopUp::YbCB()
{
	pos.y.SetB(yb);
	WhenSizeChange();
	UpdateAction();
}
void LogPosPopUp::XAlign()
{
	int q = 0;
	q |= l ? 1 : 0;
	q |= r ? 2 : 0;
	switch(q)
	{
		case 1: pos.x.SetAlign(Ctrl::LEFT); break;
		case 2: pos.x.SetAlign(Ctrl::RIGHT); break;
		case 3: pos.x.SetAlign(Ctrl::SIZE); break;
		default:
		case 0: pos.x.SetAlign(Ctrl::CENTER); break;
	}
}
void LogPosPopUp::YAlign()
{
	int q = 0;
	q |= t ? 1 : 0;
	q |= b ? 2 : 0;
	switch(q)
	{
		case 1: pos.y.SetAlign(Ctrl::TOP); break;
		case 2: pos.y.SetAlign(Ctrl::BOTTOM); break;
		case 3: pos.y.SetAlign(Ctrl::SIZE); break;
		default:
		case 0: pos.y.SetAlign(Ctrl::CENTER); break;
	}
}

LogPosPopUp::LogPosPopUp()
{
	CtrlLayoutCancel(*this, "");
	cktype = CKOKCANCEL;
	SetFrame(BlackFrame());

	l.SetLabel("<");
	r.SetLabel(">");
	t.SetLabel("/\\");
	b.SetLabel("\\/");
	
	xa <<= THISBACK(XaCB);
	xb <<= THISBACK(XbCB);
	ya <<= THISBACK(YaCB);
	yb <<= THISBACK(YbCB);

	l <<= THISBACK(LRCB);
	r <<= THISBACK(LRCB);
	t <<= THISBACK(TBCB);
	b <<= THISBACK(TBCB);
	
	lt <<= THISBACK(LTCB);
	rt <<= THISBACK(RTCB);
	lb <<= THISBACK(LBCB);
	rb <<= THISBACK(RBCB);

	hc <<= THISBACK(HCCB);
	vc <<= THISBACK(VCCB);
	cc <<= THISBACK(CCCB);

	hs <<= THISBACK(HSCB);
	vs <<= THISBACK(VSCB);
	ss <<= THISBACK(SSCB);
}

LogPosCtrl::LogPosCtrl() : push(false)
{
	IgnoreMouse(false);

	lc.WhenAccept = THISBACK(OnAccept);
	lc.WhenReject = THISBACK(OnReject);
	lc.WhenAction = THISBACK(OnAction);
	
	lc.WhenSizeChange = THISBACK(OnSizeChange);
	lc.WhenAlignChange = THISBACK(OnAlignChange);
	
	bsz.SetNull();
}

void LogPosCtrl::LeftDown(Point p, dword keyflags)
{
	if(IsReadOnly() || !IsEnabled()) return;
	if(!HasFocus()) SetFocus();
	Drop();	
}

void LogPosCtrl::Drop()
{
	if(push) return;
	push = true;
	lc.Set(pos);
	lc.Backup();
	lc.PopUp(this);
}

void LogPosCtrl::OnReject()
{
	push = false;
	Ctrl::LogPos _pos = lc.Get();
	if(pos != _pos) {
		pos = _pos;
		OnSizeChange(); //restore
		UpdateAction();
	}
}

void LogPosCtrl::OnAccept()
{
	push = false;
//	OnAction();
	pos = lc.Get();
}

void LogPosCtrl::OnAction()
{
	pos = lc.Get();
	UpdateAction();
}

void LogPosCtrl::Updated()
{
	SetText(AsString(pos));
}

void LogPosCtrl::OnSizeChange()
{
}

void LogPosCtrl::OnAlignChange()
{
	if(bsz.IsNullInstance()) return;
	lc.Set(LogPosPopUp::MakeLogPos(lc.Get(), pos, bsz));
	lc.Update(); //feedback to lc
}
