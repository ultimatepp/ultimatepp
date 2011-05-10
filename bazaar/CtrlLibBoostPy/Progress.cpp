#include "Progress.h"
using namespace boost::python;

NAMESPACE_UPP

void export_Progress()
{
ONCELOCK
{
	class_<Progress, bases<TopWindow>, Progress, boost::noncopyable>("Progress", "Upp Progress", no_init)
		.add_property("pos", &Progress::GetPos, &Progress::SetPos)
		.add_property("total", &Progress::GetTotal, &Progress::SetTotal)
		.add_property("text", &Progress::GetText, (void (Progress::*)(const String&))&Progress::SetText)

		.def("create", &Progress::Create)
		.def("cancel", &Progress::Cancel)
		.def("reset", &Progress::Reset)
		.def("set", &Progress::Set)
		.def("step", &Progress::Step)

		.def("canceled", &Progress::Canceled)
		.def("setcanceled", &Progress::SetCanceled)
		.def("setposcanceled", &Progress::SetPosCanceled)
		.def("stepcanceled", &Progress::StepCanceled)
	;

}
}

END_UPP_NAMESPACE
