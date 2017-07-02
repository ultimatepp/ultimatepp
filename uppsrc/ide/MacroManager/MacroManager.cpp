#include "MacroManager.h"

#define IMAGECLASS MacroManagerImg
#define IMAGEFILE  <ide/MacroManager/MacroManager.iml>
#include <Draw/iml_source.h>

#define METHOD_NAME "MacroManagerWindow " << UPP_FUNCTION_NAME << "(): "

namespace Upp {

MacroManagerWindow::MacroManagerWindow(const Workspace& wspc, const String& hlStyles)
	: wspc(wspc)
{
	CtrlLayout(*this, t_("Macro Manager"));
	Zoomable().Sizeable().MinimizeBox(false);
	globalTree.NoRoot();
	localTree.NoRoot();
	
	parent.Add(editor.SizePos());
	parent.AddFrame(splitter.Left(tab, 330));
	tab.Add(globalTree.SizePos(), t_("Global macros"));

	editor.Highlight("usc");
	editor.SetReadOnly();

	editButton.Disable();
	exportButton.Disable();
	
	globalNode = globalTree.Add(0, Image(), 0, t_("Global macros"));

	LoadMacros();

	editor.Hide();
	editor.LoadHlStyles(hlStyles);
	
	InitButtons();
	InitEvents();
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

void MacroManagerWindow::InitEvents()
{
	globalTree.WhenSel = [=]           { OnTreeSel(); };
	localTree.WhenSel  = [=]           { OnTreeSel(); };
	
	globalTree.WhenBar = [=](Bar& bar) { OnMacroBar(bar); };
	localTree.WhenBar  = [=](Bar& bar) { OnMacroBar(bar); };
	
	tab.WhenSet        = [=]           { OnTabSet(); };
}

void MacroManagerWindow::OnMacroBar(Bar& bar)
{
	if(IsGlobalTab()) {
		bool partOfFile = IsGlobalFile() || IsGlobalRoot();
		
		bar.Add(t_("New.."),    [=] { OnNewMacroFile();});
		bar.Add(t_("Import.."), [=] { OnImport();});
		bar.Add(t_("Delete"),   [=] { OnDeleteMacroFile();})
		    .Enable(partOfFile);
		bar.Add(t_("Export.."), [=] { OnExport();})
		    .Enable(partOfFile);
		bar.Separator();
	}
	bar.Add(t_("Edit"), [=] { OnEditFile();})
	    .Enable(IsEditPossible());
}

void MacroManagerWindow::Layout()
{
	OnTreeSel();
}

void MacroManagerWindow::OnTreeSel()
{
	const TreeCtrl& tree = GetCurrentTree();
	bool hasCursor = tree.IsCursor();
	
	exportButton.Enable(IsGlobalTab() && hasCursor && (IsGlobalFile() || IsGlobalRoot()));
	
	editButton.Enable(hasCursor && IsEditPossible());
	editor.Show(editButton.IsEnabled());
	
	if(IsFile())
		editor.Set(LoadFile(static_cast<String>(tree.Get())));
	else if(IsMacro())
		editor.Set(ValueTo<MacroElement>(tree.Get()).GetContent());
}

void MacroManagerWindow::OnTabSet()
{
	exportButton.Show(tab.Get() == 0);
	editor.Hide();
	
	OnTreeSel();
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
	OnTreeSel();
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
		list.FindAdd((String)globalTree.Get(id));
		return;
	}
	
	for(int i = 0; i < globalTree.GetChildCount(id); i++) {
		int node = globalTree.GetChild(id, i);
		FindNodeFiles(node, list);
	}
}

void MacroManagerWindow::OnExport()
{
	if(!globalTree.IsCursor())
		return;

	if(IsGlobalFile() || IsGlobalRoot()) {
		String dir = SelectDirectory();
		if(dir.IsEmpty())
			return;
		
		Index<String> list;
		FindNodeFiles(globalTree.GetCursor(), list);
		ExportFiles(list, dir);
	}
}

void MacroManagerWindow::OnEditFile()
{
	const TreeCtrl& tree = GetCurrentTree();
	if(!tree.IsCursor())
		return;

	if(IsMacro()) {
		MacroElement element = ValueTo<MacroElement>(tree.Get());
		WhenEdit(element.fileName, element.line - 1);
		Break();
	}
	else if(IsFile()) {
		WhenEdit(tree.Get(), 1);
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
	
	globalTree.FindSetCursor(fileName);
}

void MacroManagerWindow::OnDeleteMacroFile()
{
	String fileName = static_cast<String>(globalTree.GetValue());
	if(!PromptOKCancel(t_("Are you sure you want to remove followin macro file \"" + fileName + "\"?")))
		return;
	
	FileDelete(AppendFileName(GetLocalDir(), fileName));
	globalTree.Remove(globalTree.GetCursor());
	OnTreeSel();
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
		
		int fileNode = globalTree.Add(globalNode, Image(), ff.GetPath(), fileTitle);
		
		auto list = UscFileParser(ff.GetPath()).Parse();
		for(const auto& element : list)
			globalTree.Add(fileNode, element.GetImage(), RawToValue(element), element.name);
	}
}

void MacroManagerWindow::LoadMacros()
{
	ReloadGlobalMacros();
	ReloadLocalMacros();
}

void MacroManagerWindow::ReloadGlobalMacros()
{
	globalTree.RemoveChildren(globalNode);

	LoadUscDir(GetLocalDir());
	LoadUscDir(GetFileFolder(ConfigFile("x")));
	
	globalTree.OpenDeep(0);
}

void MacroManagerWindow::ReloadLocalMacros()
{
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
			
			if(tab.GetCount() == 1)
				tab.Add(localTree.SizePos(), t_("Local macros (Read only)"));
			
			if(packageNode == -1)
				packageNode = localTree.Add(0, Image(), 0, wspc[i]);
					
			int fileNode = localTree.Add(packageNode, Image(), filePath, file);
			for(int j = 0; j < list.GetCount(); j++) {
				MacroElement& element = list[j];
				localTree.Add(fileNode, element.GetImage(), RawToValue(element), element.name);
			}
		}
	}

	localTree.OpenDeep(0);
}

}
