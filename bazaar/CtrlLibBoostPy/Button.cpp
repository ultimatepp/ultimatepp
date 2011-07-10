#include "Common.h"
using namespace boost::python;

NAMESPACE_UPP

class ButtonWrap : public Button, public wrapper<Button> {
public:
	typedef Button C;

	virtual int OverPaint() const { if(override f = this->get_override("overpaint")) return f(); return C::OverPaint(); }
	int default_OverPaint() const { return C::OverPaint(); }

	virtual void MouseEnter(Point p, dword keyflags) { if(override f = this->get_override("mouseenter")) f(p, keyflags); C::MouseEnter(p, keyflags); }
	void default_MouseEnter(Point p, dword keyflags) { C::MouseEnter(p, keyflags); }

	virtual void MouseLeave() { if(override f = this->get_override("mouseleave")) f(); C::MouseLeave(); }
	void default_MouseLeave() { C::MouseLeave(); }

	virtual void GotFocus() { if(override f = this->get_override("gotfocus")) f(); C::GotFocus(); }
	void default_GotFocus() { C::GotFocus(); }

	virtual void LostFocus() { if(override f = this->get_override("lostfocus")) f(); C::LostFocus(); }
	void default_LostFocus() { C::LostFocus(); }

	virtual bool Key(dword key, int count) { if(override f = this->get_override("key")) return f(key, count); return C::Key(key, count); }
	bool default_Key(dword key, int count) { return C::Key(key, count); }

	virtual dword GetAccessKeys() const { if(override f = this->get_override("getaccesskeys")) return f(); return C::GetAccessKeys(); }
	dword default_GetAccessKeys() const { return C::GetAccessKeys(); }

	virtual void AssignAccessKeys(dword key) { if(override f = this->get_override("assignaccesskeys")) f(key); C::AssignAccessKeys(key); }
	void default_AssignAccessKeys(dword key) { C::AssignAccessKeys(key); }

	virtual bool HotKey(dword key) { if(override f = this->get_override("hotkey")) return f(key); return C::HotKey(key); }
	bool default_HotKey(dword key) { return C::HotKey(key); }

	virtual void Layout() { if(override f = this->get_override("layout")) f(); C::Layout(); }
	void default_Layout() { C::Layout(); }
};

void ButtonOk(Button& c) { c.Ok(); }
void ButtonCancel(Button& c) { c.Cancel(); }
void ButtonExit(Button& c) { c.Exit(); }
void ButtonNormal(Button& c) { c.Normal(); }

void export_Button()
{
ONCELOCK
{
	class_<Button, bases<Pusher>, boost::noncopyable>("Button", "Upp Button")
		.def("overpaint", &Button::OverPaint, &ButtonWrap::default_OverPaint)
		.def("mouseenter", &Button::MouseEnter, &ButtonWrap::default_MouseEnter)
		.def("mouseleave", &Button::MouseLeave, &ButtonWrap::default_MouseLeave)
		.def("gotfocus", &Button::GotFocus, &ButtonWrap::default_GotFocus)
		.def("lostfocus", &Button::LostFocus, &ButtonWrap::default_LostFocus)
		.def("key", &Button::Key, &ButtonWrap::default_Key)
		.def("getaccesskeys", &Button::GetAccessKeys, &ButtonWrap::default_GetAccessKeys)
		.def("assignaccesskeys", &Button::AssignAccessKeys, &ButtonWrap::default_AssignAccessKeys)
		.def("hotkey", &Button::HotKey, &ButtonWrap::default_HotKey)
		.def("layout", &Button::Layout, &ButtonWrap::default_Layout)

		.def("ok", &ButtonOk)
		.def("ok", &ButtonCancel)
		.def("ok", &ButtonExit)
		.def("ok", &ButtonNormal)
	;
}
}

END_UPP_NAMESPACE
