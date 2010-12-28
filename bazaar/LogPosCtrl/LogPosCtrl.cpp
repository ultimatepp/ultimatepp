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

Rect LogPosPopUp::CtrlRect(Ctrl::LogPos pos, Size sz)
{
	Rect r;
	sLay1(r.left, r.right, pos.x.GetAlign(), pos.x.GetA(), pos.x.GetB(), sz.cx);
	sLay1(r.top, r.bottom, pos.y.GetAlign(), pos.y.GetA(), pos.y.GetB(), sz.cy);
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

Ctrl::LogPos LogPosPopUp::MakeLogPos(int ax, int ay, const Rect& r, Size sz)
{
	return Ctrl::LogPos(MakeLogc(ax, r.left, r.right, sz.cx),
	                    MakeLogc(ay, r.top, r.bottom, sz.cy));
}

Ctrl::LogPos LogPosPopUp::MakeLogPos(Ctrl::LogPos p, const Rect& r, Size sz)
{
	return MakeLogPos(p.x.GetAlign(), p.y.GetAlign(), r, sz);
}

Ctrl::LogPos LogPosPopUp::ReAlign(const Ctrl& c, const Ctrl::LogPos& npos)
{
	if(!c.GetParent()) return npos;
	//convert ctrl's pos to its rect in parent
	//and reconvert to logpos using new align
	Size sz = c.GetParent()->GetSize();
	Ctrl::LogPos pos = c.GetPos();
	Rect r = CtrlRect(pos, sz);
	pos = MakeLogPos(npos, r, sz);
	return pos;
}

void LogPosPopUp::Set(const Ctrl::LogPos& p)
{
	pos = p;	
	xa <<= pos.x.GetA(); xb <<= pos.x.GetB();
	switch(pos.x.GetAlign())
	{
		case Ctrl::LEFT:   l <<= true;  r <<= false; break;
		case Ctrl::RIGHT:  l <<= false; r <<= true; break;
		case Ctrl::SIZE:   l <<= true;  r <<= true; break;
		default:
		case Ctrl::CENTER: l <<= false; r <<= false; break;
	}
	ya <<= pos.y.GetA(); yb <<= pos.y.GetB();
	switch(pos.y.GetAlign())
	{
		case Ctrl::TOP: u    <<= true;  d <<= false; break;
		case Ctrl::BOTTOM: u <<= false; d <<= true;  break;
		case Ctrl::SIZE: u   <<= true;  d <<= true;  break;
		default:
		case Ctrl::CENTER: u <<= false; d <<= false; break;
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
	q |= ((l)?(1):(0));
	q |= ((r)?(2):(0));
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
	q |= ((u)?(1):(0));
	q |= ((d)?(2):(0));
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
void LogPosPopUp::XAlignCB()
{
	int q = 0;
	q |= ((l)?(1):(0));
	q |= ((r)?(2):(0));
	switch(q)
	{
		case 1: pos.x.SetAlign(Ctrl::LEFT); break;
		case 2: pos.x.SetAlign(Ctrl::RIGHT); break;
		case 3: pos.x.SetAlign(Ctrl::SIZE); break;
		default:
		case 0: pos.x.SetAlign(Ctrl::CENTER); break;
	}
	WhenAlignChange();
	UpdateAction();
}
void LogPosPopUp::YAlignCB()
{
	int q = 0;
	q |= ((u)?(1):(0));
	q |= ((d)?(2):(0));
	switch(q)
	{
		case 1: pos.y.SetAlign(Ctrl::TOP); break;
		case 2: pos.y.SetAlign(Ctrl::BOTTOM); break;
		case 3: pos.y.SetAlign(Ctrl::SIZE); break;
		default:
		case 0: pos.y.SetAlign(Ctrl::CENTER); break;
	}
	WhenAlignChange();
	UpdateAction();
}

LogPosPopUp::LogPosPopUp()
{
	CtrlLayout(*this);
	SetFrame(BlackFrame());

	l.SetLabel("<");
	r.SetLabel(">");
	u.SetLabel("/\\");
	d.SetLabel("\\/");
	
	xa <<= THISBACK(XaCB);
	xb <<= THISBACK(XbCB);
	ya <<= THISBACK(YaCB);
	yb <<= THISBACK(YbCB);

	l <<= THISBACK(XAlignCB);
	r <<= THISBACK(XAlignCB);
	u <<= THISBACK(YAlignCB);
	d <<= THISBACK(YAlignCB);
	
	ok <<= THISBACK(Acceptor);
	cancel <<= THISBACK(Rejector);
}

NAMESPACE_UPP
template<>
String AsString(const Ctrl::LogPos& pos)
{
	String s;
	int xa = pos.x.GetA(), xb = pos.x.GetB();
	switch(pos.x.GetAlign())
	{
		case Ctrl::LEFT:   s << "Left(mr:" << xa << ",sz:" << xb << ")"; break;
		case Ctrl::RIGHT:  s << "Right(mr:" << xa << ",sz:" << xb << ")"; break;
		case Ctrl::SIZE:   s << "HSize(mr:" << xa << ",mr:" << xb << ")"; break;
		default:
		case Ctrl::CENTER: s << "HCent(of:" << xa << ",sz:" << xb << ")"; break;
	}
	s << ".";
	int ya = pos.y.GetA(), yb = pos.y.GetB();
	switch(pos.y.GetAlign())
	{
		case Ctrl::TOP:    s << "Top(mr:" << ya << ",sz:" << yb << ")"; break;
		case Ctrl::BOTTOM: s << "Bottom(mr:" << ya << ",sz:" << yb << ")"; break;
		case Ctrl::SIZE:   s << "VSize(mr:" << ya << ",mr:" << yb << ")"; break;
		default:
		case Ctrl::CENTER: s << "VCent(of:" << ya << ",sz:" << yb << ")"; break;
	}
	return s;
}
END_UPP_NAMESPACE

LogPosCtrl::LogPosCtrl() : push(false)
{
	IgnoreMouse(false);

	lc.WhenSelect = THISBACK(AcceptDrop);
	lc.WhenCancel = THISBACK(CloseDrop);
	lc.WhenAction = THISBACK(ActionDrop);
	
	lc.WhenSizeChange = THISBACK(OnSizeChange);
	lc.WhenAlignChange = THISBACK(OnAlignChange);
}

void LogPosCtrl::LeftDown(Point p, dword keyflags)
{
	if(!HasFocus()) SetFocus();
	Drop();	
}

void LogPosCtrl::Drop()
{
	if(push) return;
	push = true;
	savedpos = pos;
	lc.PopUp(this, pos);
}

void LogPosCtrl::CloseDrop()
{
	push = false;
	if(pos != savedpos) {
		pos = savedpos;
		OnSizeChange(); //restore
		UpdateAction();
	}
}

void LogPosCtrl::AcceptDrop()
{
	push = false;
	pos = lc.Get();
	UpdateAction();
}

void LogPosCtrl::ActionDrop()
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
	if(!posparent) return;
}

void LogPosCtrl::OnAlignChange()
{
	if(!posparent) return;
	lc.Set(LogPosPopUp::ReAlign(*posparent, lc.Get()));
	lc.Update(); //feedback to lc
}
