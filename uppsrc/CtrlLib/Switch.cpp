#include "CtrlLib.h"

namespace Upp {

#define LDUMP(x) // DUMP(x)

Switch& Switch::Set(int i, const Value& val) {
	cs.At(i).value = val;
	Refresh();
	return *this;
}

Switch& Switch::Set(int i, const Value& val, const char *text, int gap) {
	Case& v = cs.At(i);
	if(value.IsVoid())
		value = val;
	SetLabel(i, text, gap);
	v.value = val;
	Refresh();
	return *this;
}

Switch& Switch::Add(const Value& val, const char *text, int gap) {
	Set(cs.GetCount(), val, text);
	return *this;
}

Switch& Switch::Add(const char *text, int gap) {
	Set(cs.GetCount(), cs.GetCount(), text, gap);
	return *this;
}

int Switch::GetIndex() const {
	for(int i = 0; i < cs.GetCount(); i++)
		if(cs[i].value == value)
			return i;
	return -1;
}

Switch&  Switch::SetLabel(int i, const char *text, int gap) {
	if(i >= cs.GetCount())
		cs.At(i).value = i;
	Case& v = cs[i];
	v.accesskey = ExtractAccessKey(text, v.label);
	v.gap = gap;
	Refresh();
	return *this;
}

dword Switch::GetAccessKeys() const
{
	dword keys = 0;
	for(int i = 0; i < cs.GetCount(); i++)
		keys |= AccessKeyBit(cs[i].accesskey);
	return keys;
}

void  Switch::AssignAccessKeys(dword used)
{
	for(int i = 0; i < cs.GetCount(); i++) {
		Case& v = cs[i];
		if(!v.accesskey) {
			v.accesskey = ChooseAccessKey(v.label, used);
			if(v.accesskey) Refresh();
			used |= AccessKeyBit(v.accesskey);
		}
	}
	Ctrl::AssignAccessKeys(used);
}

Switch& Switch::SetLabel(const char *text) {
	const char *q = text;
	int i = 0;
	const char *s = text;
	int gap = 0;
	while(*s) {
		if(*s == '^') {
			if(s[1] == '^')
				s += 2;
			else {
				gap = atoi(String(q, s));
				q = ++s;
			}
		}
		else
		if(*s == '|') {
			if(s[1] == '|')
				s += 2;
			else {
				gap = atoi(String(q, s)) | GAP_SEPARATOR;
				q = ++s;
			}
		}
		else
		if(*s == '\r') {
			SetLabel(i++, String(q, s), gap);
			gap = 0;
			q = ++s;
			if(*q == '\n')
				q = ++s;
		}
		else
		if(*s == '\n') {
			SetLabel(i++, String(q, s), gap);
			gap = 0;
			q = ++s;
		}
		else
			s++;
	}
	SetLabel(i++, String(q, s), gap);
	cs.SetCount(i);
	return *this;
}

void  Switch::SetData(const Value& val) {
	if(val != value) {
		value = val;
		Update();
		Refresh();
	}
}

Value Switch::GetData() const {
	return value;
}

void Switch::EnableCase(int i, bool enable) {
	cs.At(i).enabled = enable;
	Refresh();
}

void Switch::EnableValue(const Value& val, bool enable) {
	for(int i = 0; i < cs.GetCount(); i++)
		if(cs[i].value == val)
			EnableCase(i, enable);
}

void Switch::GotFocus() {
	Refresh();
}

void Switch::LostFocus() {
	Refresh();
}

void Switch::Paint(Draw& w) {
	if(cs.GetCount() == 0) return;
	Size sz = GetSize();
	if(!IsTransparent())
		w.DrawRect(0, 0, sz.cx, sz.cy, SColorFace);
	int tcy = GetTextSize("W", font).cy;
	Size isz = CtrlsImg::S0().GetSize();
	linecy = max(mincy, max(isz.cy + DPI(2), tcy));
	bool horz = false;
	if(direction == 1) horz = true;
	for(int pass = !!direction; pass < 2; pass++) { // first pass to decide horz, second to actually draw
		int y = 0;
		int x = 0;
		
		if(horz)
			linecy = sz.cy;
	
		int ty = (linecy - tcy) / 2;
		bool ds = !IsShowEnabled();
		int i;
		light = -1;
		for(i = 0; i < cs.GetCount(); i++) {
			Case& v = cs[i];

			if(v.gap) {
				int gsz = (v.gap & 255) * tcy / 4;
				if(pass && (v.gap & GAP_SEPARATOR)) {
					if(horz)
						w.DrawRect(x + y + gsz / 2, y, DPI(1), linecy, SColorDisabled());
					else
						w.DrawRect(x, y + gsz / 2, sz.cx, DPI(1), SColorDisabled());
				}
				(horz ? x : y) += gsz;
			}
			
			bool dv = ds || !v.enabled;
	
			Size tsz = GetSmartTextSize(v.label, font);
			int iy = (linecy - isz.cy) / 2;
			int width = horz ? tsz.cx + isz.cx + DPI(4) : sz.cx;
			Rect hr = RectC(x, y, width, linecy);
			bool mousein = HasMouseIn(hr);
			if(mousein)
				light = i;
			if(pass) {
				Image img;
				int q = dv ? CTRL_DISABLED :
				        pushindex == i ? CTRL_PRESSED :
				        mousein ? CTRL_HOT :
				        CTRL_NORMAL;
				img = CtrlsImg::Get((v.value == value ? CtrlsImg::I_S1 : CtrlsImg::I_S0) + q);
				w.DrawImage(x, y + iy, img);
				DrawSmartText(w, x + isz.cx + DPI(4), y + ty, sz.cx, v.label, font,
				              dv || IsReadOnly() ? SColorDisabled : GetLabelTextColor(this),
				              VisibleAccessKeys() ? v.accesskey : 0);
				if(HasFocus() && (pushindex == i || v.value == value && pushindex < 0))
					DrawFocus(w, RectC(x + isz.cx + DPI(2), y + ty - DPI(1), tsz.cx + DPI(3), tsz.cy + DPI(2)) & sz);
			}
			v.rect = hr;
			if(horz)
				x += hr.Width() + tcy / 2;
			else
				y += linecy;
		}
		
		if(y > sz.cy)
			horz = true;
	}
	
}

int Switch::GetIndex(Point p) {
	for(int i = 0; i < cs.GetCount(); i++) {
		if(cs[i].rect.Contains(p) && cs[i].enabled)
			return i;
	}
	return -1;
}

void Switch::MouseMove(Point p, dword keyflags) {
	if(IsReadOnly()) return;
	int i = GetIndex(p);
	int a = -1;
	if(keyflags & K_MOUSELEFT)
		a = i;
	if(pushindex != a) {
		pushindex = a;
		Refresh();
	}
	if(i != light)
		Refresh();
}

void Switch::LeftDown(Point p, dword keyflags) {
	if(IsReadOnly()) return;
	if(Ctrl::ClickFocus()) SetWantFocus();
	pushindex = GetIndex();
	MouseMove(p, keyflags);
}

void Switch::LeftUp(Point p, dword keyflags) {
	if(IsReadOnly()) return;
	if(pushindex >= 0 && pushindex < cs.GetCount()) {
		const Value& v = cs[pushindex].value;
		if(v != value) {
			value = v;
			UpdateAction();
		}
		Refresh();
	}
	pushindex = -1;
}

void Switch::MouseLeave() {
	pushindex = -1;
	Refresh();
}

bool Switch::Key(dword key, int) {
	if(IsReadOnly()) return false;
	int index, i;
	index = i = GetIndex();
	if(key == K_UP || key == K_LEFT) {
		for(int t = 0; t < cs.GetCount(); t++) {
			if(--index < 0) index = cs.GetCount() - 1;
			if(cs[index].enabled)
				break;
		}
	}
	else
	if(key == K_DOWN || key == K_RIGHT) {
		for(int t = 0; t < cs.GetCount(); t++) {
			if(++index >= cs.GetCount()) index = 0;
			if(cs[index].enabled)
				break;
		}
	}
	else
		return false;
	if(i != index && cs[index].enabled) {
		value = cs[index].value;
		UpdateActionRefresh();
	}
	return true;
}

bool Switch::HotKey(dword key) {
	if(IsReadOnly()) return false;
	for(int i = 0; i < cs.GetCount(); i++) {
		if(cs[i].enabled && CompareAccessKey(cs[i].accesskey, key)) {
			value = cs[i].value;
			UpdateActionRefresh();
			return true;
		}
	}
	return false;
}

void Switch::CancelMode() {
	pushindex = -1;
}

Switch::Switch() {
	direction = 0;
	linecy = 16;
	mincy = 0;
	Transparent();
	NoInitFocus();
	font = StdFont();
}

Switch::~Switch() {}

}
