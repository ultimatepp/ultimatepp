#include "Common.h"
using namespace boost::python;

NAMESPACE_UPP

void export_Button()
{
ONCELOCK
{
	class_<Button, bases<Pusher>, boost::noncopyable>("Button", "Upp Button")
	;
}
}

END_UPP_NAMESPACE
