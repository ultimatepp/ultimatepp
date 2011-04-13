#include "SliderCtrlPy.h"

void export_UppCtrl()
{
	scope().attr("__doc__") = "ctrl module's docstring";
	class_<SliderCtrlPy, boost::noncopyable>("SliderCtrlPy", "A SliderCtrl wrapper", no_init)
		.def("get", &SliderCtrlPy::Get)
		.def("set", &SliderCtrlPy::Set)
	;
}

void export_SliderCtrl()
{
	scope().attr("__doc__") = "SliderCtrl module's docstring";
	class_<SliderCtrl, boost::noncopyable>("SliderCtrl", "An Upp SliderCtrl", no_init)
		.def("get", &SliderCtrl::GetData)
		.def("set", &SliderCtrl::SetData)
	;
}

void export_ValueCtrl()
{
	scope().attr("__doc__") = "ValueCtrl module's docstring";
	class_<ValueCtrl, boost::noncopyable>("ValueCtrl", "An Upp ValueCtrl", no_init)
		.def("get", &ValueCtrl::GetData)
		.def("set", &ValueCtrl::SetData)
	;
}

