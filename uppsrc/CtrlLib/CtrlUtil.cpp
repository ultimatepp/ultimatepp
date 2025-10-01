#include "CtrlLib.h"

namespace Upp {

#define LLOG(x) // RLOG(x)

#define IMAGECLASS CtrlImg
#define IMAGEFILE  <CtrlLib/Ctrl.iml>
#include <Draw/iml_source.h>

void Animate(Ctrl& c, const Rect& target, int type)
{
	if(type < 0)
		type = GUI_PopUpEffect();
	Rect r0 = c.GetRect();
	dword time0 = msecs();
	int anitime = 150;
	if(type)
		for(;;) {
			int t = int(msecs() - time0);
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
			Ctrl::ProcessEvents();
			Ctrl::GuiSleep(0);
		}
	c.SetRect(target);
	c.SetAlpha(255);
}

void Animate(Ctrl& c, int x, int y, int cx, int cy, int type)
{
	Animate(c, RectC(x, y, cx, cy), type);
}

void Animate(Event<double> update, int duration)
{
	if(duration < 1)
		return;

	int start = msecs();
	for(;;) {
        int elapsed = msecs() - start;
        if(elapsed > duration)
            break;
        double t = min(1.0, (double) elapsed / (double) duration);
        t = t * t * (3 - 2 * t);  // Ease-in-out (smoother movement).
        update(t);
        Ctrl::ProcessEvents();
        Ctrl::GuiSleep(0);
    }
    update(1);
}

void Animate(Vector<Ptr<Ctrl>>& ctrls, const Vector<Rect>& targets, int duration)
{
	Vector<Rect> data;
	for(const Ptr<Ctrl>& c : ctrls)
		data.Add(Nvl(c->GetRect(), {0, 0, 0, 0}));

	Animate(data, targets, [&] {
		for(int i = 0; i < ctrls.GetCount(); i++)
			if(ctrls[i])
				ctrls[i]->SetRect(data[i]);
	}, duration);
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
	dlg.text <<= s.ToWString();
	dlg.text.NotNull(notnull);
	dlg.text.SetFilter(f);
	if(maxlen) {
		dlg.text.MaxLen(maxlen);
		dlg.text <<= callback3(sSyncLabel, &dlg.lbl, label, &dlg.text);
		dlg.text.WhenAction();
	}
	if(dlg.Execute() == IDOK) {
		s = ~dlg.text;
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

Event<> CtrlRetriever::operator^=(Event<> cb)
{
	for(int i = 0; i < item.GetCount(); i++) {
		CtrlItem0 *m = dynamic_cast<CtrlItem0 *>(&item[i]);
		if(m)
			m->ctrl->WhenAction = cb;
	}
	return cb;
}

Event<> CtrlRetriever::operator<<(Event<> cb)
{
	for(int i = 0; i < item.GetCount(); i++) {
		CtrlItem0 *m = dynamic_cast<CtrlItem0 *>(&item[i]);
		if(m)
			m->ctrl->WhenAction << cb;
	}
	return cb;
}

void CtrlRetriever::Set()
{
	for(int i = 0; i < item.GetCount(); i++)
		item[i].Set();
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

bool IdCtrls::Accept()
{
	for(int i = 0; i < item.GetCount(); i++)
		if(!item[i].ctrl->Accept()) return false;
	return true;
}

void IdCtrls::ClearModify() {
	for(int i = 0; i < item.GetCount(); i++)
		item[i].ctrl->ClearModify();
}

bool IdCtrls::IsModified() {
	for(int i = 0; i < item.GetCount(); i++)
		if(item[i].ctrl->IsModified()) return true;
	return false;
}

void IdCtrls::Enable(bool b)
{
	for(int i = 0; i < item.GetCount(); i++)
		item[i].ctrl->Enable(b);
}

void IdCtrls::SetNull()
{
	for(int i = 0; i < item.GetCount(); i++)
		item[i].ctrl->SetData(Null);
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

Event<> IdCtrls::operator<<(Event<> action)
{
	for(int i = 0; i < item.GetCount(); i++)
		item[i].ctrl->WhenAction << action;
	return action;
}

Event<> IdCtrls::operator^=(Event<> action)
{
	for(int i = 0; i < item.GetCount(); i++)
		item[i].ctrl->WhenAction = action;
	return action;
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
	button.SetImage(mode == MODE_DIR ? CtrlImg::DirSmall() : CtrlImg::FileSmall());
	button << [=] { OnAction(); };
}

void FileSelButton::Attach(Ctrl& parent)
{
	button.Width(DPI(20));
	bool not_mb = true;
	for(int i = 0; i < parent.GetFrameCount(); i++)
		if(dynamic_cast<MultiButtonFrame *>(&parent.GetFrame(i))) {
			not_mb = false;
			break;
		}
	if(not_mb && dynamic_cast<EditField *>(&parent)) {
		parent.InsertFrame(0, button);
		parent.InsertFrame(1, RightGapFrame());
	}
	else
		parent.AddFrame(button);
}

void FileSelButton::OnAction()
{
	Ctrl *owner = button.GetParent();
	ASSERT(owner);
	String old = ~*owner;
	if(IsNull(old))
		LoadFromGlobal(old, "FileSelButtonLastPath");
	if(mode == MODE_DIR)
		for(int i = 0; i < 8 && old.GetCount(); i++) {
			if(DirectoryExists(old)) {
				ActiveDir(old);
				break;
			}
			old = GetFileFolder(old);
		}
	else
		Set(old);
	String path;
	if(mode == MODE_OPEN ? ExecuteOpen(title) : mode == MODE_SAVE ? ExecuteSaveAs(title) : ExecuteSelectDir(title)) {
		path = Get();
		*owner <<= path;
		owner->Action();
		WhenSelected();
	}
	else
		path = GetActiveDir();
	if(path.GetCount())
		StoreToGlobal(path, "FileSelButtonLastPath");
}

void FileSelButton::Detach()
{
	Ctrl *p = button.GetParent();
	if(p) p->RemoveFrame(button);
}

struct ZoomIconMaker : ImageMaker {
	double zoom;

	String Key() const override {
		return String((byte *)&zoom, sizeof(zoom));
	}
	Image Make() const override {
		Size sz(DPI(16), DPI(16));
		ImagePainter w(sz);
		w.Clear(RGBAZero());
		w.Move(DPI(11), DPI(11)).Line(DPI(16), DPI(16)).Stroke(DPI(2), SBlack());
		w.Circle(DPI(7), DPI(7), IsUHDMode() ? 12 : 6.5).Stroke(IsUHDMode() ? 3 : 1, SBlack());
		String txt = AsString(int(zoom * 100));
		Image numbers = IsUHDMode() ? CtrlImg::Numbers2() : CtrlImg::Numbers1();
		int gcx = IsUHDMode() ? 6 : 4;
		Size tsz(txt.GetCount() * gcx, numbers.GetHeight());
		int y = DPI(7) - tsz.cy / 2;
		int x = DPI(7) - tsz.cx / 2;
		for(int c : txt) {
			w.DrawImage(x, y, gcx, tsz.cy, numbers, RectC((c - '0') * gcx, 0, gcx, tsz.cy), SLtBlue());
			x += gcx;
		}
		return w;
	}
};

Image MakeZoomIcon(double scale)
{
	ZoomIconMaker m;
	m.zoom = scale;
	return MakeImage(m);
}

}
