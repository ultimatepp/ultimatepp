#include "Common.h"
using namespace boost::python;

NAMESPACE_UPP

void ButtonOptionSetLabel(ButtonOption& c, const String& s) { c.SetLabel(s); }

void export_ButtonOption()
{
ONCELOCK
{
	class_<ButtonOption, bases<Ctrl>, boost::noncopyable>("ButtonOption", "Upp ButtonOption")
		.add_property("label", &ButtonOption::GetLabel, &ButtonOptionSetLabel)
	;
}
}

END_UPP_NAMESPACE
