#include "CtrlLib.h"

NAMESPACE_UPP

Image DPI(const Image& img)
{
	return GUI_HiDPI() ? CachedRescale(img, 2 * img.GetSize(), FILTER_LANCZOS3) : img;
}

void CtrlsImageLook(Value *look, int i, int n)
{
	while(n--)
		*look++ = CtrlsImg::Get(i++);
}

void CtrlsImageLook(Value *look, int i, const Image& image, const Color *color, int n)
{
	for(int q = 0; q < n; q++)
		*look++ = ChLookWith(CtrlsImg::Get(i++), image, *color++);
}

void CtrlsImageLook(Value *look, int i, const Image& image, int n)
{
	for(int q = 0; q < n; q++)
		*look++ = ChLookWith(CtrlsImg::Get(i++), image);
}

String DeAmp(const char *s)
{
	String out;
	for(; *s; out.Cat(*s++))
		if(*s == '&')
			out.Cat('&');
	return out;
}

Size GetSmartTextSize(const char *text, Font font, int cx) {
	if(*text == '\1') {
		Size sz;
		RichText txt = ParseQTF(text + 1);
		txt.ApplyZoom(GetRichTextStdScreenZoom());
		sz.cx = min(cx, txt.GetWidth());
		sz.cy = txt.GetHeight(Zoom(1, 1), sz.cx);
		return sz;
	}
	return GetTLTextSize(ToUnicode(text, CHARSET_DEFAULT), font);
}

int GetSmartTextHeight(const char *s, int cx, Font font) {
	if(*s == '\1') {
		Size sz;
		RichText txt = ParseQTF(s + 1);
		txt.ApplyZoom(GetRichTextStdScreenZoom());
		return txt.GetHeight(Zoom(1, 1), cx);
	}
	int cy = font.Info().GetHeight();
	int h = cy;
	while(*s) {
		if(*s == '\n')
			h += cy;
		s++;
	}
	return h;
}

void DrawSmartText(Draw& draw, int x, int y, int cx, const char *text, Font font, Color ink, int accesskey) {
	if(*text == '\1') {
		RichText txt = ParseQTF(text + 1, accesskey);
		txt.ApplyZoom(GetRichTextStdScreenZoom());
		txt.Paint(Zoom(1, 1), draw, x, y, cx);
		return;
	}
	DrawTLText(draw, x, y, cx, ToUnicode(text, CHARSET_DEFAULT), font, ink, accesskey);
}

bool CompareAccessKey(int accesskey, dword key)
{
	return accesskey && dword(ToUpper(accesskey & 255) - 'A' + K_ALT_A) == key;
}

int  ExtractAccessKey(const char *s, String& label)
{
	byte akey = 0;
	int  pos = 0;
	String text;
	bool qtf = *s == '\1';
	while(*s)
		if((*s == '&' && !qtf || *s == '\b') && s[1] && s[1] != '&') {
			akey = ToAscii(ToUpper(s[1]));
			pos = text.GetLength() + 1; 
			s++;
		}
		else
			text.Cat(*s++);
	text.Shrink();
	label = text;
	return MAKELONG(akey, pos);
}

int  ChooseAccessKey(const char *text, dword used)
{
	for(const char *s = text; *s; s++) {
		byte ac = *s;
		if(ac < 128 && ac >= 'A' && ac <= 'Z' && (Ctrl::AccessKeyBit(ac) & used) == 0)
			return MAKELONG(ac, s - text + 1);
	}
	for(const char *s = text; *s; s++) {
		byte ac = ToUpper(*s);
		if(ac < 128 && ac >= 'A' && ac <= 'Z' && ac != 'I' && ac != 'L' && (Ctrl::AccessKeyBit(ac) & used) == 0)
			return ac;
	}
	for(const char *s = text; *s; s++) {
		byte ac = ToUpper(*s);
		if(ac < 128 && ac >= 'A' && ac <= 'Z' && (Ctrl::AccessKeyBit(ac) & used) == 0)
			return ac;
	}
	return 0;
}

DrawLabel::DrawLabel()
{
	push = focus = disabled = false;
	lspc = rspc = 0;
	limg_never_hide = false;
	rimg_never_hide = false;
	ink = disabledink = Null;
	align = valign = ALIGN_CENTER;
	accesskey = 0;
	accesspos = -1;
	font = StdFont();
	nowrap = false;
}

Size DrawLabel::GetSize(int txtcx) const
{
	return GetSize(txtcx, limg.GetSize(), lspc, rimg.GetSize(), rspc);
}

Size DrawLabel::GetSize(int txtcx, Size sz1, int lspc, Size sz2, int rspc) const
{
	Size isz(0, 0);
	Size txtsz = *text ? GetSmartTextSize(text, font, nowrap ? INT_MAX/2 : txtcx)
	                   : paintrect.GetStdSize();

	if(!IsNull(lspc)) {
		isz.cx = lspc;
		isz.cy = sz1.cy;
		isz.cx += sz1.cx;
	}

	if(!IsNull(rspc)) {
		isz.cx += rspc;
		if(sz2.cy > isz.cy) isz.cy = sz2.cy;
		isz.cx += sz2.cx;
	}

	isz.cy = max(txtsz.cy, max(sz1.cy, sz2.cy));
	isz.cx += txtsz.cx;

	return isz;
}

Image DisImage(const Image& m)
{
	Image mm = Grayscale(m, 200);
	ImageBuffer ib(mm);
	RGBA *s = ~ib;
	RGBA *e = s + ib.GetLength();
	while(s < e)
		(s++)->a /= 3;
	Premultiply(ib);
	return ib;
}

Image DisabledImage(const Image& img, bool dis)
{
	return dis ? MakeImage(img, GUI_GlobalStyle() == GUISTYLE_CLASSIC ? Etched : DisImage)
	           : img;
}

Color GetLabelTextColor(const Ctrl *ctrl)
{
	if(!IsLabelTextColorMismatch())
		return SColorLabel();
	while(ctrl) {
		if(!ctrl->IsTransparent()) {
			if(dynamic_cast<const TopWindow *>(ctrl) || dynamic_cast<const TabCtrl *>(ctrl) ||
			   dynamic_cast<const ToolBar *>(ctrl) || dynamic_cast<const MenuBar *>(ctrl) ||
			   dynamic_cast<const StaticRect *>(ctrl) || dynamic_cast<const StaticBarArea *>(ctrl))
				break;
			return SColorText();
		}
		ctrl = ctrl->GetParent();
	}
	return SColorLabel();
}

Size DrawLabel::Paint(Ctrl *ctrl, Draw& w, const Rect& r, bool visibleaccesskey) const
{
	int lspc = this->lspc;
	int rspc = this->rspc;
	Size sz1 = limg.GetSize();
	Size sz2 = rimg.GetSize();
	int txtcx = r.GetWidth() - sz1.cx - Nvl(lspc, 0) - sz2.cx - Nvl(rspc, 0);
	Size txtsz = *text ? GetSmartTextSize(text, font, nowrap ? INT_MAX/2 : txtcx) : paintrect.GetStdSize();
	if(txtsz.cx) {
		if(!rimg_never_hide && txtsz.cx + sz1.cx + sz2.cx + Nvl(lspc, 0) + Nvl(rspc, 0) > r.GetWidth()) {
			sz2.cx = 0;
			rspc = 0;
		}
		if(!limg_never_hide && txtsz.cx + sz1.cx + sz2.cx + Nvl(lspc, 0) + Nvl(rspc, 0) > r.GetWidth()) {
			sz1.cx = 0;
			lspc = 0;
		}
	}
	Size isz = GetSize(txtcx, sz1, lspc, sz2, rspc);
	Point p = r.TopLeft(), ip;
	if(align == ALIGN_LEFT)
		p.x = r.left;
	else
	if(align == ALIGN_RIGHT)
		p.x = r.right - isz.cx;
	else
	if(align == ALIGN_CENTER)
		p.x = (r.right + r.left - isz.cx) / 2;
	if(valign == ALIGN_TOP)
		p.y = r.top;
	else
	if(valign == ALIGN_BOTTOM)
		p.y = r.bottom - isz.cy;
	else
	if(valign == ALIGN_CENTER)
		p.y = (r.bottom + r.top - txtsz.cy) / 2;
	Color color = disabled && !IsNull(disabledink) ? disabledink : ink;
	if(IsNull(color))
		color = disabled ? SColorDisabled : GetLabelTextColor(ctrl);
	int ix;
	if(IsNull(lspc))
		ix = r.left + push;
	else {
		ix = p.x + push;
		p.x += sz1.cx;
		p.x += lspc;
	}
	int iy = push + (r.top + r.bottom - sz1.cy) / 2;

	if(sz1.cx) {
		if(IsNull(lcolor))
			w.DrawImage(ix, iy, DisabledImage(limg, disabled));
		else
			w.DrawImage(ix, iy, limg, lcolor);
	}
	iy = push + (r.top + r.bottom - sz2.cy) / 2;
	ix = (IsNull(rspc) ? r.right - sz2.cx : p.x + txtsz.cx + rspc) + push;
	if(sz2.cx) {
		if(IsNull(rcolor))
			w.DrawImage(ix, iy, DisabledImage(rimg, disabled));
		else
			w.DrawImage(ix, iy, rimg, rcolor);
	}
	paintrect.Paint(w, p.x + push, p.y + push, txtsz.cx, isz.cy, color, Null);
	if(*text) {
		if(disabled && *text != '\1')
			DrawSmartText(w, p.x + push + 1, p.y + push + 1,
			              nowrap ? INT_MAX/2 : txtcx, text, font, SColorPaper);
		DrawSmartText(w, p.x + push, p.y + push, nowrap ? INT_MAX/2 : txtcx,
		              text, font, color, visibleaccesskey ? accesskey : 0);
		if(focus)
			DrawFocus(w, p.x - 2, p.y, txtsz.cx + 5, isz.cy);
	}

	return isz;
}

Size DrawLabel::Paint(Ctrl *ctrl, Draw& w, int x, int y, int cx, int cy, bool visibleaccesskey) const
{
	return Paint(ctrl, w, RectC(x, y, cx, cy), visibleaccesskey);
}

Size DrawLabel::Paint(Draw& w, const Rect& r, bool visibleaccesskey) const
{
	return Paint(NULL, w, r, visibleaccesskey);
}

Size DrawLabel::Paint(Draw& w, int x, int y, int cx, int cy, bool vak) const
{
	return Paint(w, RectC(x, y, cx, cy), vak);
}

void LabelBase::LabelUpdate() {}

LabelBase& LabelBase::SetLeftImage(const Image& img, int spc, bool never_hide) {
	lbl.limg = img;
	lbl.lspc = spc;
	lbl.limg_never_hide = never_hide;
	LabelUpdate();
	return *this;
}

LabelBase& LabelBase::SetRightImage(const Image& img, int spc, bool never_hide) {
	lbl.rimg = img;
	lbl.rspc = spc;
	lbl.rimg_never_hide = never_hide;
	LabelUpdate();
	return *this;
}

LabelBase& LabelBase::SetPaintRect(const PaintRect& paintrect) {
	lbl.paintrect = paintrect;
	LabelUpdate();
	return *this;
}


LabelBase& LabelBase::SetText(const char *text) {
	lbl.text = text;
	LabelUpdate();
	return *this;
}

LabelBase& LabelBase::SetFont(Font font) {
	if(lbl.font != font) {
		lbl.font = font;
		LabelUpdate();
	}
	return *this;
}

LabelBase& LabelBase::NoWrap(bool b)
{
	if(lbl.nowrap != b) {
		lbl.nowrap = b;
		LabelUpdate();
	}
	return *this;
}

LabelBase& LabelBase::SetInk(Color ink, Color disabledink) {
	if(lbl.ink != ink || lbl.disabledink != disabledink) {
		lbl.ink = ink;
		lbl.disabledink = disabledink;
		LabelUpdate();
	}
	return *this;
}

LabelBase& LabelBase::SetAlign(int align) {
	if(lbl.align != align) {
		lbl.align = align;
		LabelUpdate();
	}
	return *this;
}

LabelBase& LabelBase::SetVAlign(int valign) {
	if(lbl.valign != valign) {
		lbl.valign = valign;
		LabelUpdate();
	}
	return *this;
}

Size LabelBase::PaintLabel(Ctrl *ctrl, Draw& w, const Rect& r, bool disabled, bool push, bool focus, bool vak)
{
	DrawLabel lbl1 = lbl;
	lbl1.disabled = disabled;
	lbl1.push = push;
	lbl1.focus = focus;
	return lbl1.Paint(ctrl, w, r, vak);
}

Size LabelBase::PaintLabel(Ctrl *ctrl, Draw& w, int x, int y, int cx, int cy, bool disabled, bool push, bool focus, bool vak)
{
	return PaintLabel(ctrl, w, RectC(x, y, cx, cy), disabled, push, focus, vak);
}

Size LabelBase::PaintLabel(Draw& w, const Rect& r, bool disabled, bool push, bool focus, bool vak)
{
	return PaintLabel(NULL, w, r, disabled, push, focus, vak);
}


Size LabelBase::PaintLabel(Draw& w, int x, int y, int cx, int cy, bool disabled, bool push, bool focus, bool vak)
{
	return PaintLabel(w, RectC(x, y, cx, cy), disabled, push, focus, vak);
}

Size LabelBase::GetLabelSize() const
{
	return lbl.GetSize();
}

void LinkToolTipIn__();

LabelBase::~LabelBase() {
	LinkToolTipIn__();
}

void DrawFocus(Draw& w, int x, int y, int cx, int cy, Color c) {
	w.Clipoff(x, y, cx, cy);
	for(int a = 0; a < cx; a += CtrlImg::focus_h().GetWidth()) {
		w.DrawImage(a, 0, CtrlImg::focus_h(), c);
		w.DrawImage(a, cy - 1, CtrlImg::focus_h(), c);
	}
	for(int a = 0; a < cy; a += CtrlImg::focus_v().GetHeight()) {
		w.DrawImage(0, a, CtrlImg::focus_v(), c);
		w.DrawImage(cx - 1, a, CtrlImg::focus_v(), c);
	}
	w.End();
}

void DrawFocus(Draw& w, const Rect& r, Color c) {
	DrawFocus(w, r.left, r.top, r.Width(), r.Height(), c);
}

void DrawHorzDrop(Draw& w, int x, int y, int cx)
{
	w.DrawRect(x, y, cx, 2, SColorHighlight);
	w.DrawRect(x, y - 2, 1, 6, SColorHighlight);
	w.DrawRect(x + cx - 1, y - 2, 1, 6, SColorHighlight);
	w.DrawRect(x + 1, y - 1, 1, 4, SColorHighlight);
	w.DrawRect(x + cx - 2, y - 1, 1, 4, SColorHighlight);
}

void DrawVertDrop(Draw& w, int x, int y, int cy)
{
	w.DrawRect(x, y, 2, cy, SColorHighlight);
	w.DrawRect(x - 2, y, 6, 1, SColorHighlight);
	w.DrawRect(x - 2, y + cy - 1, 6, 1, SColorHighlight);
	w.DrawRect(x - 1, y + 1, 4, 1, SColorHighlight);
	w.DrawRect(x - 1, y + cy - 2, 4, 1, SColorHighlight);
}

Point GetDragScroll(Ctrl *ctrl, Point p, Size max)
{
	if(ctrl->IsReadOnly())
		return Point(0, 0);
	Size sz = ctrl->GetSize();
	Size sd = min(sz / 6, Size(16, 16));
	Point d(0, 0);
	if(p.x < sd.cx)
		d.x = p.x - sd.cx;
	if(p.x > sz.cx - sd.cx)
		d.x = p.x - sz.cx + sd.cx;
	if(p.y < sd.cy)
		d.y = p.y - sd.cy;
	if(p.y > sz.cy - sd.cy)
		d.y = p.y - sz.cy + sd.cy;
	d.x = minmax(d.x, -max.cx, max.cx);
	d.y = minmax(d.y, -max.cy, max.cy);
	return d;
}

Point GetDragScroll(Ctrl *ctrl, Point p, int max)
{
	return GetDragScroll(ctrl, p, Size(max, max));
}

Point DisplayPopup::Op(Point p)
{
	return p + GetScreenView().TopLeft() - ctrl->GetScreenView().TopLeft();
}

void DisplayPopup::LeftDown(Point p, dword flags)
{
	ctrl->LeftDown(Op(p), flags);
}

void DisplayPopup::LeftDrag(Point p, dword flags)
{
	Cancel();
	ctrl->LeftDrag(Op(p), flags);
}

void DisplayPopup::LeftDouble(Point p, dword flags)
{
	ctrl->LeftDouble(Op(p), flags);
}

void DisplayPopup::RightDown(Point p, dword flags)
{
	ctrl->RightDown(Op(p), flags);
}

void DisplayPopup::LeftUp(Point p, dword flags)
{
	ctrl->LeftUp(Op(p), flags);
}

void DisplayPopup::MouseWheel(Point p, int zdelta, dword flags)
{
	ctrl->MouseWheel(Op(p), zdelta, flags);
}

void DisplayPopup::MouseLeave()
{
	Cancel();
}

void DisplayPopup::MouseMove(Point p, dword flags)
{
	p += GetScreenView().TopLeft();
	if(!slim.Contains(p))
		MouseLeave();
}

void DisplayPopup::Paint(Draw& w)
{
	Rect r = GetSize();
	w.DrawRect(r, SColorPaper);
	if(display) {
		display->PaintBackground(w, r, value, ink, paper, style);
		r.left += margin;
		display->Paint(w, r, value, ink, paper, style);
	}
}

DisplayPopup::DisplayPopup()
{
	SetFrame(BlackFrame());
	display = NULL;
	paper = ink = Null;
	style = 0;
	item = slim = Null;
	margin = 0;
	ONCELOCK {
		InstallStateHook(StateHook);
		InstallMouseHook(MouseHook);
	}
	LinkBefore(all());
}

DisplayPopup::~DisplayPopup()
{
	Unlink();
}

void DisplayPopup::Sync()
{
	if(!IsMainThread()) {
		PostCallback(PTEBACK(Sync));
		return;
	}
	if(display && ctrl && !ctrl->IsDragAndDropTarget() && !IsDragAndDropTarget()) {
		Ctrl *top = ctrl->GetTopCtrl();
		if(top && top->HasFocusDeep()) {
			Size sz = display->GetStdSize(value);
			if(sz.cx + 2 * margin > item.GetWidth() || sz.cy > item.GetHeight()) {
				Rect vw = ctrl->GetScreenView();
				slim = (item + vw.TopLeft()) & vw;
				if(slim.Contains(GetMousePos())) {
					Rect r = item;
					r.right = max(r.right, r.left + sz.cx + 2 * margin);
					r.bottom = max(r.bottom, r.top + sz.cy);
					r.Inflate(1, 1);
					r.Offset(ctrl->GetScreenView().TopLeft());

					Rect wa = GetWorkArea(r.BottomLeft());
					Size sz = r.GetSize();
					if(r.left < wa.left) {
						r.left = wa.left;
						r.right = min(wa.right, r.left + sz.cx);
					}
					else
					if(r.right > wa.right) {
						r.left = max(wa.left, wa.right - sz.cx);
						r.right = wa.right;
					}
					if(r.top < wa.top) {
						r.top = wa.top;
						r.bottom = min(wa.bottom, wa.top + sz.cy);
					}
					else
					if(r.bottom > wa.bottom) {
						r.top = max(wa.top, wa.bottom - sz.cy);
						r.bottom = wa.bottom;
					}
					
					SetRect(r);
					if(!IsOpen())
						Ctrl::PopUp(ctrl, true, false, false);
					Refresh();
					return;
				}
			}
		}
	}
	if(IsOpen())
		Close();
}

Link<DisplayPopup> *DisplayPopup::all()
{
	static Link<DisplayPopup> all;
	return &all;
}

void DisplayPopup::SyncAll()
{
	int n = 0;
	for(DisplayPopup *p = all()->Link<DisplayPopup>::GetNext(); p != all(); p = p->Link<DisplayPopup>::GetNext())
		if(p->ctrl && p->ctrl->IsOpen()) {
			p->Sync();
			n++;
		}
}

bool DisplayPopup::StateHook(Ctrl *, int reason)
{
	if(reason == FOCUS)
		SyncAll();
	return false;
}


bool DisplayPopup::MouseHook(Ctrl *, bool, int, Point, int, dword)
{
	SyncAll();
	return false;
}

void DisplayPopup::Cancel()
{
	display = NULL;
	Sync();
}

bool DisplayPopup::IsOpen()
{
	return Ctrl::IsOpen();
}

bool DisplayPopup::HasMouse()
{
	return Ctrl::HasMouse() || ctrl && ctrl->HasMouse();
}

void DisplayPopup::Set(Ctrl *_ctrl, const Rect& _item,
                       const Value& _value, const Display *_display,
                       Color _ink, Color _paper, dword _style, int _margin)
{
	if(item != _item || ctrl != _ctrl || value != _value || display != _display || ink != _ink ||
	   paper != _paper || style != _style) {
		item = _item;
		ctrl = _ctrl;
		value = _value;
		display = _display;
		ink = _ink;
		paper = _paper;
		style = _style;
		margin = _margin;
		if(ctrl)
			Tip(ctrl->GetTip());
		Sync();
	}
	else
		Refresh();
}

END_UPP_NAMESPACE
