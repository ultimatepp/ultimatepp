#include "ide.h"

class NewPackageFileWindow : public WithNewPackageFileLayout<TopWindow> {
	typedef NewPackageFileWindow CLASSNAME;
	
public:
	NewPackageFileWindow(const String& packageDir);
	
	String GetFileName() const;
	
private:
	void CheckFilePath();
	
private:
	String packageDir;
};

NewPackageFileWindow::NewPackageFileWindow(const String& packageDir)
	: packageDir(packageDir)
{
	CtrlLayoutOKCancel(*this, "New package file");
	
	fileName <<= THISBACK(CheckFilePath);
	
	ok.Disable();
}

void NewPackageFileWindow::CheckFilePath()
{
	String file = fileName.GetData();
	
	info.Clear();
	if (file.IsEmpty()) {
		ok.Disable();
	}
	else
	if (FileExists(packageDir + DIR_SEPS + file)) {
		ok.Disable();
		info.SetData("[ [ [@(170.42.0) File already exists.]]]");
	}
	else {
		ok.Enable();
	}
}

String NewPackageFileWindow::GetFileName() const
{
	return fileName.GetData();
}

void WorkspaceWork::NewPackageFile()
{
	NewPackageFileWindow dlg(GetFileFolder(GetActivePackagePath()));
	if (dlg.ExecuteOK())
		AddItem(dlg.GetFileName(), false, false);
}
