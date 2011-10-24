#include "PyConsoleCtrl.h"
using namespace boost::python;

NAMESPACE_UPP

void export_PyConsoleCtrl()
{
ONCELOCK
{
	export_CtrlLib();

	class_<PyConsoleCtrl, bases<Ctrl>, boost::noncopyable>("PyConsoleCtrl", "Upp PyConsoleCtrl")
		.def("clear", &PyConsoleCtrl::Clear)
		.def("savedlg", &PyConsoleCtrl::SaveDlg)
		.def("save", &PyConsoleCtrl::Save)
		.def("loaddlg", &PyConsoleCtrl::LoadDlg)
		.def("load", &PyConsoleCtrl::Load)
		//.def("exec", &PyConsoleCtrl::Exec) //very dangerous

		.def("clearlog", &PyConsoleCtrl::ClearLog)
		.def("savelog", &PyConsoleCtrl::SaveLog)
	;
}
}

END_UPP_NAMESPACE
