#include "ide.h"

#define METHOD_NAME "MacroManagerWindow " << UPP_FUNCTION_NAME << "(): "

MacroManagerWindow::MacroManagerWindow(const Workspace& wspc)
	: wspc(wspc)
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
	
	globalNode = macrosTree.Add(0, Image(), 0, t_("Global macros"));

	LoadMacros();
	macrosTree.OpenDeep(0);
	editor.Hide();
	
	macrosTree.WhenSel = [=]           { OnMacroSel(); };
	macrosTree.WhenBar = [=](Bar& bar) { OnMacroBar(bar); };
	
	InitButtons();
}

void MacroManagerWindow::OnMacroBar(Bar& bar)
{
	bar.Add(t_("New.."),                      [=] { OnNewMacroFile();});
	bar.Add(t_("Import.."),                   [=] { OnImport();});
	bar.Add(t_("Delete"),                     [=] { OnDeleteMacroFile();})
	    .Enable(IsGlobalFile());
	bar.Add(t_("Export.."),                   [=] { OnExport();})
	    .Enable(IsGlobalFile() || IsGlobalRoot());
	bar.Separator();
	bar.Add(t_("Edit"),                       [=] { OnEditFile();})
	    .Enable(IsEditPossible());
}

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
	bool hasCursor = macrosTree.IsCursor();
	
	exportButton.Enable(hasCursor && (IsGlobalFile() || IsGlobalRoot()));
	editButton.Enable(hasCursor && IsEditPossible());
	editor.Show(editButton.IsEnabled());
	
	if(IsFile())
		editor.Set(LoadFile(static_cast<String>(macrosTree.Get())));
	else if(IsMacro())
		editor.Set(ValueTo<MacroElement>(macrosTree.Get()).GetContent());
}

void MacroManagerWindow::OnImport()
{
	String filePath = SelectFileOpen("*.usc");
	if(IsNull(filePath))
		return;
	
	if(!UscFileParser(filePath).IsValid()) {
		ErrorOK(DeQtf(String(t_("Import failed! Following file")) << " \"" << filePath << "\" " << t_("is not a valid macro file!")));
		return;
	}
	
	String localDir = GetLocalDir();
	if(!DirectoryExists(localDir) && !RealizeDirectory(localDir)) {
		ErrorOK(DeQtf(String(t_("Realizing directory")) << " \"" << localDir << "\" " << t_("failed.")));
		return;
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
	if(IsFile(id)) {
		list.FindAdd((String)macrosTree.Get(id));
		return;
	}
	
	for(int i = 0; i < macrosTree.GetChildCount(id); i++) {
		int node = macrosTree.GetChild(id, i);
		FindNodeFiles(node, list);
	}
}

void MacroManagerWindow::OnExport()
{
	if(!macrosTree.IsCursor())
		return;

	if(IsGlobalFile() || IsGlobalRoot()) {
		String dir = SelectDirectory();
		if(dir.IsEmpty())
			return;
		
		Index<String> list;
		FindNodeFiles(macrosTree.GetCursor(), list);
		ExportFiles(list, dir);
	}
}

void MacroManagerWindow::OnEditFile()
{
	if(!macrosTree.IsCursor())
		return;

	if(IsMacro()) {
		MacroElement element = ValueTo<MacroElement>(macrosTree.Get());
		WhenEdit(element.fileName, element.line - 1);
		Break();
	}
	else if(IsFile()) {
		WhenEdit( (String)macrosTree.Get(), 1);
		Break();
	}
}

void MacroManagerWindow::OnNewMacroFile()
{
	String fileName;
	if(!EditTextNotNull(fileName, t_("New global macro file"), t_("Macro file name:")))
		return;
	
	if(!fileName.EndsWith(".usc"))
		fileName << ".usc";
	
	String fullPath = AppendFileName(GetLocalDir(), fileName);
	if(FileExists(fullPath)) {
		PromptOK(String() << t_("file") << " \"" << fileName << "\" " << t_("already exists!"));
		return;
	}
	
	SaveFile(fullPath, "macro \"\" {}");
	ReloadGlobalMacros();
	
	macrosTree.FindSetCursor(fileName);
}

void MacroManagerWindow::OnDeleteMacroFile()
{
	String fileName = static_cast<String>(macrosTree.GetValue());
	if(!PromptOKCancel(t_("Are you sure you want to remove followin macro file \"" + fileName + "\"?"))) {
		return;
	}
	
	FileDelete(AppendFileName(GetLocalDir(), fileName));
	macrosTree.Remove(macrosTree.GetCursor());
	OnMacroSel();
}

String MacroManagerWindow::GenFileOverrideMessage(const String& fileName)
{
	return String(t_("Target file")) << " \"" << fileName << "\" " << t_("already exists! Do you want to overwrite it?");
}

void MacroManagerWindow::LoadUscDir(const String& dir)
{
	for(FindFile ff(AppendFileName(dir, "*.usc")); ff; ff.Next()) {
		String fileTitle = ff.GetName();
		if(!ff.GetPath().EndsWith(String() << "UppLocal" << DIR_SEPS << ff.GetName()))
			fileTitle = "../" + fileTitle;
		
		int fileNode = macrosTree.Add(globalNode, Image(), ff.GetPath(), fileTitle);
		
		auto list = UscFileParser(ff.GetPath()).Parse();
		for(const auto& element : list) {
			macrosTree.Add(fileNode, element.GetImage(), RawToValue(element), element.name);
		}
	}
}

void MacroManagerWindow::LoadMacros()
{
	ReloadGlobalMacros();
	ReloadLocalMacros();
}

void MacroManagerWindow::ReloadGlobalMacros()
{
	macrosTree.RemoveChildren(globalNode);

	LoadUscDir(GetLocalDir());
	LoadUscDir(GetFileFolder(ConfigFile("x")));
	
	macrosTree.OpenDeep(0);
}

void MacroManagerWindow::ReloadLocalMacros()
{
	int localNode = macrosTree.Find(1);
		
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
			
			if(localNode == -1)
				localNode  = macrosTree.Add(0, Image(), 1, t_("Local macros (Read only)"));
			
			if(packageNode == -1)
				packageNode = macrosTree.Add(localNode, Image(), 0, wspc[i]);
					
			int fileNode = macrosTree.Add(packageNode, Image(), filePath.ToWString(), file);
			for(int j = 0; j < list.GetCount(); j++) {
				MacroElement& element = list[j];
				macrosTree.Add(fileNode, element.GetImage(), RawToValue(element), element.name);
			}
		}
	}
}

void Ide::DoMacroManager()
{
	MacroManagerWindow dlg(IdeWorkspace());
	
	dlg.WhenEdit = [&](String fileName, int line) {
		EditFile(fileName);
		editor.SetCursor(editor.GetPos(line));
		editor.CenterCursor();
	};
	
	dlg.Execute();
}
