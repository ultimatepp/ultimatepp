#include "CtrlCore.h"

NAMESPACE_UPP

#define LLOG(x) // LOG(x)

#define IMAGECLASS CtrlCoreImg
#define IMAGEFILE <CtrlCore/Ctrl.iml>
#include <Draw/iml_source.h>

static bool StdDisplayErrorFn(const Value& e)
{
	if(!e.IsError())
		return false;
	String s = GetErrorText(e);
#ifdef PLATFORM_WIN32
#ifdef PLATFORM_WINCE
	MessageBox(NULL, ToSystemCharset(s), ToSystemCharset(GetExeTitle()), MB_OK | MB_ICONQUESTION);
#else
	MessageBox(NULL, s, GetExeTitle(), MB_OK | MB_ICONQUESTION);
#endif
#else
	fputs(String().Cat() << GetExeTitle() << ": " << s << '\n', stderr);
#endif
	return true;
}

bool (*&DisplayErrorFn())(const Value& v)
{
	static bool (*errfn)(const Value& v) = &StdDisplayErrorFn;
	return errfn;
}

Ctrl *Ctrl::LoopCtrl;
int   Ctrl::LoopLevel;

bool Ctrl::MemoryCheck;

void   Ctrl::SetData(const Value&) {}
Value  Ctrl::GetData() const       { return Value(); }

void Ctrl::Paint(Draw& draw)                        {}
int  Ctrl::OverPaint() const                        { return 0; }

void Ctrl::Activate()                               {}
void Ctrl::Deactivate()                             {}
void Ctrl::CancelMode()                             {}
void Ctrl::MouseEnter(Point p, dword keyflags)      {}
void Ctrl::LeftDown(Point p, dword keyflags)        {}
void Ctrl::RightDown(Point p, dword keyflags)       {}
void Ctrl::LeftRepeat(Point p, dword keyflags)      {}
void Ctrl::RightRepeat(Point p, dword keyflags)     {}
void Ctrl::MouseMove(Point p, dword keyflags)       {}
void Ctrl::LeftUp(Point, dword keyflags)            {}
void Ctrl::RightUp(Point p, dword keyflags)         {}
void Ctrl::MouseLeave()                             {}
void Ctrl::MouseWheel(Point p, int zd, dword kf)    {}
void Ctrl::LeftDrag(Point p, dword keyflags)        {}
void Ctrl::LeftHold(Point p, dword keyflags)        {}
void Ctrl::RightDrag(Point p, dword keyflags)       {}
void Ctrl::RightHold(Point p, dword keyflags)       {}
void Ctrl::MiddleDown(Point p, dword keyflags)      {}
void Ctrl::MiddleDouble(Point p, dword keyflags)    {}
void Ctrl::MiddleTriple(Point p, dword keyflags)    {}
void Ctrl::MiddleRepeat(Point p, dword keyflags)    {}
void Ctrl::MiddleDrag(Point p, dword keyflags)      {}
void Ctrl::MiddleHold(Point p, dword keyflags)      {}
void Ctrl::MiddleUp(Point p, dword keyflags)        {}

void Ctrl::Layout()                                 {}

void Ctrl::PostInput()
{
	if(parent) parent->PostInput();
}

void Ctrl::LeftDouble(Point p, dword keyflags)
{
	LeftDown(p, keyflags);
}

void Ctrl::LeftTriple(Point p, dword keyflags)
{
	LeftDown(p, keyflags);
}

void Ctrl::RightDouble(Point p, dword keyflags)
{
	RightDown(p, keyflags);
}

void Ctrl::RightTriple(Point p, dword keyflags)
{
	RightDown(p, keyflags);
}

void Ctrl::ChildGotFocus()
{
	if(parent) parent->ChildGotFocus();
}

void Ctrl::ChildLostFocus()
{
	if(parent) parent->ChildLostFocus();
}

void Ctrl::ChildAdded(Ctrl *q)
{
	if(parent) parent->ChildAdded(q);
}

void Ctrl::ChildRemoved(Ctrl *q)
{
	if(parent) parent->ChildRemoved(q);
}

void Ctrl::ParentChange() {}

bool Ctrl::Key(dword key, int count)
{
	return false;
}

void Ctrl::GotFocus()                               {}
void Ctrl::LostFocus()                              {}

dword  Ctrl::AccessKeyBit(byte accesskey)
{
	if(accesskey >= 'A' && accesskey <= 'Z')
		return (uint64)2 << (accesskey - 'A');
	return !!accesskey;
}

dword Ctrl::GetAccessKeysDeep() const
{
	dword used = GetAccessKeys();
	for(Ctrl *ctrl = GetFirstChild(); ctrl; ctrl = ctrl->GetNext())
		used |= ctrl->GetAccessKeysDeep();
	return used;
}

void Ctrl::AssignAccessKeys(dword used)
{
	for(Ctrl *ctrl = GetFirstChild(); ctrl; ctrl = ctrl->GetNext()) {
		ctrl->AssignAccessKeys(used);
		used |= ctrl->GetAccessKeys();
	}
}

dword Ctrl::GetAccessKeys() const
{
	return 0;
}

void Ctrl::DistributeAccessKeys()
{
	AssignAccessKeys(GetAccessKeysDeep());
}

bool Ctrl::VisibleAccessKeys()
{
	if(GUI_AltAccessKeys())
		return GetAlt() && GetTopCtrl() == GetActiveCtrl();
	return true;
}

void Ctrl::State(int) {}

void Ctrl::StateDeep(int reason)
{
	if(destroying)
		return;
	State(reason);
	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext())
		q->StateDeep(reason);
}

void Ctrl::StateH(int reason)
{
	for(int i = 0; i < statehook().GetCount(); i++)
		if((*statehook()[i])(this, reason))
			return;
	if(reason == POSITION)
		State(reason);
	else
		StateDeep(reason);
}

bool   Ctrl::Accept()
{
	if(!IsEnabled() || !IsShown())
		return true;
	if(DisplayError(GetData())) {
		SetWantFocus();
		return false;
	}
	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext())
		if(!q->Accept()) return false;
	return true;
}

void   Ctrl::Reject()
{
	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext())
		q->Reject();
}

void   Ctrl::Serialize(Stream& s)
{
	Value x;
	if(s.IsStoring())
		x = GetData();
	s % x;
	if(s.IsLoading())
		SetData(x);
	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext())
		q->Serialize(s);
}

void Ctrl::Updated() {}

bool Ctrl::IsForeground() const
{
	return GetTopCtrl()->IsWndForeground();
}

void Ctrl::SetForeground()
{
	GetTopCtrl()->SetWndForeground();
}

bool Ctrl::IsOpen() const
{
	const Ctrl *q = GetTopCtrl();
	return q->IsWndOpen() && q->isopen;
}

void Ctrl::Show(bool ashow) {
	if(visible != ashow) {
		LLOG("Show " << Name() << " " << ashow);
		visible = true;
		fullrefresh = false;
		RefreshFrame();
		visible = ashow;
		fullrefresh = false;
		RefreshFrame();
		if(parent)
			StateH(SHOW);
// 01/12/2007 - mdelfede
// added support for windowed controls
//		else
		if(top)
// 01/12/2007 - END
			WndShow(visible);
		if(InFrame() && parent)
			RefreshParentLayout();
	}
}

bool Ctrl::IsVisible() const {
	const Ctrl *q = this;
	for(;;) {
		if(!q->visible) return false;
		if(!q->parent) break;
		q = q->parent;
	}
	return q->visible;
}

void Ctrl::Enable(bool aenable) {
	if(enabled != aenable) {
		enabled = aenable;
// 01/12/2007 - mdelfede
// added support for windowed controls
//		if(!parent) WndEnable(enabled);
		if(top) WndEnable(enabled);
// 01/12/2007 - END
		if(!enabled && parent && HasFocusDeep())
			IterateFocusForward(this, GetTopCtrl());
		RefreshFrame();
		StateH(ENABLE);
		SyncCaret();
	}
}

bool Ctrl::IsShowEnabled() const {
	return IsEnabled() && (!parent || parent->IsShowEnabled());
}

Ctrl& Ctrl::SetEditable(bool aeditable) {
	if(editable != aeditable) {
		editable = aeditable;
		RefreshFrame();
		StateH(EDITABLE);
	}
	return *this;
}

void Ctrl::SetModify()
{
	modify = true;
}

void Ctrl::ClearModify()
{
	modify = false;
	for(Ctrl *q = firstchild; q; q = q->next)
		q->ClearModify();
}

bool Ctrl::IsModified() const
{
	if(IsModifySet()) return true;
	for(Ctrl *q = firstchild; q; q = q->next)
		if(q->IsModified()) return true;
	return false;
}

void Ctrl::SetCaret(int x, int y, int cx, int cy)
{
	caretx = x;
	carety = y;
	caretcx = cx;
	caretcy = cy;
}

void Ctrl::SetCaret(const Rect& r)
{
	caretx = r.left;
	carety = r.top;
	caretcx = r.GetWidth();
	caretcy = r.GetHeight();
}

Rect Ctrl::GetCaret() const
{
	return RectC(caretx, carety, caretcx, caretcy);
}

void Ctrl::KillCaret()
{
	SetCaret(0, 0, 0, 0);
}

void Ctrl::SetInfoPart(int i, const char *txt)
{
	Vector<String> f = Split(info, '\x7f', false);
	f.At(i) = txt;
	info = Join(f, "\x7f");
}

Ctrl& Ctrl::Tip(const char *txt)
{
	SetInfoPart(0, txt);
	return *this;
}

Ctrl& Ctrl::HelpLine(const char *txt)
{
	SetInfoPart(1, txt);
	return *this;
}

Ctrl& Ctrl::Description(const char *txt)
{
	SetInfoPart(2, txt);
	return *this;
}

Ctrl& Ctrl::HelpTopic(const char *txt)
{
	SetInfoPart(3, txt);
	return *this;
}

Ctrl& Ctrl::LayoutId(const char *txt)
{
	SetInfoPart(4, txt);
	return *this;
}

String Ctrl::GetInfoPart(int i) const
{
	Vector<String> f = Split(info, '\x7f', false);
	return i < f.GetCount() ? f[i] : String();
}

String Ctrl::GetTip() const
{
	return GetInfoPart(0);;
}

String Ctrl::GetHelpLine() const
{
	return GetInfoPart(1);
}

String Ctrl::GetDescription() const
{
	return GetInfoPart(2);
}

String Ctrl::GetHelpTopic() const
{
	return GetInfoPart(3);
}

String Ctrl::GetLayoutId() const
{
	return GetInfoPart(4);
}

bool  Ctrl::SetWantFocus() {
	if(IsWantFocus() && IsEnabled() && IsVisible() && IsOpen())
		return SetFocus();
	return false;
}

void Ctrl::UpdateRefresh() {
	Update();
	Refresh();
}

void Ctrl::Update() {
	SetModify();
	Updated();
}

void Ctrl::Action() {
	WhenAction();
}

void Ctrl::UpdateAction() {
	Update();
	Action();
}

void Ctrl::UpdateActionRefresh() {
	Update();
	Action();
	Refresh();
};

void  Ctrl::CancelModeDeep() {
	CancelMode();
	for(Ctrl *q = firstchild; q; q = q->next)
		q->CancelModeDeep();
}

String Ctrl::Name() const {
#ifdef CPU_64
	String s = String(typeid(*this).name()) + " : 0x" + FormatIntHex(this);
#else
	String s = String(typeid(*this).name()) + " : " + Format("0x%x", (int) this);
#endif
	if(IsChild())
		s << "(parent " << String(typeid(*parent).name()) << ")";
	else
#ifdef PLATFORM_WIN32
		s << Format("(hwnd 0x%x)", (int)(intptr_t) GetHWND());
#endif
#ifdef PLATFORM_X11
		s << Format("(window 0x%x)", (int)(intptr_t) GetWindow());
#endif
	return s;
}

String Ctrl::GetDesc() const
{
	return "";
}


String Name(const Ctrl *ctrl)
{
	return ctrl ? ctrl->Name() : "NULL";
}

String Desc(const Ctrl *ctrl)
{
	if(!ctrl)
		return "NULL";
	String s;
	s << typeid(*ctrl).name();
	String q = ctrl->GetDesc();
	if(IsNull(q)) {
		if(ctrl->GetPrev()) {
			q = ctrl->GetPrev()->GetDesc();
			if(!IsNull(q))
			  s << " <<\"" << q << "\">>";
		}
	}
	else
	  s << " \"" << q << '\"';
	const Ctrl *top = ctrl->GetTopWindow();
 	if(top && top != ctrl) {
 		String q = top->GetDesc();
 		if(IsNull(q))
 			s << " (" << typeid(*top).name() << ")";
 		else
	 		s << " (\"" << q << "\")";
 	}
 	return s;
}

#ifdef _DEBUG

#define sFLAG(x)  (x ? #x" " : "")
#define LG(x)     s << x << '\n'

void Ctrl::Dump(Stream& s) const {
	LG(Name());
	LG(sFLAG(backpaint) << sFLAG(inframe) << sFLAG(visible) << sFLAG(enabled) <<
	   sFLAG(wantfocus) << sFLAG(editable) << sFLAG(IsModified()) << sFLAG(transparent));
	LG("Rect:   " << GetRect());
	LG("View:   " << GetView());
	for(int i = 0; i < frame.GetCount(); i++)
		LG("Frame " << i << ": " << typeid(*frame[i].frame).name() << " - " << frame[i].view);
	LG("Data: " << GetData().ToString());
	if(firstchild) {
		LG("Children");
		s << LOG_BEGIN;
		for(Ctrl *q = GetFirstChild(); q; q = q->GetNext()) {
			q->Dump(s);
			LG("------");
		}
		s << LOG_END;
	}
	else
		LG("No child");
}

void Ctrl::Dump() const {
	Dump(VppLog());
}

void   Dump(const Ctrl *ctrl)
{
	if(ctrl)
		ctrl->Dump();
	else
		LOG("NULL");
}

#endif

bool Ctrl::IsOcxChild()
{
	return false;
}

Ctrl::Ctrl() {
	LLOG("Ctrl::Ctrl");
	destroying = false;
	parent = prev = next = firstchild = lastchild = NULL;
	top = NULL;
	exitcode = 0;
	frame.Add().frame = &NullFrame();
	enabled = visible = wantfocus = initfocus = true;
	editable = true;
//	GLX = false;
#ifdef PLATFORM_WIN32
	activex = false;
	isdhctrl = false;
#endif
	backpaint = TRANSPARENTBACKPAINT;
	inframe = false;
	ignoremouse = transparent = false;
	caretcx = caretcy = caretx = carety = 0;
	SetRect(Rect(0, 0, 0, 0));
	inloop = popup = isopen = false;
	modify = false;
	unicode = false;
	popupgrab = false;
	fullrefresh = false;
	akv = false;
}

void KillTimeCallbacks(void *id, void *idlim);

void Ctrl::DoRemove() {
	if(!IsOpen()) return;
	ReleaseCapture();
	CancelModeDeep();
	if(HasChildDeep(mouseCtrl) || mouseCtrl == this)
		mouseCtrl = NULL;
	LLOG("DoRemove " << Name() << " focusCtrl: " << UPP::Name(focusCtrl) << BeginIndent);
#ifdef PLATFORM_X11
	if(popupgrab) {
		EndPopupGrab();
		popupgrab = false;
	}
#endif
	if(HasFocusDeep()) {
		LLOG("DoRemove - HasFocusDeep");
		if(destroying) {
			if(parent) {
				LLOG("parent - deferred SetFocus / ChildLostFocus; parent = " << UPP::Name(parent));
				defferedSetFocus = parent;
				defferedChildLostFocus.Add(parent);
			}
			else
				if(IsPopUp()) {
					LLOG("Remove Popup");
					Ctrl *owner = GetOwner();
					if(owner && owner->IsEnabled())
						owner->ActivateWnd();
				}
			NoWantFocus();
		}
		else {
			Ptr<Ctrl> fc = focusCtrl;
			focusCtrl = NULL;
			DoKillFocus(fc, NULL);
			if(parent) {
				LLOG("DoRemove -> SetFocus(" << UPP::Name(parent) << "), focusCtrl = " << UPP::Name(focusCtrl) << ", fc = " << UPP::Name(fc));
				bool b = IsWantFocus();
				NoWantFocus();
				parent->SetFocus0(false);
				WantFocus(b);
			}
			else
				if(IsPopUp()) {
					LLOG("Remove Popup");
					Ctrl *owner = GetOwner();
					if(owner && owner->IsEnabled()) {
						LLOG("Remove popup -> owner->ActivateWnd");
						owner->ActivateWnd();
					}
				}
		}
		SyncCaret();
	}
	LLOG(EndIndent << "//DoRemove " << Name() << " focusCtrl: " << UPP::Name(focusCtrl));
}

void Ctrl::Close()
{
	Ctrl *q = GetTopCtrl();
	if(!q->top) return;
	DoRemove();
	if(parent) return;
	StateH(CLOSE);
	bool vis = visible;
	UsrLogT(3, "CLOSE " + Desc(this));
	WndDestroy();
	visible = vis;
	popup = false;
}

Ctrl::~Ctrl() {
	LLOG("Ctrl::~Ctrl");
	destroying = true;
	while(GetFirstChild())
		RemoveChild(GetFirstChild());
	if(parent)
		parent->RemoveChild(this);
	Close();
	KillTimeCallbacks(this, (byte *) this + sizeof(Ctrl));
}

GLOBAL_VAR(Vector<Ctrl::MouseHook>, Ctrl::mousehook);
GLOBAL_VAR(Vector<Ctrl::KeyHook>,   Ctrl::keyhook);
GLOBAL_VAR(Vector<Ctrl::StateHook>, Ctrl::statehook);

void Ctrl::InstallMouseHook(MouseHook hook)
{
	mousehook().Add(hook);
}

void Ctrl::DeinstallMouseHook(MouseHook hook)
{
	int q = FindIndex(mousehook(), hook);
	if(q >= 0) mousehook().Remove(q);
}

void Ctrl::InstallKeyHook(KeyHook hook)
{
	keyhook().Add(hook);
}

void Ctrl::DeinstallKeyHook(KeyHook hook)
{
	int q = FindIndex(keyhook(), hook);
	if(q >= 0) keyhook().Remove(q);
}

void Ctrl::InstallStateHook(StateHook hook)
{
	statehook().Add(hook);
}

void Ctrl::DeinstallStateHook(StateHook hook)
{
	int q = FindIndex(statehook(), hook);
	if(q >= 0) statehook().Remove(q);
}

static char sZoomText[] = "OK Cancel Exit Retry";

const char *Ctrl::GetZoomText()
{
	return sZoomText;
}

Size Ctrl::Dsize;
Size Ctrl::Csize;

inline void Ctrl::Csizeinit()
{
	if(Csize.cx == 0)
		Csize = GetTextSize(sZoomText, StdFont());
	if(Dsize.cx == 0)
		Dsize = Size(99, 13);
	Csize.cx = max(Csize.cx, Dsize.cx);
	Csize.cy = max(Csize.cy, Dsize.cy);
}

void Ctrl::SetZoomSize(Size sz, Size bsz)
{
	Csize = sz;
	Dsize = bsz;
}

void Ctrl::NoLayoutZoom()
{
	Csize = Dsize = Size(1, 1);
}

void Ctrl::GetZoomRatio(Size& m, Size& d)
{
	m = Csize;
	d = Dsize;
}

int  Ctrl::HorzLayoutZoom(int cx)
{
	Csizeinit();
	return Csize.cx * cx / Dsize.cx;
}

int  Ctrl::VertLayoutZoom(int cy)
{
	Csizeinit();
	return Csize.cy * cy / Dsize.cy;
}

Size Ctrl::LayoutZoom(int cx, int cy)
{
	Csizeinit();
	return Size(Csize.cx * cx / Dsize.cx, Csize.cy * cy / Dsize.cy);
}

Size Ctrl::LayoutZoom(Size sz)
{
	Csizeinit();
	return LayoutZoom(sz.cx, sz.cy);
}

Font FontZ(int face, int height)
{
	return Font(face, Ctrl::VertLayoutZoom(height));
}

Font StdFontZ(int height) { return FontZ(Font::STDFONT, height); }
Font ScreenSansZ(int height) { return FontZ(Font::SCREEN_SANS, height); }
Font ScreenSerifZ(int height) { return FontZ(Font::SCREEN_SERIF, height); }
Font ScreenFixedZ(int height) { return FontZ(Font::SCREEN_FIXED, height); }
Font RomanZ(int height) { return FontZ(Font::ROMAN, height); }
Font ArialZ(int height) { return FontZ(Font::ARIAL, height); }
Font CourierZ(int height) { return FontZ(Font::COURIER, height); }

String Ctrl::appname;

void Ctrl::SetAppName(const String& nm)
{
	appname = nm;
}

String Ctrl::GetAppName()
{
	if(appname.IsEmpty())
		appname = GetExeTitle();
	return appname;
}

static bool _ClickFocus;
bool Ctrl::ClickFocus() { return _ClickFocus; }
void Ctrl::ClickFocus(bool cf) { _ClickFocus = cf; }


Vector<Ctrl *> Ctrl::GetTopWindows()
{
	Vector<Ctrl *> c = GetTopCtrls();
	Vector<Ctrl *> r;
	for(int i = 0; i < c.GetCount(); i++)
		if(!c[i]->IsPopUp())
			r.Add(c[i]);
	return r;
}

void Ctrl::CloseTopCtrls()
{
	Vector<Ctrl *> tc = Ctrl::GetTopCtrls();
	for(int i = 0; i < tc.GetCount(); i++)
		tc[i]->Close();
}

bool xpstyle;

bool IsOrOwnedBy(Ctrl *q, Ctrl *window)
{
	while(q) {
		if(q == window)
			return true;
		q = q->GetOwner();
	}
	return false;
}

Vector< Ptr<Ctrl> > DisableCtrls(const Vector<Ctrl *>& ctrl, Ctrl *exclude)
{
	Vector< Ptr<Ctrl> > disabled;
	for(int i = 0; i < ctrl.GetCount(); i++) {
		Ctrl *q = ctrl[i];
		if(q && q->IsEnabled() && !IsOrOwnedBy(q, exclude)) {
			q->Disable();
			disabled.Add(q);
		}
	}
	return disabled;
}

void EnableCtrls(const Vector< Ptr<Ctrl> >& ctrl)
{
	for(int i = ctrl.GetCount(); --i >= 0;) {
		Ctrl *q = ctrl[i];
		if(q) q->Enable();
	}
}

void Modality::Begin(Ctrl *modal, bool fo)
{
	active = Ctrl::GetFocusCtrl();
	fore_only = fo;
	LLOG("Active " << Name(active));
	enable = DisableCtrls(Ctrl::GetTopWindows(), modal);
}

void Modality::End()
{
	EnableCtrls(enable);
	if(active && (!fore_only || active->IsForeground()))
		active->SetFocus();
	enable.Clear();
	active = NULL;
}

void (*s_chdefault)();

void (*Ctrl::skin)();

void CtrlSetDefaultSkin(void (*fn1)(), void (*fn2)())
{
	s_chdefault = fn1;
	Ctrl::skin = fn2;
}

void Ctrl::SetSkin(void (*_skin)())
{
	skin = _skin;
	ChSync();
	Vector<Ctrl *> ctrl = GetTopCtrls();
	for(int i = 0; i < ctrl.GetCount(); i++) {
		ctrl[i]->RefreshLayoutDeep();
		ctrl[i]->RefreshFrame();
	}
}

void Ctrl::ChSync()
{
	if(s_chdefault)
		(*s_chdefault)();
	if(skin)
		(*skin)();
	Csize.cx = Dsize.cx = 0;
	ChFinish();
}

CH_INT(GUI_GlobalStyle, GUISTYLE_CLASSIC);
CH_INT(GUI_DragFullWindow, 1);
CH_INT(GUI_PopUpEffect, GUIEFFECT_SLIDE);
CH_INT(GUI_DropShadows, 1);
CH_INT(GUI_AltAccessKeys, 1);
CH_INT(GUI_AKD_Conservative, 0);
CH_INT(GUI_DragDistance, 4);
CH_INT(GUI_DblClickTime, 500);

END_UPP_NAMESPACE
