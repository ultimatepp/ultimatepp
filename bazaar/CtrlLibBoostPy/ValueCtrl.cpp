#include "ValueCtrl.h"
using namespace boost::python;

NAMESPACE_UPP

void export_ValueCtrl()
{
ONCELOCK
{
	scope().attr("__doc__") = "ValueCtrl module's docstring";
	class_<ValueCtrl, bases<Ctrl>, ValueCtrl, boost::noncopyable>("ValueCtrl", "An Upp ValueCtrl", no_init)
	;
}
}

END_UPP_NAMESPACE
