#include "CtrlLib.h"

namespace Upp {

#define LLOG(x) // LOG(x)

void Sb(Button::Style& bs, const Image& img)
{
	bs = Button::StyleNormal();
	ChLookWith(bs.look, img, bs.monocolor);
}

CH_STYLE(ScrollBar, Style, StyleDefault)
{
	arrowsize = ScrollBarArrowSize();
	barsize = FrameButtonWidth();
	thumbmin = 16;
	overthumb = 0;
	through = false;
	CtrlsImageLook(vupper, CtrlsImg::I_SBVU);
	CtrlsImageLook(vthumb, CtrlsImg::I_SBVT, CtrlsImg::SBVI());
	CtrlsImageLook(vlower, CtrlsImg::I_SBVL);
	CtrlsImageLook(hupper, CtrlsImg::I_SBHU);
	CtrlsImageLook(hthumb, CtrlsImg::I_SBHT, CtrlsImg::SBHI());
	CtrlsImageLook(hlower, CtrlsImg::I_SBHL);
	Sb(up, CtrlsImg::UA());
	Sb(up2, CtrlsImg::UA());
	Sb(down, CtrlsImg::DA());
	Sb(down2, CtrlsImg::DA());
	Sb(left, CtrlsImg::LA());
	Sb(left2, CtrlsImg::LA());
	Sb(right, CtrlsImg::RA());
	Sb(right2, CtrlsImg::RA());
	isup2 = isdown2 = isleft2 = isright2 = false;
	thumbwidth = Null;
	bgcolor = SColorPaper();
}

ScrollBar::ScrollBar() {
	minthumb = DPI(16);
	pagepos = pagesize = totalsize = 0;
	linesize = 1;
	autohide = false;
	autodisable = true;
	jump = false;
	track = true;
	horz = false;
	disabled = false;
	thumbsize = 8;
	thumbpos = 0;
	push = light = -1;
	NoWantFocus();
	style = NULL;
	SetStyle(StyleDefault());
	BackPaint();
	is_active = false;
}

ScrollBar::~ScrollBar() {}

void ScrollBar::GetSectionInfo(SectionInfo& info)const
{
	info.horz = IsHorz();
	
	Size sz = GetSize();
	int  s0, s1, s2, s3, s5, // width/height of each section (s6==s0)
		 total, m=3;

	int sbo = style->overthumb;
	int ts = thumbsize;
	if(ts < style->thumbmin)
		ts = 0;

	s2 = thumbpos;
	s3 = ts + 2 * sbo;
	s1 = s5 = 0;

	if(info.IsHorz()) {
		info.wh = sz.cy;
		total = sz.cx;
		if( style->isleft2  ) ++m, ++s1;
		if( style->isright2 ) ++m, ++s5;
	}
	else {
		info.wh = sz.cx;
		total = sz.cy;
		if( style->isup2   ) ++m, ++s1;
		if( style->isdown2 ) ++m, ++s5;
	}
	s0 = total > m * style->arrowsize ? style->arrowsize : 0;
	if(s1) s1 = s0;
	if(s5) s5 = s0;

	info.starts[0] = 0;		info.starts[7] = total;
	info.starts[1] = s0;	info.starts[6] = total - s0;
	info.starts[2] = s0+s1; info.starts[5] = info.starts[6] - s5;
	info.starts[3] = info.starts[2] + s2;
	info.starts[4] = info.starts[3] + s3;

}

void ScrollBar::Paint(Draw& w) {
	SectionInfo info(this);
	
	w.DrawRect(GetSize(), style->bgcolor);

	Size sz = style->through ? GetSize() : info.Slider().GetSize();
	
	light = info.WhichSection(GetMouseViewPos());

	int p = HasCapture() ? push : -1;
	
	auto vs = [=](int i){
		return disabled ? CTRL_DISABLED :
			p == i ? CTRL_PRESSED :
			light == i ?  CTRL_HOT : CTRL_NORMAL;
	};
	const Value * vl1[]={style->up.look, style->up2.look, style->vlower,
		style->vthumb, style->vupper, style->down2.look, style->down.look};
	const Value * hl1[] = {style->left.look, style->left2.look, style->hlower,
		style->hthumb, style->hupper, style->right2.look, style->right.look};

	const Value** looks = IsHorz() ? hl1 : vl1;

	for(int i=0; i<7; ++i)
	{
		Rect r = info.GetPartRect(i);
		if(r.Width() != 0 && r.Height() != 0)
		{
			if(disabled && i>=2 && i<=4){
			// trying to construct from original painting logic
			// but never tested with disabled = true;
				if(i == 2){
					ChPaint(w, sz, looks[2][CTRL_DISABLED]);
				}
				continue;
			}
			if(i == 2 || i == 4) {
				w.Clip(r);
				r = style->through ? GetSize() : info.Slider();
			}
			if( i!=3 || thumbsize >= style->thumbmin)
				ChPaint(w, r, looks[i][vs(i)]);
			if(i == 2 || i == 4)
				w.End();
		}
	}
}

void ScrollBar::Bounds() {
	int maxsize = SectionInfo(this).GetSliderSize();
	if(thumbsize > maxsize)
		thumbsize = maxsize;
	if(thumbpos + thumbsize > maxsize)
		thumbpos = maxsize - thumbsize;
	if(thumbpos < 0)
		thumbpos = 0;
}

bool ScrollBar::SetThumb(int _thumbpos, int _thumbsize) {
	int ts = thumbsize;
	int tp = thumbpos;
	thumbsize = _thumbsize;
	thumbpos = _thumbpos;
	Bounds();
	if(thumbsize != ts || thumbpos != tp) {
		Refresh();
		return true;
	}
	return false;
}

void ScrollBar::Drag(Point p) {
	if(SetThumb(max(0, IsHorz() ? p.x - delta : p.y - delta), thumbsize) && track)
		Position();
}

void ScrollBar::LeftDown(Point p, dword) {
	SectionInfo info(this);

	push = info.WhichSection(GetMouseViewPos());

	LLOG("ScrollBar::LeftDown(" << p << ")");
	LLOG("MousePos = " << GetMousePos() << ", ScreenView = " << GetScreenView()
	<< ", rel. pos = " << (GetMousePos() - GetScreenView().TopLeft()));
	LLOG("GetWorkArea = " << GetWorkArea());
	LLOG("VisibleScreenView = " << GetVisibleScreenView());
	LLOG("PartRect(0) = " << GetPartRect(0));
	LLOG("PartRect(1) = " << GetPartRect(1));
	LLOG("PartRect(2) = " << GetPartRect(2));
	LLOG("ScrollBar::LeftDown: mousepart = " << (int)push << ", rect = " << GetPartRect(push)
		<< ", overthumb = " << style->overthumb << ", slider = " << Slider());
	LLOG("thumbpos = " << thumbpos << ", thumbsize = " << thumbsize);
	switch(push)
	{
	case 0:
	case 1:
		PrevLine();  break;
	case 2:
	case 4:
		if(jump) {
			delta = thumbsize / 2;
			Drag(p);
		}
		else
			if(push==2){
				PrevPage();
			}else
				NextPage();
		break;

	case 3:
		delta = info.GetHV(p.x, p.y) - thumbpos; break;
	case 5:
	case 6:
		NextLine();
	}
	SetCapture();
	Refresh();
	WhenLeftClick();
}

void ScrollBar::MouseMove(Point p, dword) {
	SectionInfo info(this);

	LLOG("Mouse in section " << info.WhichSection(p) );
	if(HasCapture() && push == 3)
		Drag(p);
	else
	if(light != info.WhichSection(p))
		Refresh();
}

void ScrollBar::MouseEnter(Point p, dword)
{
	Refresh();
}

void ScrollBar::MouseLeave()
{
	Refresh();
}

void ScrollBar::LeftUp(Point p, dword) {
	ReleaseCapture();
	if(!track)
		Position();
	Refresh();
	push = -1;
}

void ScrollBar::LeftRepeat(Point p, dword) {
	if(jump || push < 0 || push == 3) return;
	switch(push)
	{
	case 0:
	case 1:
		PrevLine();  break;
	case 2:
		PrevPage(); break;
	case 4:
		NextPage(); break;
	case 5:
	case 6:
		NextLine();
	}

	Refresh();
}

void ScrollBar::MouseWheel(Point p, int zdelta, dword keyflags)
{
	Wheel(zdelta);
}

void ScrollBar::CancelMode() {
	push = light = -1;
}

bool  ScrollBar::Set(int apagepos) {
	int op = pagepos;
	pagepos = apagepos;
	if(pagepos > totalsize - pagesize) pagepos = totalsize - pagesize;
	if(pagepos < 0) pagepos = 0;
	int slsize = SectionInfo(this).GetSliderSize();
	int mint = max((int)minthumb, style->thumbmin);
	if(totalsize <= 0)
		SetThumb(0, slsize);
	else {
		double thumbsize = slsize * pagesize / (double) totalsize;
		double rest = slsize * pagesize - thumbsize * totalsize;
		double ts, ps;
		if(thumbsize >= slsize || thumbsize < 0) {
			ts = slsize;
			ps = 0;
		}
		else
		if(thumbsize <= mint) {
			ps = ((slsize - mint) * (double)pagepos + rest) / (double) (totalsize - pagesize);
			ts = mint;
		}
		else {
			ps = (slsize * (double)pagepos + rest) / (double) totalsize;
			ts = thumbsize;
		}
		SetThumb(ffloor(ps), fceil(ts));
    }
	if(pagepos != op) {
		Refresh();
		WhenScroll();
		return true;
	}
	return false;
}

void ScrollBar::Set(int _pagepos, int _pagesize, int _totalsize) {
	pagesize = _pagesize;
	totalsize = _totalsize;
	is_active = totalsize > pagesize && pagesize > 0;
	if(autohide && is_active != IsShown()) {
		Show(is_active);
		WhenVisibility();
	}
	if(autodisable) {
		disabled=!is_active;
	}
	Set(_pagepos);
}

void ScrollBar::SetPage(int _pagesize) {
	Set(pagepos, _pagesize, totalsize);
}

void ScrollBar::SetTotal(int _totalsize) {
	Set(pagepos, pagesize, _totalsize);
}

void ScrollBar::Position() {
	int slsize = SectionInfo(this).GetSliderSize();
	int mint = max((int)minthumb, style->thumbmin);
	if(slsize < mint || totalsize <= pagesize)
		pagepos = 0;
	else
	if(thumbpos == slsize - thumbsize)
		pagepos = totalsize - pagesize;
	else
	if(thumbsize == mint)
		pagepos = iscale(thumbpos, (totalsize - pagesize), (slsize - mint));
	else
		pagepos = iscale(thumbpos, totalsize, slsize);
	Action();
	WhenScroll();
}

void ScrollBar::Uset(int a) {
	if(Set(a))
		Action();
}

void ScrollBar::PrevLine() {
	Uset(pagepos - linesize);
}

void ScrollBar::NextLine() {
	Uset(pagepos + linesize);
}

void ScrollBar::PrevPage() {
	Uset(pagepos - max(pagesize - linesize, 1));
}

void ScrollBar::NextPage() {
	Uset(pagepos + max(pagesize - linesize, 1));
}

void ScrollBar::Wheel(int zdelta, int lines) {
	Uset(pagepos - lines * linesize * zdelta / 120);
}

void ScrollBar::Wheel(int zdelta) {
	Wheel(zdelta, GUI_WheelScrollLines());
}

bool ScrollBar::VertKey(dword key, bool homeend) {
	if(!IsVisible() || !IsEnabled() || GetRect().IsEmpty())
		return false;
	switch(key) {
	case K_PAGEUP:
		PrevPage();
		break;
	case K_PAGEDOWN:
		NextPage();
		break;
	case K_UP:
		PrevLine();
		break;
	case K_DOWN:
		NextLine();
		break;
	case K_HOME:
		if(!homeend) break;
	case K_CTRL_HOME:
	case K_CTRL_PAGEUP:
		Begin();
		break;
	case K_END:
		if(!homeend) break;
	case K_CTRL_END:
	case K_CTRL_PAGEDOWN:
		End();
		break;
	default:
		return false;
	}
	return true;
}

void ScrollBar::Begin()
{
	Uset(0);
}

void ScrollBar::End()
{
	Uset(max(0, totalsize - pagesize));
}

bool ScrollBar::HorzKey(dword key) {
	if(!IsVisible() || !IsEnabled() || GetRect().IsEmpty())
		return false;
	switch(key) {
	case K_CTRL_LEFT:
		PrevPage();
		break;
	case K_CTRL_RIGHT:
		NextPage();
		break;
	case K_LEFT:
		PrevLine();
		break;
	case K_RIGHT:
		NextLine();
		break;
	case K_HOME:
		Begin();
		break;
	case K_END:
		End();
		break;
	default:
		return false;
	}
	return true;
}

void ScrollBar::Layout() {
	Set(pagepos);
	Refresh();
}

bool ScrollBar::ScrollInto(int pos, int _linesize) {
	int new_pos = pagepos;
	if(pos > new_pos + pagesize - _linesize)
		new_pos = pos - pagesize + _linesize;
	if(pos < new_pos)
		new_pos = pos;
	return Set(new_pos);
}

Size ScrollBar::GetStdSize() const {
	int a = HeaderCtrl::GetStdHeight();
	return Size(a, a);
}

void ScrollBar::FrameLayout(Rect& r)
{
	(IsHorz() ? LayoutFrameBottom : LayoutFrameRight)(r, this, ScrollBarSize());
}

void ScrollBar::FrameAddSize(Size& sz)
{
	(IsHorz() ? sz.cy : sz.cx) += ScrollBarSize();
}

Size ScrollBar::GetViewSize() const {
	if(IsChild() && InFrame()) {
		Size sz = GetParent()->GetSize();
		if(IsShown())
			(IsVert() ? sz.cx : sz.cy) += ScrollBarSize();
		return sz;
	}
	return Size(0, 0);
}

Size ScrollBar::GetReducedViewSize() const {
	if(IsChild() && InFrame()) {
		Size sz = GetParent()->GetSize();
		if(!IsShown())
			(IsVert() ? sz.cx : sz.cy) -= ScrollBarSize();
		return sz;
	}
	return Size(0, 0);
}

ScrollBar& ScrollBar::AutoHide(bool b) {
	autohide = b;
	if(b)
		SetTotal(totalsize);
	else
		Show();
	WhenVisibility();
	return *this;
}

ScrollBar& ScrollBar::AutoDisable(bool b) {
	autodisable = b;
	
	if(!b)
		disabled=false;
	return *this;
}

ScrollBar& ScrollBar::SetStyle(const Style& s)
{
	if(style != &s) {
		style = &s;
		RefreshLayout();
		Refresh();
	}
	return *this;
}


int ScrollBar::SectionInfo::WhichSection(int p)const
{
	int q=-1;
	for(int i = 0; i < 8; ++i)
	{
		if(p < starts[i])
			break;
		++q;
	}
	if(q == 7)
		q = -1;
	return q;
}

Rect ScrollBar::SectionInfo::GetAll()const
{
	int l,t,r,b;
	if(horz){
		l = Start(0); t = 0; r = Start(7); b = wh;
	}else{
		l = 0; t = Start(0); r = wh; b = Start(7);
	}
	return Rect(l, t, r, b);
}

Rect ScrollBar::SectionInfo::Slider()const
{
	int l,t,r,b;
	if(horz){
		l = Start(2); t = 0; r = Start(5); b = wh;
	}else{
		l = 0; t=Start(2); r = wh; b = Start(5);
	}
	return Rect(l, t, r, b);
}

Rect ScrollBar::SectionInfo::GetPartRect(int i)const{
	int l,t,r,b;
	if(horz){
		l = Start(i); t = 0; r = End(i); b = wh;
	}else{
		l = 0; t=Start(i); r = wh; b = End(i);
	}
	return Rect(l, t, r, b);
}

Image SizeGrip::CursorImage(Point p, dword)
{
	if(GuiPlatformHasSizeGrip()) {
		TopWindow *q = dynamic_cast<TopWindow *>(GetTopCtrl());
		if(q && !q->IsMaximized() && q->IsSizeable())
			return Image::SizeBottomRight();
	}
	return Image::Arrow();
}

CH_IMAGE(SizeGripImg, CtrlsImg::SizeGrip());

void SizeGrip::Paint(Draw& w)
{
	Size sz = GetSize();
	if(!IsTransparent())
	    w.DrawRect(sz, SColorFace);
	if(GuiPlatformHasSizeGrip()) {
		TopWindow *q = dynamic_cast<TopWindow *>(GetTopCtrl());
		if(q && !q->IsMaximized() && q->IsSizeable()) {
			Size isz = CtrlsImg::SizeGrip().GetSize();
			w.DrawImage(sz.cx - isz.cx, sz.cy - isz.cy, CtrlsImg::SizeGrip());
		}
    }
}

SizeGrip::SizeGrip()
{
	Transparent();
	RightPos(0, DPI(12)).BottomPos(0, DPI(12));
	NoWantFocus();
	Width(DPI(14));
}

SizeGrip::~SizeGrip() {}

void SizeGrip::LeftDown(Point p, dword flags)
{
	TopWindow *q = dynamic_cast<TopWindow *>(GetTopCtrl());
	if(q && !q->IsMaximized() && q->IsSizeable())
		GuiPlatformGripResize(q);
}

void ScrollBars::Scroll() {
	WhenScroll();
}

bool ScrollBars::Key(dword key) {
	return x.HorzKey(key) || y.VertKey(key);
}

void ScrollBars::Set(Point pos, Size page, Size total) {
	x.Set(pos.x, page.cx, total.cx);
	y.Set(pos.y, page.cy, total.cy);
}

bool ScrollBars::Set(int _x, int _y) {
	bool b = x.Set(_x) | y.Set(_y);
	return b;
}

bool ScrollBars::Set(Point pos) {
	return Set(pos.x, pos.y);
}

void ScrollBars::SetPage(int cx, int cy) {
	x.SetPage(cx);
	y.SetPage(cy);
}

void ScrollBars::SetPage(Size page) {
	SetPage(page.cx, page.cy);
}

void ScrollBars::SetTotal(int cx, int cy) {
	x.SetTotal(cx);
	y.SetTotal(cy);
}

void ScrollBars::SetTotal(Size total) {
	SetTotal(total.cx, total.cy);
}

bool ScrollBars::ScrollInto(Point pos, Size linesize) {
	return x.ScrollInto(pos.x, linesize.cx) | y.ScrollInto(pos.y, linesize.cy);
}

bool ScrollBars::ScrollInto(Point pos) {
	return x.ScrollInto(pos.x) | y.ScrollInto(pos.y);
}

ScrollBars& ScrollBars::SetLine(int linex, int liney) {
	x.SetLine(linex);
	y.SetLine(liney);
	return *this;
}

ScrollBars& ScrollBars::Track(bool b) {
	x.Track(b);
	y.Track(b);
	return *this;
}

ScrollBars& ScrollBars::Jump(bool b) {
	x.Jump(b);
	y.Jump(b);
	return *this;
}

ScrollBars& ScrollBars::AutoHide(bool b) {
	x.AutoHide(b);
	y.AutoHide(b);
	return *this;
}

ScrollBars& ScrollBars::AutoDisable(bool b) {
	x.AutoDisable(b);
	y.AutoDisable(b);
	return *this;
}

ScrollBars& ScrollBars::Box(Ctrl& _box) {
	box->Remove();
	box = &_box;
	if(x.GetParent() && x.GetParent() != box->GetParent())
		x.GetParent()->Add(*box);
	return *this;
}

void ScrollBars::FrameAdd(Ctrl& p) {
	p.Add(x);
	p.Add(y);
	if(box->GetParent() != &p)
		p.Add(*box);
}

void ScrollBars::FrameRemove() {
	x.Remove();
	y.Remove();
	box->Remove();
}

void ScrollBars::FramePaint(Draw& w, const Rect& r) {
	if(x.IsShown() && y.IsShown() && !box) {
		int h = ScrollBarSize();
		w.DrawRect(r.right - h, r.bottom - h, h, h, SColorFace);
	}
}

void ScrollBars::FrameLayout(Rect& r) {
	int h = ScrollBarSize();
	int by = 0;
	int bx = x.IsShown() && y.IsShown() ? h : 0;
	if(box_type == 1)
		by = bx;
	if(box_type == 2)
		by = h;
	int dx = x.IsShown() * h;
	int dy = y.IsShown() * h;
	y.SetFrameRect(r.right - dy, r.top, dy, r.Height() - by);
	x.SetFrameRect(r.left, r.bottom - dx, r.Width() - bx, dx);
	if(box)
		box->SetFrameRect(r.right - by, r.bottom - by, by, by);
	r.right -= dy;
	r.bottom -= dx;
}

void ScrollBars::FrameAddSize(Size& sz) {
	int h = ScrollBarSize();
	sz.cy += x.IsShown() * h;
	sz.cx += y.IsShown() * h;
}

Size ScrollBars::GetViewSize() const {
	return Size(y.GetViewSize().cx, x.GetViewSize().cy);
}

Size ScrollBars::GetReducedViewSize() const {
	return Size(y.GetReducedViewSize().cx, x.GetReducedViewSize().cy);
}

ScrollBars& ScrollBars::NormalBox()
{
	box_type = 1;
	y.RefreshParentLayout();
	return *this;
}

ScrollBars& ScrollBars::NoBox()
{
	box_type = 0;
	y.RefreshParentLayout();
	return *this;
}

ScrollBars& ScrollBars::FixedBox()
{
	box_type = 2;
	y.RefreshParentLayout();
	return *this;
}

ScrollBars& ScrollBars::WithSizeGrip()
{
	box_bg.Color(SColorFace());
	the_box.Add(box_bg.SizePos());
	the_box.Add(grip);
	return *this;
}

ScrollBars::ScrollBars() {
	box = &the_box;
//	the_box.NoTransparent();
	x.WhenScroll = y.WhenScroll = callback(this, &ScrollBars::Scroll);
	x.WhenLeftClick = y.WhenLeftClick = Proxy(WhenLeftClick);
	x.AutoHide();
	y.AutoHide();
	box_type = 1;
}

ScrollBars::~ScrollBars() {}

void Scroller::Scroll(Ctrl& p, const Rect& rc, Point newpos, Size cellsize)
{
	if(!IsNull(psb) && !p.IsTransparent()) {
		Point d = psb - newpos;
		p.ScrollView(rc, d.x * cellsize.cx, d.y * cellsize.cy);
	}
	else
		p.Refresh();
	psb = newpos;
}

void Scroller::Scroll(Ctrl& p, const Rect& rc, int newposy, int linesize)
{
	Scroll(p, rc, Point(0, newposy), Size(0, linesize));
}

void Scroller::Scroll(Ctrl& p, Point newpos)
{
	Scroll(p, p.GetSize(), newpos);
}

void Scroller::Scroll(Ctrl& p, int newposy)
{
	Scroll(p, p.GetSize(), newposy);
}

}
