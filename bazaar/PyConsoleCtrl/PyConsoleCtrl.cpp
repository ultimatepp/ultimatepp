#include "PyConsoleCtrl.h"

void PyConsoleCtrl::LoadDlg()
{
	fs.Types("Python Files\t*.py\nAll Files\t*.*\n");
	if(!fs.ExecuteOpen("Load a Python script")) return;
	Load(fs.Get());
}

void PyConsoleCtrl::SaveDlg()
{
	fs.Types("Python Files\t*.py\nAll Files\t*.*\n");
	if(!fs.ExecuteSaveAs("Save a Python script")) return;
	Save(fs.Get());
}

void PyConsoleCtrl::Clear()
{
	cmd.Clear();
}

void PyConsoleCtrl::Exec()
{
	String c = cmd.GetData();
	if(c.IsEmpty()) { cmd.SetFocus(); return; }

	int ret = -1;

	int n = cmd.GetLineCount();
	ret = PyCon::Exec(c, n<=1);

	if(ret == 0)
	{
		if( clonex.Get())
			cmd.Clear();
		
		//history
		if(idx < ac.GetCount())
		{
			//executed sth from the history, move it to the end of history
			ac.Move(idx, ac.GetCount());
		}
		else
		{
			//executed new, add to the end, if not yet at the end.
			if(ac.IsEmpty() || ac[ac.GetCount()-1] != c)
				ac.Add(c);
		}
		idx = ac.GetCount(); //'commit' the history
	}
	ex.Clear();
	ex <<= ret;
	cmd.SetFocus();
}

void PyConsoleCtrl::ShowHistory(int i)
{
	i = minmax(i, 0, ac.GetCount()-1);
	cmd.SetData(ac[i]);
}

void PyConsoleCtrl::Inc()
{
	//got to next in hostory and display it
	++idx;
	if(idx >= ac.GetCount())
	{
		idx = ac.GetCount();
		Clear();
	}
	else
		ShowHistory(idx);
}

void PyConsoleCtrl::Dec()
{
	//go to previous in history
	--idx;
	if(idx < 0)
	{
		idx = 0;
//		Clear(); //no clear at beginning of history
	}
//	else
		ShowHistory(idx);
}

void PyConsoleCtrl::Dirtify()
{
	idx = ac.GetCount();	
}

PyConsoleCtrl::PyConsoleCtrl()
	: idx(0)
{
	CtrlLayout(*this);

	exec.AddFrame(sb);
	sb.inc.WhenRepeat = sb.inc.WhenAction = THISBACK(Dec); //reverse because of history
	sb.dec.WhenRepeat = sb.dec.WhenAction = THISBACK(Inc);
	cmd <<= THISBACK(Dirtify);

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

