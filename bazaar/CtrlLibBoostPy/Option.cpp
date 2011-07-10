#include "Common.h"
using namespace boost::python;

NAMESPACE_UPP

class OptionExpose : public Option
{
public:
	typedef Option P;

	inline virtual void RefreshPush() { return P::RefreshPush(); }
	inline virtual void RefreshFocus() { return P::RefreshFocus(); }
	inline virtual void PerformAction() { return P::PerformAction(); }
};

class OptionWrap : public Option, public wrapper<Option> {
public:
	typedef Option C;

	virtual Size GetMinSize() const { if(override f = this->get_override("getminsize")) return f(); return C::GetMinSize(); }
	Size default_GetMinSize() const { return C::GetMinSize(); }

	virtual void SetData(const Value& data) { if(override f = this->get_override("setdata")) f(data); C::SetData(data); }
	void default_SetData(const Value& data) { C::SetData(data); }

	virtual Value GetData() const { if(override f = this->get_override("getdata")) return f(); return C::GetData(); }
	Value default_GetData() const { return C::GetData(); }

	virtual void MouseEnter(Point p, dword keyflags) { if(override f = this->get_override("mouseenter")) f(p, keyflags); C::MouseEnter(p, keyflags); }
	void default_MouseEnter(Point p, dword keyflags) { C::MouseEnter(p, keyflags); }

	virtual void MouseLeave() { if(override f = this->get_override("mouseleave")) f(); C::MouseLeave(); }
	void default_MouseLeave() { C::MouseLeave(); }

	//own
	virtual void RefreshPush() { if(override f = this->get_override("refreshpush")) f(); C::RefreshPush(); }
	void default_RefreshPush() { C::RefreshPush(); }

	virtual void RefreshFocus() { if(override f = this->get_override("refreshfocus")) f(); C::RefreshFocus(); }
	void default_RefreshFocus() { C::RefreshFocus(); }

	virtual void PerformAction() { if(override f = this->get_override("performaction")) f(); C::PerformAction(); }
	void default_PerformAction() { C::PerformAction(); }
};

void OptionBlackEdge(Option& o, bool b) { o.BlackEdge(b); }
void OptionSwitchImage(Option& o, bool b) { o.SwitchImage(b); }
void OptionThreeState(Option& o, bool b) { o.ThreeState(b); }
void OptionShowLabel(Option& o, bool b) { o.ShowLabel(b); }
void OptionNotNull(Option& o, bool b) { o.NotNull(b); }
void OptionSet(Option& o, int b) { o.Set(b); }

void export_Option()
{
ONCELOCK
{
	class_<Option, bases<Pusher>, boost::noncopyable>("Option", "Upp Option")
		.def("getminsize", &Option::GetMinSize, &OptionWrap::default_GetMinSize)
		.def("setdata", &Option::SetData, &OptionWrap::default_SetData)
		.def("getdata", &Option::GetData, &OptionWrap::default_GetData)
		.def("mouseenter", &Option::MouseEnter, &OptionWrap::default_MouseEnter)
		.def("mouseleave", &Option::MouseLeave, &OptionWrap::default_MouseLeave)

		.def("refreshpush", (void (Option::*)())&OptionExpose::RefreshPush, &OptionWrap::default_RefreshPush)
		.def("refreshfocus", (void (Option::*)())&OptionExpose::RefreshFocus, &OptionWrap::default_RefreshFocus)
		.def("performaction", (void (Option::*)())&OptionExpose::PerformAction, &OptionWrap::default_PerformAction)

		.add_property("blackedge", &Option::IsBlackEdge, &OptionBlackEdge)
		.add_property("switchimage", &Option::IsSwitchImage, &OptionSwitchImage)
		.add_property("threestate", &Option::IsThreeState, &OptionThreeState)
		.add_property("showlabel", &Option::IsShowLabel, &OptionShowLabel)
		.add_property("notnull", &Option::IsNotNull, &OptionNotNull)

		.add_property("data", &Option::Get, &OptionSet)
	;
}
}

END_UPP_NAMESPACE
