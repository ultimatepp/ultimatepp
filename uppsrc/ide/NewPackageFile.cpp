#include "ide.h"

class NewPackageFileWindow : public WithNewPackageFileLayout<TopWindow> {
public:
	NewPackageFileWindow(const String& packageDir, const String& extension);
	
	
	String GetFileName() const;
	
private:
	void OnFileNameChanged();

	void CheckFilePath();
	
private:
	String packageDir;
	String extension;
};

NewPackageFileWindow::NewPackageFileWindow(const String& packageDir, const String& extension)
	: packageDir(packageDir)
	, extension(extension)
{
	CtrlLayoutOKCancel(*this, "");
	
	fileName.WhenAction << [=] { OnFileNameChanged(); };
	
	fullFileName.Disable();
	if (extension.IsEmpty()) {
		fullFileNameLabel.Hide();
		fullFileName.Hide();
		
		info.VSizePosZ(28, 36);
	}
	
	OnFileNameChanged();
}

void NewPackageFileWindow::OnFileNameChanged()
{
	String name = fileName.GetData();
	if (!extension.IsEmpty())
		fullFileName.SetData(name + "." + extension);
	else
		fullFileName.SetData(name);
	
	CheckFilePath();
}

void NewPackageFileWindow::CheckFilePath()
{
	String name = fileName.GetData();
	String fullName = fullFileName.GetData();
	
	info.Clear();
	if (name.IsEmpty()) {
		ok.Disable();
	}
	else
	if (FileExists(packageDir + DIR_SEPS + fullName)) {
		ok.Disable();
		info.SetData("[ [ [@(170.42.0) File already exists.]]]");
	}
	else {
		ok.Enable();
	}
}

String NewPackageFileWindow::GetFileName() const
{
	return fullFileName.GetData();
}

void WorkspaceWork::NewPackageFile(const String& title, const String& extension)
{
	NewPackageFileWindow dlg(GetFileFolder(GetActivePackagePath()), extension);
	dlg.Title(title);
	
	if (dlg.ExecuteOK())
		AddItem(dlg.GetFileName(), false, false);
}
