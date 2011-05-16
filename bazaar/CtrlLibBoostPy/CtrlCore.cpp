#include "CtrlCore.h"
using namespace boost::python;

NAMESPACE_UPP

void export_Logc()
{
ONCELOCK
{
	enum_<Ctrl::PlacementConstants>("placementcontstants")
		.value("center", Ctrl::CENTER)
		.value("middle", Ctrl::MIDDLE)
		.value("left", Ctrl::LEFT)
		.value("right", Ctrl::RIGHT)
		.value("top", Ctrl::TOP)
		.value("bottom", Ctrl::BOTTOM)
		.value("size", Ctrl::SIZE)
		.value("minsize", Ctrl::MINSIZE)
		.value("maxsize", Ctrl::MAXSIZE)
		.value("stdsize", Ctrl::STDSIZE)
	;

	class_<Ctrl::Logc>("Logc", "Upp Ctrl::Logc")
		.def(init<int, int, int>())
		.add_property("align", &Ctrl::Logc::GetAlign, &Ctrl::Logc::SetAlign)
		.add_property("a", &Ctrl::Logc::GetA, &Ctrl::Logc::SetA)
		.add_property("b", &Ctrl::Logc::GetB, &Ctrl::Logc::SetB)
		.def("isempty", &Ctrl::Logc::IsEmpty)
//		.def("__str__", &AsString<Ctrl::LogC>)
	;
}
}

void export_LogPos()
{
ONCELOCK
{
	export_Logc();
	class_<Ctrl::LogPos>("LogPos", "Upp Ctrl::LogPos")
		.def(init<Ctrl::Logc, Ctrl::Logc>())
		.def_readwrite("x", &Ctrl::LogPos::x)
		.def_readwrite("y", &Ctrl::LogPos::y)
//		.def("__str__", &AsString<Ctrl::LogPos>) //FIXME LogPosCtrl
	;
}
}

//virtuals
Value CtrlGetData(const Ctrl& c) { return c.GetData(); }
void CtrlSetData(Ctrl& c, const Value& data) { c.SetData(data); }

//return ignorers
void CtrlLeftPos(Ctrl& c, int a, int size) { c.LeftPos(a, size); }
void CtrlRightPos(Ctrl& c, int a, int size) { c.RightPos(a, size); }
void CtrlTopPos(Ctrl& c, int a, int size) { c.TopPos(a, size); }
void CtrlBottomPos(Ctrl& c, int a, int size) { c.BottomPos(a, size); }
void CtrlHSizePos(Ctrl& c, int a, int b) { c.HSizePos(a, b); }
void CtrlVSizePos(Ctrl& c, int a, int b) { c.VSizePos(a, b); }
void CtrlHCenterPos(Ctrl& c, int size, int delta) { c.HCenterPos(size, delta); }
void CtrlVCenterPos(Ctrl& c, int size, int delta) { c.HCenterPos(size, delta); }
void CtrlSizePos(Ctrl& c) { c.SizePos(); }

void CtrlLeftPosZ(Ctrl& c, int a, int size) { c.LeftPosZ(a, size); }
void CtrlRightPosZ(Ctrl& c, int a, int size) { c.RightPosZ(a, size); }
void CtrlTopPosZ(Ctrl& c, int a, int size) { c.TopPosZ(a, size); }
void CtrlBottomPosZ(Ctrl& c, int a, int size) { c.BottomPosZ(a, size); }
void CtrlHSizePosZ(Ctrl& c, int a, int b) { c.HSizePosZ(a, b); }
void CtrlVSizePosZ(Ctrl& c, int a, int b) { c.VSizePosZ(a, b); }
void CtrlHCenterPosZ(Ctrl& c, int size, int delta) { c.HCenterPosZ(size, delta); }
void CtrlVCenterPosZ(Ctrl& c, int size, int delta) { c.HCenterPosZ(size, delta); }

void CtrlIgnoreMouse(Ctrl& c, bool b) { c.IgnoreMouse(b); }
void CtrlWantFocus(Ctrl& c, bool b) { c.WantFocus(b); }
void CtrlInitFocus(Ctrl& c, bool b) { c.InitFocus(b); }
void CtrlSetEditable(Ctrl& c, bool b) { c.SetEditable(b); }
void CtrlSetPos(Ctrl& c, const Ctrl::LogPos& p) { c.SetPos(p); }
void CtrlSetTip(Ctrl& c, const String& s) { c.Tip(s); }
void CtrlTransparent(Ctrl& c, bool b) { c.Transparent(b); }

void CtrlSetInfo(Ctrl& c, const String& s) { c.Info(s); }

//other
void CtrlSetCapture(Ctrl& c, bool b) { if(b) c.SetCapture(); else c.ReleaseCapture(); }
void CtrlSetFocus(Ctrl& c, bool b) { if(b) c.SetFocus(); else if(c.GetParent()) c.GetParent()->SetFocus(); }
void CtrlSetBackPaint(Ctrl& c, bool b) { c.BackPaint(b?(Ctrl::FULLBACKPAINT):(Ctrl::NOBACKPAINT)); }
bool CtrlIsBackPaint(const Ctrl& c) { return (c.GetBackPaint()!=Ctrl::NOBACKPAINT); }

String CtrlToString(const Ctrl& c)
{
	return c.Name();
	return String().Cat() << String(typeid(c).name()) << ": " << AsString(c.GetData());
}

enum PaintEnum {
	_NOBACKPAINT = Ctrl::NOBACKPAINT,
	_FULLBACKPAINT = Ctrl::FULLBACKPAINT,
	_TRANSPARENTBACKPAINT = Ctrl::TRANSPARENTBACKPAINT,
	_EXCLUDEPAINT = Ctrl::EXCLUDEPAINT,
};

class CtrlWrap : public Ctrl, public wrapper<Ctrl> {
public:
	typedef Ctrl C;

	virtual bool Accept() { if(override f = this->get_override("accept")) return f(); return C::Accept(); }
	bool default_Accept() { return C::Accept(); }

	virtual void Reject() { if(override f = this->get_override("reject")) f(); C::Reject(); }
	void default_Reject() { C::Reject(); }

	virtual void SetData(const Value& data) { if(override f = this->get_override("setdata")) f(data); C::SetData(data); }
	void default_SetData(const Value& data) { C::SetData(data); }

	virtual Value GetData() const { if(override f = this->get_override("getdata")) return f(); return C::GetData(); }
	Value default_GetData() const { return C::GetData(); }

	virtual void SetModify() { if(override f = this->get_override("setmodify")) f(); C::SetModify(); }
	void default_SetModify() { C::SetModify(); }

	virtual void ClearModify() { if(override f = this->get_override("clearmodify")) f(); C::ClearModify(); }
	void default_ClearModify() { C::ClearModify(); }

	virtual bool IsModified() const { if(override f = this->get_override("ismodified")) return f(); return C::IsModified(); }
	bool default_IsModified() const { return C::IsModified(); }

	virtual void CancelMode() { if(override f = this->get_override("cancelmode")) f(); C::CancelMode(); }
	void default_CancelMode() { C::CancelMode(); }

	virtual void Activate() { if(override f = this->get_override("activate")) f(); C::Activate(); }
	void default_Activate() { C::Activate(); }

	virtual void Deactivate() { if(override f = this->get_override("deactivate")) f(); C::Deactivate(); }
	void default_Deactivate() { C::Deactivate(); }

	virtual void LeftDown(Point p, dword keyflags) { if(override f = this->get_override("leftdown")) f(p, keyflags); C::LeftDown(p, keyflags); }
	void default_LeftDown(Point p, dword keyflags) { C::LeftDown(p, keyflags); }

	//more of interface to come
	
	virtual bool Key(dword key, int count) { if(override f = this->get_override("key")) return f(key, count); return C::Key(key, count); }
	bool default_Key(dword key, int count) { return C::Key(key, count); }

	virtual void GotFocus() { if(override f = this->get_override("gotfocus")) f(); C::GotFocus(); }
	void default_GotFocus() { C::GotFocus(); }

	virtual void LostFocus() { if(override f = this->get_override("lostfocus")) f(); C::LostFocus(); }
	void default_LostFocus() { C::LostFocus(); }

	virtual void ChildGotFocus() { if(override f = this->get_override("childgotfocus")) f(); C::ChildGotFocus(); }
	void default_ChildGotFocus() { C::ChildGotFocus(); }

	virtual void ChildLostFocus() { if(override f = this->get_override("childlostfocus")) f(); C::ChildLostFocus(); }
	void default_ChildLostFocus() { C::ChildLostFocus(); }

	virtual void ChildAdded(Ctrl *child) { if(override f = this->get_override("childadded")) f(child); C::ChildAdded(child); }
	void default_ChildAdded(Ctrl *child) { C::ChildAdded(child); }

	virtual void ChildRemoved(Ctrl *child) { if(override f = this->get_override("childremoved")) f(child); C::ChildRemoved(child); }
	void default_ChildRemoved(Ctrl *child) { C::ChildRemoved(child); }

	virtual void State(int reason) { if(override f = this->get_override("state")) f(reason); C::State(reason); }
	void default_State(int reason) { C::State(reason); }

	virtual void Layout() { if(override f = this->get_override("layout")) f(); C::Layout(); }
	void default_Layout() { C::Layout(); }

	virtual Size GetMinSize() const { if(override f = this->get_override("getminsize")) return f(); return C::GetMinSize(); }
	Size default_GetMinSize() const { return C::GetMinSize(); }

	virtual Size GetStdSize() const { if(override f = this->get_override("getstdsize")) return f(); return C::GetStdSize(); }
	Size default_GetStdSize() const { return C::GetStdSize(); }

	virtual Size GetMaxSize() const { if(override f = this->get_override("getmaxsize")) return f(); return C::GetMaxSize(); }
	Size default_GetMaxSize() const { return C::GetMaxSize(); }

	virtual bool IsShowEnabled() const { if(override f = this->get_override("isshowenabled")) return f(); return C::IsShowEnabled(); }
	bool default_IsShowEnabled() const { return C::IsShowEnabled(); }

	virtual Rect GetOpaqueRect() const { if(override f = this->get_override("getopaquerect")) return f(); return C::GetOpaqueRect(); }
	Rect default_GetOpaqueRect() const { return C::GetOpaqueRect(); }

	virtual Rect GetVoidRect() const { if(override f = this->get_override("getvoidrect")) return f(); return C::GetVoidRect(); }
	Rect default_GetVoidRect() const { return C::GetVoidRect(); }

	virtual void Updated() { if(override f = this->get_override("updated")) f(); C::Updated(); }
	void default_Updated() { C::Updated(); }

	virtual void Close() { if(override f = this->get_override("close")) f(); C::Close(); }
	void default_Close() { C::Close(); }

	virtual String GetDesc() const { if(override f = this->get_override("getdesc")) return f(); return C::GetDesc(); }
	String default_GetDesc() const { return C::GetDesc(); }

#ifdef _DEBUG
	virtual void Dump() const { if(override f = this->get_override("dump")) f(); C::Dump(); }
	void default_Dump() const { C::Dump(); }
#endif

};

void export_CtrlCore()
{
ONCELOCK
{
	export_Core();
	export_LogPos();

	enum_<Ctrl::StateReason>("statereason")
		.value("focus", Ctrl::FOCUS)
		.value("activate", Ctrl::ACTIVATE)
		.value("deactivate", Ctrl::DEACTIVATE)
		.value("show", Ctrl::SHOW)
		.value("enable", Ctrl::ENABLE)
		.value("editable", Ctrl::EDITABLE)
		.value("open", Ctrl::OPEN)
		.value("close", Ctrl::CLOSE)
		.value("position", Ctrl::POSITION)
		.value("layoutpos", Ctrl::LAYOUTPOS)
	;

	enum_<Ctrl::MouseEvents>("mouseevents")
		.value("button", Ctrl::BUTTON)
		.value("action", Ctrl::ACTION)

		.value("mouseenter", Ctrl::MOUSEENTER)
		.value("mousemove", Ctrl::MOUSEMOVE)
		.value("mouseleave", Ctrl::MOUSELEAVE)
		.value("cursorimage", Ctrl::CURSORIMAGE)
		.value("mousewheel", Ctrl::MOUSEWHEEL)

		.value("down", Ctrl::DOWN)
		.value("up", Ctrl::UP)
		.value("double", Ctrl::DOUBLE)
		.value("repeat", Ctrl::REPEAT)
		.value("drag", Ctrl::DRAG)
		.value("hold", Ctrl::HOLD)
		.value("tripple", Ctrl::TRIPLE)

		.value("leftbutton", Ctrl::LEFTDOWN)
		.value("leftdouble", Ctrl::LEFTDOUBLE)
		.value("leftrepeat", Ctrl::LEFTREPEAT)
		.value("leftup", Ctrl::LEFTUP)
		.value("leftdrag", Ctrl::LEFTDRAG)
		.value("lefthold", Ctrl::LEFTHOLD)
		.value("lefttripple", Ctrl::LEFTTRIPLE)

		.value("rightdown", Ctrl::RIGHTDOWN)
		.value("rightdouble", Ctrl::RIGHTDOUBLE)
		.value("rightrepeat", Ctrl::RIGHTREPEAT)
		.value("rightup", Ctrl::RIGHTUP)
		.value("rightdrag", Ctrl::RIGHTDRAG)
		.value("righthold", Ctrl::RIGHTHOLD)
		.value("righttripple", Ctrl::RIGHTTRIPLE)

		.value("middledown", Ctrl::MIDDLEDOWN)
		.value("middledouble", Ctrl::MIDDLEDOUBLE)
		.value("middlerepeat", Ctrl::MIDDLEREPEAT)
		.value("middleup", Ctrl::MIDDLEUP)
		.value("middledrag", Ctrl::MIDDLEDRAG)
		.value("middlehold", Ctrl::MIDDLEHOLD)
		.value("middletripple", Ctrl::MIDDLETRIPLE)
	;

	enum_<PaintEnum>("paintenum")
		.value("nobackpaint", _NOBACKPAINT)
		.value("fullbackpaint", _FULLBACKPAINT)
		.value("transparentbackpaint", _TRANSPARENTBACKPAINT)
		.value("excludepaint", _EXCLUDEPAINT)
	;

	class_<CtrlWrap, boost::noncopyable>("Ctrl", "Upp Ctrl")

		.def("posleft", &Ctrl::PosLeft)
		.staticmethod("posleft")
		.def("posright", &Ctrl::PosRight)
		.staticmethod("posright")
		.def("postop", &Ctrl::PosTop)
		.staticmethod("postop")
		.def("posbottom", &Ctrl::PosBottom)
		.staticmethod("posbottom")
		.def("possize", &Ctrl::PosSize)
		.staticmethod("possize")
		.def("poscenter", (Ctrl::Logc (*)(int, int))&Ctrl::PosCenter)
		.staticmethod("poscenter")

		.def("accept", &Ctrl::Accept, &CtrlWrap::default_Accept)
		.def("reject", &Ctrl::Reject, &CtrlWrap::default_Reject)

		.def("setdata", &Ctrl::SetData, &CtrlWrap::default_SetData)
		.def("getdata", &Ctrl::GetData, &CtrlWrap::default_GetData)
		.add_property("data", &CtrlGetData, &CtrlSetData) //FIXME is not how it should work, but for testing the C++ side interface

		.def("setmodify", &Ctrl::SetModify, &CtrlWrap::default_SetModify)
		.def("clearmodify", &Ctrl::ClearModify, &CtrlWrap::default_ClearModify)
		.def("ismodified", &Ctrl::IsModified, &CtrlWrap::default_IsModified)
		.def("resetmodify", &Ctrl::ResetModify)
		.def("ismodifiedset", &Ctrl::IsModifySet)

		.def("cancelmode", &Ctrl::CancelMode, &CtrlWrap::default_CancelMode)
		.def("activate", &Ctrl::Activate, &CtrlWrap::default_Activate)
		.def("deactivate", &Ctrl::Deactivate, &CtrlWrap::default_Deactivate)

		.def("leftdown", &Ctrl::LeftDown, &CtrlWrap::default_LeftDown)
		//...
		.def("key", &Ctrl::Key, &CtrlWrap::default_Key)

		.def("gotfocus", &Ctrl::GotFocus, &CtrlWrap::default_GotFocus)
		.def("lostfocus", &Ctrl::LostFocus, &CtrlWrap::default_LostFocus)

		.def("childgotfocus", &Ctrl::ChildGotFocus, &CtrlWrap::default_ChildGotFocus)
		.def("childlostfocus", &Ctrl::ChildLostFocus, &CtrlWrap::default_ChildLostFocus)
		.def("childadded", &Ctrl::ChildAdded, &CtrlWrap::default_ChildAdded)
		.def("childremoved", &Ctrl::ChildRemoved, &CtrlWrap::default_ChildRemoved)

		.def("state", &Ctrl::State, &CtrlWrap::default_State)
		.def("layout", &Ctrl::Layout, &CtrlWrap::default_Layout)

		.def("getminsize", &Ctrl::GetMinSize, &CtrlWrap::default_GetMinSize)
		.def("getstdsize", &Ctrl::GetStdSize, &CtrlWrap::default_GetStdSize)
		.def("getminsize", &Ctrl::GetMaxSize, &CtrlWrap::default_GetMaxSize)

		.def("isshowenabled", &Ctrl::IsShowEnabled, &CtrlWrap::default_IsShowEnabled)

		.def("getopaquerect", &Ctrl::GetOpaqueRect, &CtrlWrap::default_GetOpaqueRect)
		.def("getvoidrect", &Ctrl::GetVoidRect, &CtrlWrap::default_GetVoidRect)

		.def("updated", &Ctrl::Updated, &CtrlWrap::default_Updated)
		.def("close", &Ctrl::Close, &CtrlWrap::default_Close)

		.def("getdesc", &Ctrl::GetDesc, &CtrlWrap::default_GetDesc)

		.def_readwrite("whenaction", &Ctrl::WhenAction)

		.def("addchild", (void (Ctrl::*)(Ctrl*))&Ctrl::AddChild)
		.def("addchildafter", (void (Ctrl::*)(Ctrl*, Ctrl*))&Ctrl::AddChild)
		.def("addchildbefore", &Ctrl::AddChildBefore)
		.def("removechild", &Ctrl::RemoveChild)

/// dangerous, since it doesnt care where the objects are owned..this info gets lost in python then.
/// if the object is detroyd the object created from these calls in python wont know, and crash
		.def("getparent", &Ctrl::GetParent, return_value_policy<reference_existing_object>())
		.def("getfirstchild", &Ctrl::GetFirstChild, return_value_policy<reference_existing_object>())
		.def("getlastchild", &Ctrl::GetLastChild, return_value_policy<reference_existing_object>())
		.def("getnext", &Ctrl::GetNext, return_value_policy<reference_existing_object>())
		.def("getprev", &Ctrl::GetPrev, return_value_policy<reference_existing_object>())
///
		.def("ischild", &Ctrl::IsChild)

		.add_property("foreground", &Ctrl::IsForeground, &Ctrl::SetForeground)

		.def("childfrompoint", &Ctrl::ChildFromPoint, return_value_policy<reference_existing_object>())

		.add_property("isopen", &Ctrl::IsOpen)

		.def("shutdown", &Ctrl::Shutdown)
		.def("isshutdown", &Ctrl::IsShutdown)

		.add_property("pos", &Ctrl::GetPos, &CtrlSetPos)
		.add_property("rect", &Ctrl::GetRect, (void (Ctrl::*)(const Rect&))&Ctrl::SetRect)
		.add_property("size", &Ctrl::GetSize)

		.add_property("inview", &Ctrl::InView)
		.add_property("inframe", &Ctrl::InFrame)

		.def("refreshlayout", &Ctrl::RefreshLayout)
		.def("refreshlayoutdeep", &Ctrl::RefreshLayoutDeep)
		.def("refreshparentlayout", &Ctrl::RefreshParentLayout)
		.def("updatelayout", &Ctrl::UpdateLayout)
		.def("updateparentlayout", &Ctrl::UpdateParentLayout)

		.def("leftpos", &CtrlLeftPos)
		.def("rightpos", &CtrlRightPos)
		.def("toppos", &CtrlTopPos)
		.def("bottompos", &CtrlBottomPos)
		.def("hsizepos", &CtrlHSizePos)
		.def("vsizepos", &CtrlVSizePos)
		.def("hcenterpos", &CtrlHCenterPos)
		.def("vcenterpos", &CtrlVCenterPos)
		.def("sizepos", &CtrlSizePos)

		.def("leftposz", &CtrlLeftPosZ)
		.def("rightposz", &CtrlRightPosZ)
		.def("topposz", &CtrlTopPosZ)
		.def("bottomposz", &CtrlBottomPosZ)
		.def("hsizeposz", &CtrlHSizePosZ)
		.def("vsizeposz", &CtrlVSizePosZ)
		.def("hcenterposz", &CtrlHCenterPosZ)
		.def("vcenterposz", &CtrlVCenterPosZ)

		.def("getscreenrect", &Ctrl::GetScreenRect)
		.def("getview", &Ctrl::GetView)
		.def("getscreenview", &Ctrl::GetScreenView)
		.def("getvisiblescreenrect", &Ctrl::GetVisibleScreenRect)
		.def("getvisiblescreenview", &Ctrl::GetVisibleScreenView)
		.def("getworkarea", (Rect (Ctrl::*)() const)&Ctrl::GetWorkArea)

		.def("refresh", (void (Ctrl::*)())&Ctrl::Refresh)
		.def("isfullrefresh", &Ctrl::IsFullRefresh)

		.def("sync", (void (Ctrl::*)())&Ctrl::Sync)

		//.add_property("ignoremouse", &Ctrl::IgnoreMouse, &CtrlIgnoreMouse)
		.add_property("capture", &Ctrl::HasCapture, &CtrlSetCapture)
		.add_property("focus", &Ctrl::HasFocus, &CtrlSetFocus)
		.def("hasfocusdeep", &Ctrl::HasFocusDeep)
		.add_property("wantfocus", &Ctrl::IsWantFocus, &CtrlWantFocus)
		.add_property("initfocus", &Ctrl::IsInitFocus, &CtrlInitFocus)

		.def("cancelmodedeep", &Ctrl::CancelModeDeep)

		.add_property("show", &Ctrl::IsVisible, &Ctrl::Show)
		.add_property("shown", &Ctrl::IsShown)
		.add_property("enable", &Ctrl::IsEnabled, &Ctrl::Enable)
		.add_property("edit", &Ctrl::IsEditable, &CtrlSetEditable)

		.def("updaterefresh", &Ctrl::UpdateRefresh)
		.def("update", &Ctrl::Update)
		.def("action", &Ctrl::Action)
		.def("updateaction", &Ctrl::UpdateAction)
		.def("updateactionrefresh", &Ctrl::UpdateActionRefresh)

		.add_property("backpaint", &CtrlIsBackPaint, &CtrlSetBackPaint)
		.add_property("transparent", &Ctrl::IsTransparent, &CtrlTransparent)

		.add_property("info", &Ctrl::GetInfo, &CtrlSetInfo)
		.add_property("tip", &Ctrl::GetTip, &CtrlSetTip)

		.def("add", &Ctrl::Add)
		.def("remove", &Ctrl::Remove)

		.def("setalpha", &Ctrl::SetAlpha)
		.def("ispopup", &Ctrl::IsPopUp)

#ifdef _DEBUG
		.def("dump", (void (Ctrl::*)() const)&Ctrl::Dump, &CtrlWrap::default_Dump)
#endif
		.def("__str__", &CtrlToString)
	;

	def("showrepaint", &Ctrl::ShowRepaint, "Show Repaint process in Upp");
}
}

class TopWindowWrap : public TopWindow, public wrapper<TopWindow> {
public:
	typedef TopWindow C;

	virtual bool Accept() { if(override f = this->get_override("accept")) return f(); return C::Accept(); }
	bool default_Accept() { return C::Accept(); }

	virtual void Reject() { if(override f = this->get_override("reject")) f(); C::Reject(); }
	void default_Reject() { C::Reject(); }

	virtual void Activate() { if(override f = this->get_override("activate")) f(); C::Activate(); }
	void default_Activate() { C::Activate(); }

	virtual void Deactivate() { if(override f = this->get_override("deactivate")) f(); C::Deactivate(); }
	void default_Deactivate() { C::Deactivate(); }

	virtual bool Key(dword key, int count) { if(override f = this->get_override("key")) return f(); return C::Key(key, count); }
	bool default_Key(dword key, int count) { return C::Key(key, count); }

	virtual Size GetMinSize() const { if(override f = this->get_override("getminsize")) return f(); return C::GetMinSize(); }
	Size default_GetMinSize() const { return C::GetMinSize(); }

	virtual Size GetStdSize() const { if(override f = this->get_override("getstdsize")) return f(); return C::GetStdSize(); }
	Size default_GetStdSize() const { return C::GetStdSize(); }

	virtual bool IsShowEnabled() const { if(override f = this->get_override("isshowenabled")) return f(); return C::IsShowEnabled(); }
	bool default_IsShowEnabled() const { return C::IsShowEnabled(); }

	virtual void Close() { if(override f = this->get_override("close")) f(); C::Close(); }
	void default_Close() { C::Close(); }

	virtual void ChildGotFocus() { if(override f = this->get_override("childgotfocus")) f(); C::ChildGotFocus(); }
	void default_ChildGotFocus() { C::ChildGotFocus(); }

	virtual String GetDesc() const { if(override f = this->get_override("getdesc")) return f(); return C::GetDesc(); }
	String default_GetDesc() const { return C::GetDesc(); }
};

//virtuals
void TopWindowClose(TopWindow& c) { c.Close(); }

//return ignore
void TopWindowSetTitle(TopWindow& c, const String& s) { c.Title(s); }
String TopWindowGetTitle(const TopWindow& c) { return c.GetTitle().ToString(); }

void TopWindowFullScreen(TopWindow& c, bool b) { c.FullScreen(b); }
void TopWindowSizeable(TopWindow& c, bool b) { c.Sizeable(b); }
void TopWindowZoomable(TopWindow& c, bool b) { c.Zoomable(b); }
void TopWindowTopMost(TopWindow& c, bool b, bool d) { c.TopMost(b, d); }
void TopWindowToolWindow(TopWindow& c, bool b) { c.ToolWindow(b); }

//enum helper, boost needs namespace
enum TopWindowEnum
{
	_IDOK = IDOK,
	_IDCANCEL = IDCANCEL,
	_IDABORT = IDABORT,
	_IDRETRY = IDRETRY,
	_IDIGNORE = IDIGNORE,
	_IDYES = IDYES,
	_IDNO = IDNO,
	_IDCLOSE = IDCLOSE,
	_IDHELP = IDHELP,
	_IDEXIT = IDEXIT,
};

void export_TopWindow()
{
ONCELOCK
{
	enum_<TopWindowEnum>("choice")
		.value("idok", _IDOK)
		.value("idcancel", _IDCANCEL)
		.value("idabort", _IDABORT)
		.value("idretry", _IDRETRY)
		.value("idignore", _IDIGNORE)
		.value("idyes", _IDYES)
		.value("idno", _IDNO)
		.value("idclose", _IDCLOSE)
		.value("idhelp", _IDHELP)
		.value("idexit", _IDEXIT)
	;

	class_<TopWindowWrap, bases<Ctrl>, boost::noncopyable>("TopWindow", "Upp TopWindow", no_init)

		.def("accept", &TopWindow::Accept, &TopWindowWrap::default_Accept)
		.def("reject", &TopWindow::Reject, &TopWindowWrap::default_Reject)
		.def("activate", &TopWindow::Activate, &TopWindowWrap::default_Activate)
		.def("deactivate", &TopWindow::Deactivate, &TopWindowWrap::default_Deactivate)
		.def("key", &TopWindow::Key, &TopWindowWrap::default_Key)
		.def("getminsize", &TopWindow::GetMinSize, &TopWindowWrap::default_GetMinSize)
		.def("getstdsize", &TopWindow::GetStdSize, &TopWindowWrap::default_GetStdSize)
		.def("isshowenabled", &TopWindow::IsShowEnabled, &TopWindowWrap::default_IsShowEnabled)
		.def("close", &TopWindow::Close, &TopWindowWrap::default_Close)
		.def("getdesc", &TopWindow::GetDesc, &TopWindowWrap::default_GetDesc)
		.def("childgotfocus", &TopWindow::ChildGotFocus, &TopWindowWrap::default_ChildGotFocus)

		.def_readwrite("whenclose", &TopWindow::WhenClose)

		.def("backup", &TopWindow::Backup)
		.def("restore", &TopWindow::Restore)
		.def("break", &TopWindow::Break)
		.def("acceptbreak", &TopWindow::AcceptBreak)
		.def("rejectbreak", &TopWindow::RejectBreak)

		.def("workareatrim", &TopWindow::WorkAreaTrim)

		.def("setminsize", &TopWindow::SetMinSize)

		.def("open", (void (TopWindow::*)())&TopWindow::Open)
		.def("openmain", &TopWindow::OpenMain)
		.def("run", &TopWindow::Run)
		.def("runappmodal", &TopWindow::RunAppModal)
		.def("execute", &TopWindow::Execute)
		.def("executeok", &TopWindow::ExecuteOK)
		.def("executecancel", &TopWindow::ExecuteCancel)

		.add_property("minimize", &TopWindow::IsMinimized, &TopWindow::Minimize)
		.add_property("maximize", &TopWindow::IsMaximized, &TopWindow::Maximize)
		.add_property("overlap", &TopWindow::IsOverlapped, &TopWindow::Overlap)

		.add_property("sizeable", &TopWindow::IsSizeable, &TopWindowSizeable)
		.add_property("zoomable", &TopWindow::IsZoomable, &TopWindowZoomable)
		.add_property("topmost", &TopWindow::IsTopMost, &TopWindowTopMost)
		.add_property("toolwindow", &TopWindow::IsToolWindow, &TopWindowToolWindow)

		.add_property("fullscreen", &TopWindow::IsFullScreen, &TopWindowFullScreen)
	;
}
}

END_UPP_NAMESPACE
