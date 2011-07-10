#include "Common.h"
using namespace boost::python;

NAMESPACE_UPP

class ButtonOptionWrap : public ButtonOption, public wrapper<ButtonOption> {
public:
	typedef ButtonOption C;

	virtual void SetData(const Value& data) { if(override f = this->get_override("setdata")) f(data); C::SetData(data); }
	void default_SetData(const Value& data) { C::SetData(data); }

	virtual Value GetData() const { if(override f = this->get_override("getdata")) return f(); return C::GetData(); }
	Value default_GetData() const { return C::GetData(); }

	virtual void MouseEnter(Point p, dword keyflags) { if(override f = this->get_override("mouseenter")) f(p, keyflags); C::MouseEnter(p, keyflags); }
	void default_MouseEnter(Point p, dword keyflags) { C::MouseEnter(p, keyflags); }

	virtual void MouseMove(Point p, dword keyflags) { if(override f = this->get_override("mousemove")) f(p, keyflags); C::MouseMove(p, keyflags); }
	void default_MouseMove(Point p, dword keyflags) { C::MouseMove(p, keyflags); }

	virtual void MouseLeave() { if(override f = this->get_override("mouseleave")) f(); C::MouseLeave(); }
	void default_MouseLeave() { C::MouseLeave(); }

	virtual void LeftDown(Point p, dword keyflags) { if(override f = this->get_override("leftdown")) f(p, keyflags); C::LeftDown(p, keyflags); }
	void default_LeftDown(Point p, dword keyflags) { C::LeftDown(p, keyflags); }

	virtual void LeftUp(Point p, dword keyflags) { if(override f = this->get_override("leftup")) f(p, keyflags); C::LeftUp(p, keyflags); }
	void default_LeftUp(Point p, dword keyflags) { C::LeftUp(p, keyflags); }

	virtual dword GetAccessKeys() const { if(override f = this->get_override("getaccesskeys")) return f(); return C::GetAccessKeys(); }
	dword default_GetAccessKeys() const { return C::GetAccessKeys(); }

	virtual void AssignAccessKeys(dword key) { if(override f = this->get_override("assignaccesskeys")) f(key); C::AssignAccessKeys(key); }
	void default_AssignAccessKeys(dword key) { C::AssignAccessKeys(key); }
};

void ButtonOptionSetLabel(ButtonOption& c, const String& s) { c.SetLabel(s); }

void export_ButtonOption()
{
ONCELOCK
{
	class_<ButtonOption, bases<Ctrl>, boost::noncopyable>("ButtonOption", "Upp ButtonOption")
		.def("setdata", &ButtonOption::SetData, &ButtonOptionWrap::default_SetData)
		.def("getdata", &ButtonOption::GetData, &ButtonOptionWrap::default_GetData)
		.def("mouseenter", &ButtonOption::MouseEnter, &ButtonOptionWrap::default_MouseEnter)
		.def("mousemove", &ButtonOption::MouseMove, &ButtonOptionWrap::default_MouseMove)
		.def("mouseleave", &ButtonOption::MouseLeave, &ButtonOptionWrap::default_MouseLeave)
		.def("leftdown", &ButtonOption::LeftDown, &ButtonOptionWrap::default_LeftDown)
		.def("leftup", &ButtonOption::LeftUp, &ButtonOptionWrap::default_LeftUp)
		.def("getaccesskeys", &ButtonOption::GetAccessKeys, &ButtonOptionWrap::default_GetAccessKeys)
		.def("assignaccesskeys", &ButtonOption::AssignAccessKeys, &ButtonOptionWrap::default_AssignAccessKeys)

		.add_property("label", &ButtonOption::GetLabel, &ButtonOptionSetLabel)
		.add_property("data", &ButtonOption::Get, &ButtonOption::Set)
	;
}
}

END_UPP_NAMESPACE
