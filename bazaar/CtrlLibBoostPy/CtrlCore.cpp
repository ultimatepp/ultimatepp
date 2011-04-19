#include "CtrlCore.h"
using namespace boost::python;

NAMESPACE_UPP

Value CtrlGetData(const Ctrl& c) { return c.GetData(); }
void CtrlSetData(Ctrl& c, const Value& data) { c.SetData(data); }

void export_CtrlCore()
{
ONCELOCK
{
	scope().attr("__doc__") = "Ctrl's docstring";
	class_<Ctrl, boost::noncopyable>("Ctrl", "An Upp Ctrl", no_init)
		.def("getdata", CtrlGetData)
		.def("setdata", CtrlSetData)
	;
}
}

END_UPP_NAMESPACE
