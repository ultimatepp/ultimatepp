#include "ValueCtrl.h"
using namespace boost::python;

NAMESPACE_UPP

void export_ValueCtrl()
{
ONCELOCK
{
	class_<ValueCtrl, bases<Ctrl>, boost::noncopyable>("ValueCtrl", "Upp ValueCtrl")
	;
}
}

END_UPP_NAMESPACE
