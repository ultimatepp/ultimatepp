#include "Misc.h"
using namespace boost::python;

NAMESPACE_UPP

void DoLog(const String& s)
{
	RLOG(s);	
}

void export_Log()
{
ONCELOCK
{
	def("log", &DoLog, "Writes to the UppLog");

}
}

END_UPP_NAMESPACE
