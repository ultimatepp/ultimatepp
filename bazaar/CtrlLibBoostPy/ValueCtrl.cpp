#include "ValueCtrl.h"
using namespace boost::python;

NAMESPACE_UPP

void export_ValueCtrl()
{
ONCELOCK
{
	class_<ValueCtrl, bases<Ctrl>, ValueCtrl, boost::noncopyable>("ValueCtrl", "Upp ValueCtrl", no_init)
	;
}
}

END_UPP_NAMESPACE
