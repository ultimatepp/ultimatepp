#include "ide.h"

struct JumpDlg : WithJumpLayout<TopWindow>, Navigator {
	typedef JumpDlg CLASSNAME;
	
	virtual bool Key(dword key, int count);

	virtual int GetCurrentLine();
	
	void GoTo();

	JumpDlg();
};

JumpDlg::JumpDlg()
{
	CtrlLayoutOKCancel(*this, "Navigator");
	dlgmode = true;
	search.WhenEnter.Clear();
}

bool JumpDlg::Key(dword key, int count)
{
	if(key == K_UP || key == K_DOWN) {
		if(list.IsCursor())
			return list.Key(key, count);
		else
			list.GoBegin();
		return true;
	}
	return TopWindow::Key(key, count);
}

void JumpDlg::GoTo()
{
	if(navlines.IsCursor()) {
		const NavLine& l = navlines.Get(0).To<NavLine>();
		theide->GotoPos(GetCppFile(l.file), l.line);
	}
}

int JumpDlg::GetCurrentLine()
{
	return theide->editor.GetCurrentLine();
}

void Ide::GotoGlobal()
{
	JumpDlg dlg;
	dlg.theide = this;
	dlg.Search();
	if(dlg.ExecuteOK())
		dlg.GoTo();
}
