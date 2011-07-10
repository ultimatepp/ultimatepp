#include "Common.h"
using namespace boost::python;

NAMESPACE_UPP

class SwitchWrap : public Switch, public wrapper<Switch> {
public:
	typedef Switch C;

	virtual void CancelMode() { if(override f = this->get_override("cancelmode")) f(); C::CancelMode(); }
	void default_CancelMode() { C::CancelMode(); }

	virtual void SetData(const Value& data) { if(override f = this->get_override("setdata")) f(data); C::SetData(data); }
	void default_SetData(const Value& data) { C::SetData(data); }

	virtual Value GetData() const { if(override f = this->get_override("getdata")) return f(); return C::GetData(); }
	Value default_GetData() const { return C::GetData(); }

	virtual void MouseMove(Point p, dword keyflags) { if(override f = this->get_override("mousemove")) f(p, keyflags); C::MouseMove(p, keyflags); }
	void default_MouseMove(Point p, dword keyflags) { C::MouseMove(p, keyflags); }

	virtual void MouseLeave() { if(override f = this->get_override("mouseleave")) f(); C::MouseLeave(); }
	void default_MouseLeave() { C::MouseLeave(); }

	virtual void LeftDown(Point p, dword keyflags) { if(override f = this->get_override("leftdown")) f(p, keyflags); C::LeftDown(p, keyflags); }
	void default_LeftDown(Point p, dword keyflags) { C::LeftDown(p, keyflags); }

	virtual void LeftUp(Point p, dword keyflags) { if(override f = this->get_override("leftup")) f(p, keyflags); C::LeftUp(p, keyflags); }
	void default_LeftUp(Point p, dword keyflags) { C::LeftUp(p, keyflags); }

	virtual void GotFocus() { if(override f = this->get_override("gotfocus")) f(); C::GotFocus(); }
	void default_GotFocus() { C::GotFocus(); }

	virtual void LostFocus() { if(override f = this->get_override("lostfocus")) f(); C::LostFocus(); }
	void default_LostFocus() { C::LostFocus(); }

	virtual bool Key(dword key, int count) { if(override f = this->get_override("key")) return f(key, count); return C::Key(key, count); }
	bool default_Key(dword key, int count) { return C::Key(key, count); }

	virtual bool HotKey(dword key) { if(override f = this->get_override("hotkey")) return f(key); return C::HotKey(key); }
	bool default_HotKey(dword key) { return C::HotKey(key); }

	virtual dword GetAccessKeys() const { if(override f = this->get_override("getaccesskeys")) return f(); return C::GetAccessKeys(); }
	dword default_GetAccessKeys() const { return C::GetAccessKeys(); }

	virtual void AssignAccessKeys(dword key) { if(override f = this->get_override("assignaccesskeys")) f(key); C::AssignAccessKeys(key); }
	void default_AssignAccessKeys(dword key) { C::AssignAccessKeys(key); }
};

void SwitchSetLabel(Switch& c, const String& s) { c.SetLabel(s); }
void SwitchMinCaseHeight(Switch& c, int cy) { c.MinCaseHeight(cy); }

void SwitchSet(Switch& c, int i, const Value& v, const String& s) { c.Set(i, v, s); }
void SwitchAdd(Switch& c, const Value& v, const String& s) { c.Add(v, s); }

void export_Switch()
{
ONCELOCK
{
	class_<Switch, bases<Ctrl>, boost::noncopyable>("Switch", "Upp Switch")
		.def("setdata", &Switch::SetData, &SwitchWrap::default_SetData)
		.def("getdata", &Switch::GetData, &SwitchWrap::default_GetData)
		.def("cancelmode", &Switch::CancelMode, &SwitchWrap::default_CancelMode)
		.def("leftdown", &Switch::LeftDown, &SwitchWrap::default_LeftDown)
		.def("leftup", &Switch::LeftUp, &SwitchWrap::default_LeftUp)
		.def("mousemove", &Switch::MouseMove, &SwitchWrap::default_MouseMove)
		.def("mouseleave", &Switch::MouseLeave, &SwitchWrap::default_MouseLeave)
		.def("gotfocus", &Switch::GotFocus, &SwitchWrap::default_GotFocus)
		.def("lostfocus", &Switch::LostFocus, &SwitchWrap::default_LostFocus)
		.def("key", &Switch::Key, &SwitchWrap::default_Key)
		.def("getaccesskeys", &Switch::GetAccessKeys, &SwitchWrap::default_GetAccessKeys)
		.def("assignaccesskeys", &Switch::AssignAccessKeys, &SwitchWrap::default_AssignAccessKeys)
		.def("hotkey", &Switch::HotKey, &SwitchWrap::default_HotKey)

		.def("set", &SwitchSet)
		.def("add", &SwitchAdd)

		.def("enablecase", &Switch::EnableCase)
		.def("enablevalue", &Switch::EnableValue)
		.def("reset", &Switch::Reset)
		.def("mincaseheight", &SwitchMinCaseHeight)

		.add_property("label", (String (Switch::*)() const)&Switch::GetLabel, &SwitchSetLabel)
	;
}
}

END_UPP_NAMESPACE
