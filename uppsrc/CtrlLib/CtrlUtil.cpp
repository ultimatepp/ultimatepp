#include "CtrlLib.h"

NAMESPACE_UPP

#define LLOG(x) // RLOG(x)

// #define SLOWANIMATION

#define IMAGECLASS CtrlImg
#define IMAGEFILE  <CtrlLib/Ctrl.iml>
#include <Draw/iml_source.h>

void Animate(Ctrl& c, const Rect& target, int type)
{
	if(type < 0)
		type = GUI_PopUpEffect();
	Rect r0 = c.GetRect();
	dword time0 = GetTickCount();
	int anitime = 150;
#ifdef SLOWANIMATION
	anitime = 1500;
#endif
	if(type)
		for(;;) {
			int t = int(GetTickCount() - time0);
			if(t > anitime)
				break;
			if(type == GUIEFFECT_SLIDE) {
				Rect r = r0;
				if(r.left > target.left)
				   r.left -= ((r.left - target.left)* t) / anitime;
				if(r.top > target.top)
				   r.top -= ((r.top - target.top) * t) / anitime;
				if(r.right < target.right)
				   r.right += ((target.right - r.right) * t) / anitime;
				if(r.bottom < target.bottom)
				   r.bottom += ((target.bottom - r.bottom) * t) / anitime;
				if(r.GetWidth() > target.GetWidth())
				   r.right = r.left + target.GetWidth();
				if(r.GetHeight() > target.GetHeight())
				   r.bottom = r.top + target.GetHeight();
				c.SetRect(r);
				if(r == target)
					break;
			}
			else
			if(type == GUIEFFECT_FADE)
				c.SetAlpha((byte)(255 * t / anitime));
			else
				break;
			c.Sync();
			Sleep(0);
#ifdef SLOWANIMATION
			Sleep(100);
#endif
		}
	c.SetRect(target);
	c.SetAlpha(255);
}

void Animate(Ctrl& c, int x, int y, int cx, int cy, int type)
{
	Animate(c, RectC(x, y, cx, cy), type);
}

bool CtrlLibDisplayError(const Value& e) {
	if(!e.IsError())
		return false;
	String s = GetErrorText(e);
	if(s.IsEmpty())
		s = t_("Invalid data.");
	Exclamation(s);
	return true;
}

INITBLOCK
{
	DisplayErrorFn() = &CtrlLibDisplayError;
}
/*
String SaveCtrlLayout(Ctrl::LogPos p, const String& classname, const String& variable,
					  const String& label, const String& help) {
	String out;
	if(classname.IsEmpty())
		out << "\tUNTYPED(";
	else
		out << "\tITEM(" << classname << ", ";
	out << variable << ", ";
	switch(p.x.GetAlign()) {
	case Ctrl::LEFT:   out << Format("LeftPos(%d, %d).", p.x.GetA(), p.x.GetB()); break;
	case Ctrl::RIGHT:  out << Format("RightPos(%d, %d).", p.x.GetA(), p.x.GetB()); break;
	case Ctrl::SIZE:   out << Format("HSizePos(%d, %d).", p.x.GetA(), p.x.GetB()); break;
	case Ctrl::CENTER: out << Format("HCenterPos(%d, %d).", p.x.GetB(), p.x.GetA()); break;
	}
	switch(p.y.GetAlign()) {
	case Ctrl::TOP:    out << Format("TopPos(%d, %d)", p.y.GetA(), p.y.GetB()); break;
	case Ctrl::BOTTOM: out << Format("BottomPos(%d, %d)", p.y.GetA(), p.y.GetB()); break;
	case Ctrl::SIZE:   out << Format("VSizePos(%d, %d)", p.y.GetA(), p.y.GetB()); break;
	case Ctrl::CENTER: out << Format("VCenterPos(%d, %d)", p.y.GetB(), p.y.GetA()); break;
	}
	if(!label.IsEmpty()) {
		out << ".SetLabel(\"";
		for(const char *s = label; *s; s++)
			if(*s == '\n')
				out.Cat("\\n");
			else
				out.Cat(*s);
		out << "\")";
	}
	if(!help.IsEmpty()) {
		out << ".HelpC(\"";
		for(const char *s = help; *s; s++)
			if(*s == '\n')
				out.Cat("\\n");
			else
				out.Cat(*s);
		out << "\")";
	}
	out << ")\n";
	return out;
}
*/
void Show2(Ctrl& ctrl1, Ctrl& ctrl2, bool show) {
	ctrl1.Show(show);
	ctrl2.Show(show);
}

void Hide2(Ctrl& ctrl1, Ctrl& ctrl2) {
	Show2(ctrl1, ctrl2, false);
}

void DelayCallback::Invoke() {
	KillTimeCallback(this);
	SetTimeCallback(delay, target, this);
}

void sSyncLabel(Label *lbl, const char *label, EditString *text)
{
	lbl->SetLabel(String().Cat() << label << " (" << text->GetLength() << "/" << text->GetMaxLength() << ")");
}

bool EditText(String& s, const char *title, const char *label, int (*f)(int), int maxlen, bool notnull)
{
	WithEditStringLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, title);
	dlg.lbl = label;
	dlg.text = s.ToWString();
	dlg.text.NotNull(notnull);
	dlg.text.SetFilter(f);
	if(maxlen) {
		dlg.text.MaxLen(maxlen);
		dlg.text <<= callback3(sSyncLabel, &dlg.lbl, label, &dlg.text);
		dlg.text.WhenAction();
	}
	if(dlg.Execute() == IDOK) {
		s = dlg.text;
		return true;
	}
	return false;
}

bool EditText(String& s, const char *title, const char *label, int (*filter)(int), int maxlen)
{
	return EditText(s, title, label, filter, maxlen, false);
}

bool EditText(String& s, const char *title, const char *label, int maxlen)
{
	return EditText(s, title, label, CharFilterUnicode, maxlen, false);
}

bool EditTextNotNull(String& s, const char *title, const char *label, int (*filter)(int), int maxlen)
{
	return EditText(s, title, label, filter, maxlen, true);
}

bool EditTextNotNull(String& s, const char *title, const char *label, int maxlen)
{
	return EditText(s, title, label, CharFilterUnicode, maxlen, true);
}

bool EditText(WString& s, const char *title, const char *label, int (*f)(int), int maxlen, bool notnull)
{
	String ss = s.ToString();
	if(EditText(ss, title, label, f, maxlen, notnull)) {
		s = ss.ToWString();
		return true;
	}
	return false;
}

bool EditText(WString& s, const char *title, const char *label, int (*filter)(int), int maxlen)
{
	return EditText(s, title, label, filter, maxlen, false);
}

bool EditText(WString& s, const char *title, const char *label, int maxlen)
{
	return EditText(s, title, label, CharFilterUnicode, maxlen, false);
}

bool EditTextNotNull(WString& s, const char *title, const char *label, int (*filter)(int), int maxlen)
{
	return EditText(s, title, label, filter, maxlen, true);
}

bool EditTextNotNull(WString& s, const char *title, const char *label, int maxlen)
{
	return EditText(s, title, label, CharFilterUnicode, maxlen, true);
}

bool EditNumber(int& n, const char *title, const char *label, int min, int max, bool notnull)
{
	WithEditIntLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, title);
	dlg.lbl = label;
	dlg.number <<= n;
	dlg.number.MinMax(min, max);
	dlg.number.NotNull(notnull);
	if(dlg.Execute() == IDOK) {
		n = ~dlg.number;
		return true;
	}
	return false;
}

bool EditNumber(double& n, const char *title, const char *label, double min, double max, bool notnull)
{
	WithEditDoubleLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, title);
	dlg.lbl = label;
	dlg.number <<= n;
	dlg.number.MinMax(min, max);
	dlg.number.NotNull(notnull);
	if(dlg.Execute() == IDOK) {
		n = ~dlg.number;
		return true;
	}
	return false;	
}

bool EditDateDlg(Date& d, const char *title, const char *label, Date min, Date max, bool notnull)
{
	WithEditDateLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, title);
	dlg.lbl = label;
	dlg.date <<= d;
	dlg.date.MinMax(min, max);
	dlg.date.NotNull(notnull);
	if(dlg.Execute() == IDOK) {
		d = ~dlg.date;
		return true;
	}
	return false;	
}

Callback CtrlRetriever::operator<<=(Callback cb)
{
	for(int i = 0; i < item.GetCount(); i++) {
		CtrlItem0 *m = dynamic_cast<CtrlItem0 *>(&item[i]);
		if(m)
			m->ctrl->WhenAction = cb;
	}
	return cb;
}

void CtrlRetriever::Retrieve()
{
	for(int i = 0; i < item.GetCount(); i++)
		item[i].Retrieve();
}

void IdCtrls::Add(Id id, Ctrl& ctrl)
{
	Item& m = item.Add();
	m.id = id;
	m.ctrl = &ctrl;
}

ValueMap IdCtrls::Get() const
{
	ValueMap m;
	for(int i = 0; i < item.GetCount(); i++)
		m.Add(item[i].id, item[i].ctrl->GetData());
	return m;
}

void IdCtrls::Set(const ValueMap& m)
{
	for(int i = 0; i < item.GetCount(); i++)
		item[i].ctrl->SetData(m[item[i].id]);
}

void Set(ArrayCtrl& array, int ii, IdCtrls& m)
{
	for(int i = 0; i < m.GetCount(); i++)
		array.Set(ii, m(i), ~m[i]);
}

void Get(ArrayCtrl& array, int ii, IdCtrls& m)
{
	for(int i = 0; i < m.GetCount(); i++)
		m[i] <<= array.Get(ii, m(i));
}

String sProfile(const MemoryProfile& mem)
{
	return AsString(mem);
}

void MemoryProfileInfo() {
	MemoryProfile mem;
	String text = sProfile(mem);
	const MemoryProfile *peak = PeakMemoryProfile();
	if(peak)
		text << "\r\n=== Peak memory profile\r\n" << sProfile(*peak);
	PromptOK("[C " + DeQtfLf(text));
};

FileSelButton::FileSelButton(MODE mode, const char *title)
: title(title), mode(mode)
{
	button.NoWantFocus();
	button.SetImage(CtrlImg::right_arrow());
	button <<= THISBACK(OnAction);
}

void FileSelButton::OnAction()
{
	Ctrl *owner = button.GetParent();
	ASSERT(owner);
	String old = ~*owner;
	if(mode == MODE_DIR)
		ActiveDir(old);
	else
		Set(old);
	if(mode == MODE_OPEN ? ExecuteOpen(title) : mode == MODE_SAVE ? ExecuteSaveAs(title) : ExecuteSelectDir(title))
	{
		*owner <<= Get();
		owner->Action();
	}
}

void FileSelButton::Detach()
{
	Ctrl *p = button.GetParent();
	if(p) p->RemoveFrame(button);
}

END_UPP_NAMESPACE
