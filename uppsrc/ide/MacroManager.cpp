#include "ide.h"

#define METHOD_NAME "MacroManagerWindow " << UPP_FUNCTION_NAME << "(): "

MacroManagerWindow::MacroManagerWindow(Ide& ide)
	: ide(ide)
{
	CtrlLayout(*this, t_("Macro Manager"));
	Zoomable().Sizeable().MinimizeBox(false);
	macrosTree.NoRoot();
	
	parent.Add(editor.SizePos());
	parent.AddFrame(splitter.Left(macrosTree, 300));

	editor.Highlight("usc");
	editor.SetReadOnly();

	editButton.Disable();
	exportButton.Disable();

	LoadMacros();
	macrosTree.OpenDeep(0);
	editor.Hide();
	
	macrosTree.WhenSel = [=] { OnMacroSel(); };
	
	InitButtons();
};

void MacroManagerWindow::InitButtons()
{
	closeButton.Tip(t_("Close macro manager."));
	editButton.Tip(t_("Edit selected element inside TheIDE."));
	importButton.Tip(t_("Import macro from .usc file to your global macros store."));
	exportButton.Tip(t_("Export current selected element. If macro or function is selected the parent file will be exported."));
	
	closeButton.Close();
	
	closeButton  << [=] { Break(); };
	editButton   << [=] { OnEditFile(); };
	importButton <<	[=] { OnImport(); };
	exportButton <<	[=] { OnExport(); };
}

void MacroManagerWindow::Layout()
{
	OnMacroSel();
}

void MacroManagerWindow::OnMacroSel()
{
	
	if(!macrosTree.IsCursor()) {
		editor.Hide();
		editButton.Disable();
		exportButton.Disable();
		return;
	}

	Value key = macrosTree.Get();
	if(IsNumber(key)) {
		editor.Hide();
		editButton.Disable();
		
		int node = macrosTree.Find(key);
		exportButton.Enable(macrosTree.GetChildCount(node) > 0);
		
		return;
	}

	MacroElement element = ValueTo<MacroElement>(key);

	String macroContent = TrimBoth(element.comment);
	if(macroContent.GetCount())
		macroContent << "\n";

	macroContent << element.prototype <<  element.code;

	editButton.Enable();
	exportButton.Disable();
	editor.Show();
	editor.Set(macroContent);
}

void MacroManagerWindow::OnImport()
{
	String filePath = SelectFileOpen("*.usc");
	if(IsNull(filePath))
		return;
	
	{
		String localDir = GetLocalDir();
		if(!DirectoryExists(localDir) && !RealizeDirectory(localDir)) {
			ErrorOK(DeQtf(String(t_("Realizing directory")) << " \"" << localDir << "\" " << t_("failed.")));
			return;
		}
	}
	
	String newFileName = GetFileName(filePath);
	String newFilePath = LocalPath(newFileName);
	if(FileExists(newFilePath) && !PromptYesNo(DeQtf(GenFileOverrideMessage(newFileName))))
		return;

	FileCopy(filePath, newFilePath);
	ReloadGlobalMacros();
	OnMacroSel();
}

void MacroManagerWindow::ExportFiles(Index<String>& files, const String& dir)
{
	for(const String& file : files) {
		String fileName = GetFileName(file);
		String filePath = AppendFileName(dir, GetFileName(file));
		
		if(FileExists(filePath) && !PromptYesNo(DeQtf(GenFileOverrideMessage(fileName))))
			continue;
		
		FileCopy(file, filePath);
	}
}

void MacroManagerWindow::FindNodeFiles(int id, Index<String>& list)
{
	Value key = macrosTree.Get(id);
	if(IsNumber(key)) {
		for(int i = 0; i < macrosTree.GetChildCount(id); i++) {
			int node = macrosTree.GetChild(id, i);
			FindNodeFiles(node, list);
		}
	}
	else {
		MacroElement element = ValueTo<MacroElement>(key);
		list.FindAdd(element.fileName);
	}
}

void MacroManagerWindow::OnExport()
{
	if(!macrosTree.IsCursor())
		return;
	
	String dir = SelectDirectory();
	if(dir.IsEmpty())
		return;

	Value key = macrosTree.Get();
	if(IsNumber(key)) {
		int id = macrosTree.GetCursor();
		Index<String> list;
		FindNodeFiles(id, list);
		ExportFiles(list, dir);
	}
}

void MacroManagerWindow::OnEditFile()
{
	if(!macrosTree.IsCursor())
		return;

	Value key = macrosTree.Get();
	if(IsNumber(key))
		return;

	MacroElement element = ValueTo<MacroElement>(key);

	ide.EditFile(element.fileName);
	ide.editor.SetCursor(ide.editor.GetPos(element.line - 1));
	ide.editor.CenterCursor();
	
	Break();
}

String MacroManagerWindow::GenFileOverrideMessage(const String& fileName)
{
	return String(t_("Target file")) << " \"" << fileName << "\" " << t_("already exists! Do you want to overwrite it?");
}

void MacroManagerWindow::LoadUscDir(const String& dir, MacroList& store)
{
	for(FindFile ff(AppendFileName(dir, "*.usc")); ff; ff.Next())
	{
		auto list = UscFileParser(AppendFileName(dir, ff.GetName())).Parse();
		if(list.GetCount())
			store.AppendRange(list);
	}
}

void MacroManagerWindow::LoadMacros()
{
	ReloadGlobalMacros();
	ReloadLocalMacros();
}

void MacroManagerWindow::ReloadGlobalMacros()
{
	MacroList global;
	LoadUscDir(GetLocalDir(), global);
	LoadUscDir(GetFileFolder(ConfigFile("x")), global);
	
	int configFolderNameLength = GetFileFolder(ConfigFile("x")).GetCount() + 1;

	int globalNode = macrosTree.Find(0);
	if(globalNode < 0) {
		globalNode = macrosTree.Add(0, Image(), 0, t_("Global macros"));
	}
	else {
		macrosTree.RemoveChildren(globalNode);
	}

	String file;
	int fileNode = -1;
	for(const auto& element : global) {
		if(!file.IsEqual(element.fileName)) {
			file = element.fileName;
			String fileTitle = file.Mid(configFolderNameLength);
			if(!fileTitle.TrimStart(String() << "UppLocal" << DIR_SEPS))
				fileTitle = "../" + fileTitle;
			fileNode = macrosTree.Add(globalNode, Image(), 3, fileTitle);
		}
			
		macrosTree.Add(fileNode, MacroElement::GetImage(element.type), RawToValue(element), element.name);
	}
}

void MacroManagerWindow::ReloadLocalMacros()
{
	int localNode = macrosTree.Add(0, Image(), 1, t_("Local macros"));

	const Workspace& wspc = ide.IdeWorkspace();

	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& package = wspc.GetPackage(i);
		int packageNode = -1;
		for (const auto& file : package.file) {
			String filePath = SourcePath(wspc[i], file);

			if (ToLower(GetFileExt(filePath)) != ".usc")
				continue;
				
			auto list = UscFileParser(filePath).Parse();
			if (list.GetCount() == 0)
				continue;
			
			if(packageNode == -1)
				packageNode = macrosTree.Add(localNode, Image(), 2, wspc[i]);
					
			int fileNode = macrosTree.Add(packageNode, Image(), 3, file);
			for(int j = 0; j < list.GetCount(); j++) {
				MacroElement& element = list[j];
				macrosTree.Add(fileNode, MacroElement::GetImage(element.type), RawToValue(element), element.name);
			}
		}
	}
}

void Ide::DoMacroManager()
{
	MacroManagerWindow(*this).Execute();
}
