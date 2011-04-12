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

void PyConsoleCtrl::Exec()
{
	String c = cmd.GetData();
	if(c.IsEmpty()) return;

	int ret = -1;

	int n = cmd.GetLineCount();
	ret = PyCon::Exec(c, n<=1);

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

	clonex.SetLabel("AutoCl");
	clonex.Set(1);

	PyCon::SetStream(log);
	PyCon::Init();
}

