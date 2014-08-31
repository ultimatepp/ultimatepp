#include "ide.h"

struct JumpDlg : WithJumpLayout<TopWindow>, Navigator {
	typedef JumpDlg CLASSNAME;
	
	virtual bool Key(dword key, int count);

	virtual int GetCurrentLine();
	
	void Ok();
	void Cancel();

	JumpDlg();
};

JumpDlg::JumpDlg()
{
	CtrlLayout(*this, "Navigator");
	ok <<= THISBACK(Ok);
	cancel <<= THISBACK(Cancel);
	dlgmode = true;
}

bool JumpDlg::Key(dword key, int count)
{
	if(key == K_UP || key == K_DOWN) {
		if(list.IsCursor())
			return list.Key(key, count);
		else
			list.GoBegin();
	}
	return TopWindow::Key(key, count);
}

void JumpDlg::Ok()
{
	if(navlines.IsCursor()) {
		const NavLine& l = navlines.Get(0).To<NavLine>();
		theide->GotoPos(GetCppFile(l.file), l.line);
	}
	Close();
}

void JumpDlg::Cancel()
{
	Close();
}

int JumpDlg::GetCurrentLine()
{
	return theide->editor.GetCurrentLine();
}

void Ide::GotoGlobal()
{
	static JumpDlg dlg;
	dlg.theide = this;
	if(dlg.IsOpen())
		dlg.SetForeground();
	else 
		dlg.Open();
	dlg.Search();
}
