#include "ide.h"

struct NavDlg : WithJumpLayout<TopWindow>, Navigator {
	typedef NavDlg CLASSNAME;
	
	virtual bool Key(dword key, int count);

	virtual int GetCurrentLine();
	
	void GoTo();
	
	void Serialize(Stream& s);

	NavDlg();
};

void NavDlg::Serialize(Stream& s)
{
	
	SerializePlacement(s);
}

INITBLOCK {
	RegisterGlobalConfig("NavDlg");
}

NavDlg::NavDlg()
{
	CtrlLayoutOKCancel(*this, "Navigator");
	dlgmode = true;
	search.WhenEnter.Clear();
	Sizeable().Zoomable();
	Icon(IdeImg::Navigator());
}

bool NavDlg::Key(dword key, int count)
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

void NavDlg::GoTo()
{
	if(navlines.IsCursor()) {
		const NavLine& l = navlines.Get(0).To<NavLine>();
		theide->GotoPos(GetSourceFilePath(l.file), l.line);
	}
}

int NavDlg::GetCurrentLine()
{
	return theide->editor.GetCurrentLine();
}

void Ide::NavigatorDlg()
{
	NavDlg dlg;
	LoadFromGlobal(dlg, "NavDlg");
	dlg.theide = this;
	dlg.Search();
	if(dlg.ExecuteOK())
		dlg.GoTo();
	StoreToGlobal(dlg, "NavDlg");
}
