#include "CtrlLib.h"

namespace Upp {

#define LLOG(x) // LOG(x)

void Pusher::RefreshFocus() {
	Refresh();
}

void Pusher::RefreshPush() {
	Refresh();
}

void Pusher::PerformAction() {
	Action();
}

void Pusher::GotFocus() {
	RefreshFocus();
}

void Pusher::LostFocus() {
	if(keypush) EndPush();
	RefreshFocus();
}

void Pusher::LeftDown(Point, dword) {
	if(Ctrl::ClickFocus() || clickfocus) SetWantFocus();
	if(IsReadOnly()) return;
	push = true;
	RefreshPush();
	WhenPush();
	SetCapture();
}

void Pusher::MouseMove(Point, dword style) {
	LLOG("Mouse move !");
	if(IsReadOnly() || !HasCapture()) return;
	bool b = GetScreenView().Contains(GetMousePos());
	if(b != push) {
		push = b;
		RefreshPush();
	}
}

void Pusher::MouseLeave() {
	EndPush();
}

void Pusher::LeftRepeat(Point, dword) {
	if(IsReadOnly()) return;
	WhenRepeat();
}

void Pusher::LeftUp(Point, dword) {
	FinishPush();
}

dword Pusher::GetAccessKeys() const
{
	return AccessKeyBit(accesskey);
}

void  Pusher::AssignAccessKeys(dword used)
{
	if(!accesskey) {
		accesskey = ChooseAccessKey(label, used);
		if(accesskey) Refresh();
		used |= AccessKeyBit(accesskey);
	}
	Ctrl::AssignAccessKeys(used);
}

void  Pusher::State(int)
{
	if(keypush && GetTopCtrl() && !GetTopCtrl()->HasFocusDeep())
		EndPush();
}

void Pusher::KeyPush() {
	if(IsReadOnly()) return;
	if(keypush) {
		WhenRepeat();
		return;
	}
	push = keypush = true;
	RefreshPush();
	WhenPush();
}

bool Pusher::FinishPush()
{
	bool b = IsPush();
	EndPush();
	if(!b)
		return false;
	if(IsReadOnly())
		return false;
	ReleaseCapture(); // this is to mitigate X11 mouse capture debugger problems
	PerformAction();
	return true;
}

bool Pusher::Key(dword key, int) {
	if(IsReadOnly()) return false;
	if(key == K_SPACE) {
		KeyPush();
		return true;
	}
	if(key == (K_SPACE|K_KEYUP))
		return FinishPush();
	return false;
}

bool Pusher::HotKey(dword key) {
	if(CompareAccessKey(accesskey, key)) {
		PseudoPush();
		return true;
	}
	return false;
}

void Pusher::PseudoPush() {
	if(IsReadOnly() || !IsEnabled()) return;
	KeyPush();
	Sync();
	Sleep(50);
	FinishPush();
}

void Pusher::EndPush() {
	keypush = push = false;
	RefreshPush();
}

Pusher&  Pusher::SetFont(Font fnt) {
	font = fnt;
	Refresh();
	return *this;
}

Pusher& Pusher::ClickFocus(bool cf)
{
	clickfocus = cf;
	return *this;
}

String Pusher::GetDesc() const
{
	return label;
}

Pusher& Pusher::SetLabel(const char *atext) {
	accesskey = ExtractAccessKey(atext, label);
	Refresh();
	return *this;
}

void Pusher::CancelMode() {
	EndPush();
}

int Pusher::GetVisualState() const
{
	return !IsShowEnabled() ? CTRL_DISABLED :
	       IsPush() ? CTRL_PRESSED :
	       HasMouse() ? CTRL_HOT :
	       CTRL_NORMAL;
}

Pusher::Pusher() {
	keypush = push = clickfocus = false;
	accesskey = 0;
	font = StdFont();
	NoInitFocus();
}

Pusher::~Pusher() {}

// ----------------

CH_STYLE(Button, Style, StyleNormal)
{
	CtrlsImageLook(look, CtrlsImg::I_B);
	monocolor[0] = monocolor[1] = monocolor[2] = monocolor[3] =
		Blend(Blend(SColorHighlight, SColorShadow), SColorLabel, 80);
	textcolor[0] = textcolor[1] = textcolor[2] = SColorLabel();
	textcolor[3] = SColorDisabled();
	font = StdFont();
	pressoffset = Point(0, 0);
	focusmargin = 3;
	overpaint = 0;
	transparent = true;
	focus_use_ok = true;
}

CH_STYLE(Button, Style, StyleOk)
{
	Assign(Button::StyleNormal());
	CtrlsImageLook(look, CtrlsImg::I_OkB);
}

CH_STYLE(Button, Style, StyleEdge)
{
	Assign(Button::StyleNormal());
	CtrlsImageLook(look, CtrlsImg::I_EB);
}

CH_STYLE(Button, Style, StyleLeftEdge)
{
	Assign(Button::StyleEdge());
}

CH_STYLE(Button, Style, StyleScroll)
{
	Assign(Button::StyleNormal());
	CtrlsImageLook(look, CtrlsImg::I_SB);
}

CH_STYLE(Button, Style, StyleNaked)
{
	Assign(Button::StyleNormal());
	look[0] = look[1] = look[2] = look[3] = Null;
}

Color ButtonMonoColor(int i)
{
	return Button::StyleNormal().monocolor[i];
}

Button& Button::Ok()
{
	if(IsNull(img))
		img = St()->ok;
	type = OK;
	Refresh();
	return *this;
}

Button& Button::Cancel()
{
	if(IsNull(img))
		img = St()->cancel;
	type = CANCEL;
	Refresh();
	return *this;
}

Button& Button::Exit()
{
	if(IsNull(img))
		img = St()->exit;
	type = EXIT;
	Refresh();
	return *this;
}

const Button::Style *GetButtonStyle(const Ctrl *q)
{
	const Button::Style *st = &Button::StyleNormal();
	if(q->InFrame()) {
		st = &Button::StyleEdge();
		if(q->GetParent()) {
			Rect r = q->GetRect();
			Size sz = q->GetParent()->GetRect().GetSize();
			if(r.right < sz.cx / 2)
				st = &Button::StyleLeftEdge();
		}
	}
	return st;
}

int Button::OverPaint() const
{
	return St()->overpaint;
}

Button& Button::SetStyle(const Button::Style& s)
{
	if(style != &s) {
		style = &s;
		RefreshLayout();
		Refresh();
	}
	return *this;
}

Button& Button::AutoStyle()
{
	style = NULL;
	RefreshLayout();
	Refresh();
	return *this;
}

void Button::Layout()
{
	Transparent(St()->transparent);
}

void Button::RefreshOK(Ctrl *p)
{
	for(Ctrl *q = p->GetFirstChild(); q; q = q->GetNext()) {
		Button *b = dynamic_cast<Button *>(q);
		if(b && b->type == OK)
			b->Refresh();
		RefreshOK(q);
	}
}

void Button::GotFocus()
{
	RefreshOK(GetTopCtrl());
	Pusher::GotFocus();
}

void Button::LostFocus()
{
	RefreshOK(GetTopCtrl());
	Pusher::LostFocus();
}

const Button::Style *Button::St() const
{
	const Style *st;
	if(style)
		st = style;
	else {
		st = GetButtonStyle(this);
		if(type == OK) {
			Button *b = dynamic_cast<Button *>(GetFocusCtrl());
			if(!b || b == this || b->GetTopCtrl() != GetTopCtrl())
				st = &StyleOk();
		}
		if(HasFocus() && st->focus_use_ok)
			st = &StyleOk();
	}
	return st;
}

void Button::PaintButton(Draw& w, const Rect& r, const Button::Style& st, int visualstate, bool focus,
                         const String& label, Font font, const Image& img,
                         bool monoimg, int accesskey, bool visibleaccesskeys, bool disabled)
{
	DrawLabel dl;
	dl.text = label;
	dl.font = Nvl(font, st.font);
	dl.limg = img;
	dl.disabled = disabled;
	dl.lspc = !label.IsEmpty() && !img.IsEmpty() ? 4 : 0;
	if(*label == '\1')
		dl.align = ALIGN_LEFT;
	if(visibleaccesskeys)
		dl.accesskey = accesskey;
	if(monoimg)
		dl.lcolor = SColorText;
	ChPaint(w, r, st.look[visualstate]);
	dl.ink = st.textcolor[visualstate];
	if(monoimg)
		dl.lcolor = st.monocolor[visualstate];
	bool push = visualstate == CTRL_PRESSED;
	dl.Paint(w, r.left + 3 + push * st.pressoffset.x, r.top + 3 + push * st.pressoffset.y,
	         r.GetWidth() - 6, r.GetHeight() - 6);
	if(focus)
		DrawFocus(w, r.Deflated(st.focusmargin));
}

void Button::Paint(Draw& w)
{
	PaintButton(w, GetSize(), *St(), GetVisualState(), HasFocus(),
	            label, font, img,
	            monoimg, accesskey, VisibleAccessKeys(), !IsShowEnabled());
}

void  Button::MouseEnter(Point, dword)
{
	Refresh();
}

void  Button::MouseLeave()
{
	Refresh();
	Pusher::MouseLeave();
}

bool Button::Key(dword key, int w) {
	if(Pusher::Key(key, w)) return true;
	if(key == K_ENTER) {
		KeyPush();
		return true;
	}
	if(key == (K_ENTER|K_KEYUP))
		return FinishPush();
	return false;
}

bool Button::HotKey(dword key) {
	if(Pusher::HotKey(key)) return true;
	if(CompareAccessKey(accesskey, key)) {
		PseudoPush();
		return true;
	}
	bool up = key & K_KEYUP;
	key = key & ~K_KEYUP;
	if(key == K_ENTER && (type == OK || type == EXIT) ||
	   key == K_ESCAPE && (type == CANCEL || type == EXIT)) {
		if(up) {
			if(IsKeyPush())
				return FinishPush();
		}
		else
			KeyPush();
		return true;
	}
	return false;
}

dword Button::GetAccessKeys() const
{
	if(type == OK || type == EXIT)
		return 1;
	return Pusher::GetAccessKeys();
}

void  Button::AssignAccessKeys(dword used)
{
	if(type == OK || type == EXIT || type == CANCEL)
		return;
	return Pusher::AssignAccessKeys(used);
}

Button& Button::SetImage(const Image& _img)
{
	img = _img;
	monoimg = false;
	Refresh();
	return *this;
}

Button& Button::SetMonoImage(const Image& _img)
{
	img = _img;
	monoimg = true;
	Refresh();
	return *this;
}

Button::Button() {
	style = NULL;
	type = NORMAL;
	monoimg = false;
	font = Null;
}

Button::~Button() {}

CH_STYLE(SpinButtons, Style, StyleDefault)
{
	inc = dec = Button::StyleEdge();
	ChLookWith(inc.look, CtrlsImg::SpU(), inc.monocolor);
	ChLookWith(dec.look, CtrlsImg::SpD(), dec.monocolor);
	width = Zx(12);
	over = 0;
	onsides = false;
}

CH_STYLE(SpinButtons, Style, StyleOnSides)
{
	Assign(SpinButtons::StyleDefault());
	onsides = true;
	inc = dec = Button::StyleEdge();
	ChLookWith(inc.look, CtrlImg::plus(), inc.monocolor);
	ChLookWith(dec.look, CtrlImg::minus(), dec.monocolor);
}

void SpinButtons::FrameLayout(Rect& r)
{
	if(!visible) {
		inc.Hide();
		dec.Hide();
		return;
	}
	inc.Show();
	dec.Show();
	Size sz = r.Size();
	int h = r.Height();
	int h7 = min(sz.cx / 2, style->width);
	int h7o = h7 - style->over;

	if(style->onsides) {
		dec.SetFrameRect(r.left - style->over, r.top, h7, h);
		inc.SetFrameRect(r.right - h7o, r.top, h7, h);
		r.left += h7o;
		r.right -= h7o;
	}
	else {
		int h2 = h / 2;
		inc.SetFrameRect(r.right - h7o, r.top, h7, h2);
		dec.SetFrameRect(r.right - h7o, r.top + h2, h7, h - h2);
		r.right -= h7o;
	}
}

void SpinButtons::FrameAddSize(Size& sz)
{
	if(!visible)
		return;
	sz.cx += (1 + style->onsides) * (min(sz.cx / 2, style->width) - style->over);
}

void SpinButtons::FrameAdd(Ctrl& ctrl)
{
	ctrl.Add(inc);
	ctrl.Add(dec);
}

void SpinButtons::FrameRemove() {
	inc.Remove();
	dec.Remove();
}

void SpinButtons::Show(bool s)
{
	visible = s;
	inc.RefreshParentLayout();
}

SpinButtons& SpinButtons::SetStyle(const Style& s)
{
	if(style != &s) {
		style = &s;
		inc.SetStyle(style->inc);
		dec.SetStyle(style->dec);
		inc.RefreshParentLayout();
	}
	return *this;
}

SpinButtons::SpinButtons() {
	visible = true;
	inc.NoWantFocus();
	dec.NoWantFocus();
	style = NULL;
	SetStyle(StyleDefault());
}

SpinButtons::~SpinButtons() {}

// -----------------

void  Option::MouseEnter(Point, dword)
{
	RefreshPush();
}

void  Option::MouseLeave()
{
	RefreshPush();
	Pusher::MouseLeave();
}

void Option::RefreshFocus() {
	Refresh();
}

void  Option::State(int s)
{
	AutoSync();
	Pusher::State(s);
}

void Option::RefreshPush() {
	if(box)
		Refresh(0, 0, GetSize().cx, GetSmartTextSize(label, font).cy);
	else
	if(showlabel)
		Refresh(0, 0, CtrlsImg::O0().GetSize().cx, GetSize().cy);
	else
		Pusher::RefreshPush();
}

void Option::EnableBox(bool b)
{
	Ctrl *p = GetParent();
	if(!p)
		return;
	Rect r = GetScreenRect();
	for(Ctrl *q = p->GetFirstChild(); q; q = q->GetNext())
		if(q != this && r.Intersects(q->GetScreenRect()))
			q->Enable(b);
}

void Option::AutoSync()
{
	if(!autobox)
		return;
	Ctrl *p = GetParent();
	if(!p)
		return;
	Rect r = GetScreenRect();
	for(Ctrl *q = p->GetFirstChild(); q; q = q->GetNext())
		if(q != this && r.Intersects(q->GetScreenRect()))
			q->Enable(option);
}

Size Option::GetMinSize() const {
	Size isz = CtrlsImg::O0().GetSize();
	return AddFrameSize(isz.cx + (GetSmartTextSize(label).cx + 4) * showlabel,
		                max(isz.cy, StdFont().Info().GetHeight()) + 2);
}

void Option::Paint(Draw& w) {
	Size sz = GetSize();
	
	if(!IsTransparent())
		w.DrawRect(0, 0, sz.cx, sz.cy, SColorFace);
	
	Size isz = CtrlsImg::O0().GetSize();
	Size tsz = GetSmartTextSize(label, font);
	int ix = 0;
	int d = tsz.cy >> 1;
	int ty = (sz.cy - tsz.cy) / 2;
	int iy = (tsz.cy - isz.cy) / 2 + ty;

	if(box) {
		ix = d + DPI(4);
		if(tsz.cy > isz.cy) {
			ty = 0;
			iy = (tsz.cy - isz.cy) / 2;
		}
		else {
			iy = 0;
			ty = (isz.cy - tsz.cy) / 2;
		}
	}
	else
	if(!showlabel) {
		ix = (sz.cx - isz.cx) / 2;
		iy = (sz.cy - isz.cy) / 2;
	}
	
	int q = GetVisualState();
	int g = (!notnull || threestate) && IsNull(option) ? CtrlsImg::I_O2
	                                                   : option == 1 ? CtrlsImg::I_O1
	                                                                 : CtrlsImg::I_O0;
	if(switchimage)
		g = option ? CtrlsImg::I_S1 : CtrlsImg::I_S0;
	
	w.DrawImage(ix, iy, CtrlsImg::Get(g + q));
	
	if(showlabel) {
		bool ds = !IsShowEnabled();
		DrawSmartText(w, ix + isz.cx + DPI(2), ty, tsz.cx, label, font,
		              ds || IsReadOnly() ? SColorDisabled : Nvl(color, GetLabelTextColor(this)),
		              VisibleAccessKeys() ? accesskey : 0);
		if(HasFocus())
			DrawFocus(w, RectC(ix + isz.cx + DPI(2), ty - DPI(1), tsz.cx + DPI(3), tsz.cy + DPI(2)) & sz);
	}
	
	if(box) {
		w.Begin();
		w.ExcludeClip(ix - DPI(3), 0, isz.cx + DPI(8) + tsz.cx, tsz.cy);
		PaintLabelBox(w, sz, Null, d);
		w.End();
	}
}

void   Option::SetData(const Value& data) {
	Set(IsString(data) ? String(data) == "1" ? 1
	                                         : IsNull(data) ? (notnull ? 0 : int(Null)) : 0
	                   : notnull ? Nvl((int)data, 0) : (int)data);
}

Value  Option::GetData() const {
	return (int) option;
}

void  Option::PerformAction() {
	if(option == 0)
		option = threestate ? Null : 1;
	else
	if(option == 1)
		option = 0;
	else
		option = 1;
	UpdateAction();
	RefreshPush();
	AutoSync();
}

Option& Option::Set(int b)
{
	b = IsNull(b) ? Null : (int)(bool)b;
	if(b != option) {
		option = b;
		Update();
		RefreshPush();
		AutoSync();
	}
	return *this;
}

Option::Option() {
	option = 0;
	notnull = true;
	switchimage = threestate = false;
	blackedge = false;
	showlabel = true;
	autobox = box = false;
	Transparent();
	font = StdFont();
	color = Null;
}

Option::~Option() {}

// --------

ButtonOption::ButtonOption()
{
	option = push = false;
	style = &StyleDefault();
	Transparent();
}

void ButtonOption::Serialize(Stream& s) {
	int version = 0;
	s / version;
	bool b = Get();
	s % b;
	Set(b);
	if(s.IsLoading())
		Refresh();
}

void  ButtonOption::Paint(Draw& w) {
	Size sz = GetSize();
	bool ds = !IsShowEnabled();
	DrawLabel dl;
	dl.text = label;
	dl.font = StdFont();
	dl.limg = DisabledImage((option && !IsNull(image1)) ? image1 : image, !IsEnabled());
	dl.disabled = ds;
	dl.lspc = !label.IsEmpty() && !image.IsEmpty() ? 4 : 0;
	if(*label == '\1')
		dl.align = ALIGN_LEFT;
	if(VisibleAccessKeys() && IsEnabled())
		dl.accesskey = accesskey;
	int i = !IsShowEnabled() ? CTRL_DISABLED :
	         push ? CTRL_PRESSED :
	         HasMouse() || HasFocus() ? CTRL_HOT :
	         CTRL_NORMAL;
	if(option) i = CTRL_PRESSED;
	ChPaint(w, sz, style->look[i]);
	dl.ink = style->textcolor[i];
	dl.Paint(w, 3, 3, sz.cx - 6, sz.cy - 6, true);
	if(HasFocus() && style->drawfocus)
		DrawFocus(w, Rect(sz).Deflated(3));
}

CH_STYLE(ButtonOption, Style, StyleDefault)
{
	const Button::Style& bs = Button::StyleNormal();
	for (int i = 0; i < 4; i++)
	{
		look[i] = bs.look[i];
		textcolor[i] = bs.textcolor[i];
	}
	drawfocus = false;
}

CH_STYLE(ButtonOption, Style, StyleFlat)
{
	const ToolButton::Style& tbs = ToolButton::StyleDefault();
	for (int i = 0; i < 4; i++)
	{
		look[i] = tbs.look[i];
		textcolor[i] = tbs.textcolor[i];
	}
	drawfocus = false;
}
	
void  ButtonOption::LeftDown(Point, dword) {
	if(IsReadOnly())
		return;
	push = true;
	Refresh();
}

void  ButtonOption::LeftUp(Point, dword) {
	if(IsReadOnly())
		return;
	option = !option;
	push = false;
	UpdateActionRefresh();
}

void  ButtonOption::MouseMove(Point, dword flags) {
	if(IsReadOnly())
		return;
	bool p = !!(flags & K_MOUSELEFT);
	if(push != p) {
		push = p;
		Refresh();
	}
}

void  ButtonOption::MouseEnter(Point, dword) {
	Refresh();
}

void  ButtonOption::MouseLeave() {
	if(IsReadOnly())
		return;
	if(push)
		push = false;
	Refresh();
}

void  ButtonOption::SetData(const Value& v)
{
	Set(IsNull(v) ? 0 : IsString(v) ? String(v) == "1" : (int)v);
}

Value ButtonOption::GetData() const
{
	return (int)Get();
}

dword ButtonOption::GetAccessKeys() const
{
	return AccessKeyBit(accesskey);
}

void  ButtonOption::AssignAccessKeys(dword used)
{
	if(!accesskey) {
		accesskey = ChooseAccessKey(label, used);
		if(accesskey) Refresh();
		used |= AccessKeyBit(accesskey);
	}
	Ctrl::AssignAccessKeys(used);
}

ButtonOption& ButtonOption::SetLabel(const String& text) {
	accesskey = ExtractAccessKey(text, label);
	Refresh();
	return *this;
}

// --------------------

DataPusher::DataPusher()
{
	convert = &NoConvert();
	display = &StdDisplay();
	RefreshAll();
	SetFrame(edge);
}

void DataPusher::RefreshAll()
{
	edge.Set(this, EditField::StyleDefault().edge, EditField::StyleDefault().activeedge);
	edge.Mouse(HasMouse());
	edge.Push(IsPush());
	edge.SetColor(EditField::StyleDefault().coloredge, GetPaper());
	RefreshFrame();
}

DataPusher::DataPusher(const Convert& convert, const Display& display)
: convert(&convert), display(&display)
{
	RefreshAll();
	SetFrame(edge);
}

DataPusher::DataPusher(const Display& display)
: convert(&NoConvert()), display(&display)
{
	RefreshAll();
	SetFrame(edge);
}

void DataPusher::PerformAction()
{
	WhenPreAction();
	DoAction();
}

void DataPusher::DoAction()
{
	Action();
}

Value DataPusher::GetData() const
{
	return data;
}

void DataPusher::Set(const Value& value)
{
	if(value != data) {
		data = value;
		Refresh();
	}
}

void DataPusher::SetData(const Value& value)
{
	if(value != data) {
		data = value;
		UpdateRefresh();
	}
}

DataPusher& DataPusher::NullText(const char *text, Font fnt, Color ink)
{
	WString txt = text;
	if(nulltext != txt || nullink != ink || nullfont != fnt) {
		nulltext = txt;
		nullink = ink;
		nullfont = fnt;
		Refresh();
	}
	return *this;
}

DataPusher& DataPusher::NullText(const char *text, Color ink)
{
	return NullText(text, StdFont(), ink);
}

Color DataPusher::GetPaper()
{
	return (IsPush() ? Blend(SColorHighlight, SColorFace, 235)
	                 : IsShowEnabled() && !IsReadOnly() ? SColorPaper : SColorFace);
}

Value DataPusher::Format(const Value& v)
{
	Value h = v;
	if(convertby)
		h = convertby(h);
	return convert->Format(h);
}

void DataPusher::Paint(Draw& w)
{
	Rect rc = GetSize();
	Color paper = GetPaper();
	w.DrawRect(rc, paper);
	rc.Deflate(2, 1);
	if(IsPush() && GUI_GlobalStyle() < GUISTYLE_XP)
		rc += Size(1, 1);
	w.Clip(rc);
	display -> Paint(w, rc, Format(data),
		(IsEnabled() ? SColorText : SColorDisabled), Color(paper),
		(HasFocus() ? Display::FOCUS : 0) | (IsReadOnly() ? Display::READONLY : 0));
	w.End();
	if(HasFocus())
		DrawFocus(w, GetSize());
	if(IsNull(data) && !IsNull(nulltext)) {
		int cy = nullfont.Info().GetHeight();
		w.DrawText(rc.left, rc.top + (rc.GetHeight() - cy) / 2, nulltext, nullfont, nullink);
	}
}

void DataPusher::SetDataAction(const Value& value)
{
	SetData(value);
	UpdateActionRefresh();
}

void DataPusher::RefreshPush()
{
	RefreshAll();
}

void DataPusher::RefreshFocus()
{
	RefreshAll();
}

}
