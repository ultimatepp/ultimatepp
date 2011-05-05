#include "CtrlCore.h"
using namespace boost::python;

NAMESPACE_UPP

void export_Logc()
{
ONCELOCK
{
	enum_<Ctrl::PlacementConstants>("choice")
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
bool CtrlAccept(Ctrl& c) { return c.Accept(); }
void CtrlReject(Ctrl& c) { c.Reject(); }

void CtrlSetModify(Ctrl& c, bool b) { if (b) c.SetModify(); else c.ClearModify(); }
bool CtrlIsModified(const Ctrl& c) { return c.IsModified(); }

Value CtrlGetData(const Ctrl& c) { return c.GetData(); }
void CtrlSetData(Ctrl& c, const Value& data) { c.SetData(data); }

//return ignorers
void CtrlWantFocus(Ctrl& c, bool b) { c.WantFocus(b); }
void CtrlInitFocus(Ctrl& c, bool b) { c.InitFocus(b); }
void CtrlSetEditable(Ctrl& c, bool b) { c.SetEditable(b); }
void CtrlSetPos(Ctrl& c, const Ctrl::LogPos& p) { c.SetPos(p); }
void CtrlSetTip(Ctrl& c, const String& s) { c.Tip(s); }
void CtrlTransparent(Ctrl& c, bool b) { c.Transparent(b); }

void CtrlLeftPos(Ctrl& c, int a, int size) { c.LeftPos(a, size); }
void CtrlRightPos(Ctrl& c, int a, int size) { c.RightPos(a, size); }
void CtrlTopPos(Ctrl& c, int a, int size) { c.TopPos(a, size); }
void CtrlBottomPos(Ctrl& c, int a, int size) { c.BottomPos(a, size); }
void CtrlHSizePos(Ctrl& c, int a, int b) { c.HSizePos(a, b); }
void CtrlVSizePos(Ctrl& c, int a, int b) { c.VSizePos(a, b); }
void CtrlHCenterPos(Ctrl& c, int size, int delta) { c.HCenterPos(size, delta); }
void CtrlVCenterPos(Ctrl& c, int size, int delta) { c.HCenterPos(size, delta); }
void CtrlSizePos(Ctrl& c) { c.SizePos(); }

//other
void CtrlSetFocus(Ctrl& c, bool b) { if(b) c.SetFocus(); else if(c.GetParent()) c.GetParent()->SetFocus(); }
void CtrlSetBackPaint(Ctrl& c, bool b) { c.BackPaint(b?(Ctrl::FULLBACKPAINT):(Ctrl::NOBACKPAINT)); }
bool CtrlIsBackPaint(const Ctrl& c) { return (c.GetBackPaint()!=Ctrl::NOBACKPAINT); }

String CtrlToString(const Ctrl& c)
{
	return String().Cat() << String(typeid(c).name()) << ": " << AsString(c.GetData());
}

//

void export_CtrlCore()
{
ONCELOCK
{
	export_Core();
	export_LogPos();
	class_<Ctrl, boost::noncopyable>("Ctrl", "Upp Ctrl", no_init)

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

		.def("accept", &CtrlAccept)
		.def("reject", &CtrlReject)

		.add_property("data", &CtrlGetData, &CtrlSetData)

		.add_property("modify", &CtrlIsModified, &CtrlSetModify)
		.def("resetmodify", &Ctrl::ResetModify)

		.add_property("pos", &Ctrl::GetPos, &CtrlSetPos)
		.add_property("rect", &Ctrl::GetRect, (void (Ctrl::*)(const Rect&))&Ctrl::SetRect)
		.add_property("size", &Ctrl::GetSize)

		.def("leftpos", &CtrlLeftPos)
		.def("rightpos", &CtrlRightPos)
		.def("toppos", &CtrlTopPos)
		.def("bottompos", &CtrlBottomPos)
		.def("hsizepos", &CtrlHSizePos)
		.def("vsizepos", &CtrlVSizePos)
		.def("hcenterpos", &CtrlHCenterPos)
		.def("vcenterpos", &CtrlVCenterPos)
		.def("sizepos", &CtrlSizePos)

		.add_property("focus", &Ctrl::HasFocus, &CtrlSetFocus)
		.add_property("wantfocus", &CtrlWantFocus, &Ctrl::IsWantFocus)
		.add_property("initfocus", &Ctrl::IsInitFocus, &CtrlInitFocus)

		.add_property("show", &Ctrl::IsVisible, &Ctrl::Show)
		.add_property("shown", &Ctrl::IsShown)
		.add_property("enable", &Ctrl::IsEnabled, &Ctrl::Enable)
		.add_property("edit", &Ctrl::IsEditable, &CtrlSetEditable)

		.def("update", &Ctrl::Update)
		.def("action", &Ctrl::Action)
		.def("refresh", (void (Ctrl::*)(void))&Ctrl::Refresh)

		.add_property("backpaint", &CtrlIsBackPaint, &CtrlSetBackPaint)
		.add_property("transparent", &Ctrl::IsTransparent, &CtrlTransparent)
		.add_property("tip", &Ctrl::GetTip, &CtrlSetTip)

		.def("__str__", &CtrlToString)
	;
}
}

END_UPP_NAMESPACE
