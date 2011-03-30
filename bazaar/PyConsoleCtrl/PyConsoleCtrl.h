#ifndef _PyConsoleCtrl_PyConsoleCtrl_h
#define _PyConsoleCtrl_PyConsoleCtrl_h

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include <PyCon/PyCon.h>

#define LAYOUTFILE <PyConsoleCtrl/PyConsoleCtrl.lay>
#include <CtrlCore/lay.h>

class CmdDocEdit : public DocEdit
{
public:
	typedef CmdDocEdit CLASSNAME;
	typedef DocEdit D;

	CmdDocEdit()
	{
		ProcessEnter().ProcessTab();
	}

	virtual bool Key(dword key, int count)
	{
		bool b = true;
		if(key == K_CTRL_ENTER)
			WhenCmdDone();
		else b = D::Key(key, count);
		return b;
	}

	Callback WhenCmdDone;
};

class PyConsoleCtrl : public WithPyConsoleCtrlLay<ParentCtrl>
{
public:
	typedef PyConsoleCtrl CLASSNAME;
	PyConsoleCtrl();
	
	void LoadDlg();
	void SaveDlg();
	void Exec();
	void Clear();

	void ClearLog() { log.Clear(); }
	void SaveLog() { log.Save(); }
	
	LoggerCtrl log;
	CmdDocEdit cmd;

protected:
	FileSel fs;
};

#endif

