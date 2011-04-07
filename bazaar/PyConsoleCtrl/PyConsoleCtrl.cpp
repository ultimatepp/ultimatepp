#include "PyConsoleCtrl.h"

void PyConsoleCtrl::LoadDlg()
{
	fs.Types("Python Files\t*.py\nAll Files\t*.*\n");
	if(!fs.ExecuteOpen("Load a Python script")) return;
	
	FileIn in(fs.Get());
	cmd.Load(in);
}

void PyConsoleCtrl::SaveDlg()
{
	fs.Types("Python Files\t*.py\nAll Files\t*.*\n");
	if(!fs.ExecuteSaveAs("Save a Python script")) return;
	
	FileOut out(fs.Get());
	cmd.Save(out);
}

void PyConsoleCtrl::Clear()
{
	cmd.Clear();
}

//for having an echo print of return PyObject, is simply a copy of PyRun_SimpleStringFlags, with Py_single_input
int
MyPyRun_SimpleStringFlags(const char *command, PyCompilerFlags *flags)
{
	PyObject *m, *d, *v;
	m = PyImport_AddModule("__main__");
	if (m == NULL)
		return -1;
	d = PyModule_GetDict(m);
	v = PyRun_StringFlags(command, Py_single_input, d, d, flags); //changed from Py_file_input
	if (v == NULL) {
		PyErr_Print();
		return -1;
	}
	Py_DECREF(v);
	if (Py_FlushLine())
		PyErr_Clear();
	return 0;
}

void PyConsoleCtrl::Exec()
{
	String c = cmd.GetData();
	if(c.IsEmpty()) return;

	int ret = MyPyRun_SimpleStringFlags(c, NULL);
	if(ret == 0 && clonex.Get())
		cmd.Clear();

	ex.Clear();
	ex <<= ret;
}

PyConsoleCtrl::PyConsoleCtrl()
{
	CtrlLayout(*this);

	spl.Vert(log, cmd);
	spl.SetPos(6600);
	Add(spl.HSizePos().VSizePos(0,20));

	load <<= THISBACK(LoadDlg);
	save <<= THISBACK(SaveDlg);
	clear <<= THISBACK(Clear);
	exec  <<= THISBACK(Exec);
	cmd.WhenCmdDone = THISBACK(Exec);

	savelog <<= THISBACK(SaveLog);
	clearlog <<= THISBACK(ClearLog);

	clonex.Set(1);

	PyCon::SetStream(log);
	PyCon::Init();
}

