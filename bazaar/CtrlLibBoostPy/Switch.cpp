#include "Common.h"
using namespace boost::python;

NAMESPACE_UPP

void SwitchSetLabel(Switch& c, const String& s) { c.SetLabel(s); }

void export_Switch()
{
ONCELOCK
{
	class_<Switch, bases<Ctrl>, boost::noncopyable>("Switch", "Upp Switch")
		.add_property("label", (String (Switch::*)() const)&Switch::GetLabel, &SwitchSetLabel)
	;
}
}

END_UPP_NAMESPACE
