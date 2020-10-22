#include "MacroManager.h"

#include <ide/Common/Common.h>

#define IMAGECLASS MacroManagerImg
#define IMAGEFILE  <ide/MacroManager/MacroManager.iml>
#include <Draw/iml_source.h>

#define METHOD_NAME "MacroManagerWindow::" << UPP_FUNCTION_NAME << "(): "

namespace Upp {

MacroManagerWindow::MacroManagerWindow(const Workspace& wspc, const String& hlStyles)
	: wspc(wspc)
	, globalMacrosChanged(false)
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

	LoadMacros();

	editor.Hide();
	editor.LoadHlStyles(hlStyles);
	
	InitButtons();
	InitEvents();
	InitToolBar();
}

void MacroManagerWindow::InitToolBar()
{
	InitToolButton(editLabel, t_("Edit"), MacroManagerImg::PluginEdit());
	InitToolButton(exportLabel, t_("Export current"), MacroManagerImg::PluginGo());
	tool.Separator();
	InitToolButton(newGlobalLabel, t_("New global"), IdeCommonImg::PageAdd());
	InitToolButton(importGlobalsLabel, t_("Import globals"), MacroManagerImg::PluginAdd());
	InitToolButton(exportGlobalsLabel, t_("Export globals"), MacroManagerImg::ArrowRight());
	
	newGlobalLabel.Enable();
	importGlobalsLabel.Enable();
	exportGlobalsLabel.Enable(globalTree.GetChildCount(0));
}

void MacroManagerWindow::InitToolButton(
	ToolButton& toolButton, const String& label, const Image& image)
{
	tool.Add(toolButton.Label(label));
	toolButton.Image(image);
	toolButton.SizePos();
	toolButton.Disable();
}

void MacroManagerWindow::InitButtons()
{
	close.Close();
	
	close              << [=] { Break(); };
	help               << [=] { LaunchWebBrowser("https://www.ultimatepp.org/app$ide$MacroManager_en-us.html"); };
	editLabel          << [=] { OnEditFile(); };
	exportLabel        << [=] { OnExport(globalTree.GetCursor()); };
	newGlobalLabel     << [=] { OnNewMacroFile(); };
	importGlobalsLabel << [=] { OnImport(); };
	exportGlobalsLabel << [=] { OnExport(0); };
	
	editLabel.Tip(t_("Edit currently selected macro inside TheIde.."));
	exportLabel.Tip(t_("Export selected macro file.."));
	newGlobalLabel.Tip(t_("Create new file that stores global macros.."));
	importGlobalsLabel.Tip(t_("Install file/files containing macros.."));
	exportGlobalsLabel.Tip(t_("Export all global macros files.."));
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
		bool partOfFile = IsGlobalFile();
		
		bar.Add(t_("New.."),    [=] { OnNewMacroFile(); });
		bar.Add(t_("Import.."), [=] { OnImport(); });
		bar.Add(t_("Delete"),   [=] { OnDeleteMacroFile(); })
		    .Enable(partOfFile);
		bar.Add(t_("Export.."), [=] { OnExport(globalTree.GetCursor()); })
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
	exportLabel.Enable(IsGlobalTab() && IsGlobalFile());
	
	editLabel.Enable(hasCursor && IsEditPossible());

	editor.Show(editLabel.IsEnabled());
	
	if(IsFile())
		editor.Set(LoadFile(static_cast<String>(tree.Get())));
	else if(IsMacro())
		editor.Set(ValueTo<MacroElement>(tree.Get()).GetContent());
}

void MacroManagerWindow::OnTabSet()
{
	exportLabel.Enable(tab.Get() == 0);
	editor.Hide();
	
	OnTreeSel();
}

void MacroManagerWindow::OnImport()
{
	auto filePath = SelectFileOpen("*.usc");
	if(IsNull(filePath))
		return;
	
	if(!UscFileParser(filePath).IsValid()) {
		ErrorOK(DeQtf(String(t_("Import failed! Following file")) << " \"" << filePath << "\" " << t_("is not a valid macro file!")));
		return;
	}
	
	auto localDir = GetLocalDir();
	if(!DirectoryExists(localDir) && !RealizeDirectory(localDir)) {
		ErrorOK(DeQtf(String(t_("Realizing directory")) << " \"" << localDir << "\" " << t_("failed.")));
		return;
	}
	
	auto newFileName = GetFileName(filePath);
	auto newFilePath = LocalPath(newFileName);
	if(FileExists(newFilePath) && !PromptYesNo(DeQtf(GenFileOverrideMessage(newFileName))))
		return;
	
	FileCopy(filePath, newFilePath);
	ReloadGlobalMacros();
	OnTreeSel();
	
	OnGlobalMacrosChanged();
}

void MacroManagerWindow::ExportFiles(Index<String>& files, const String& dir)
{
	for(const auto& file : files) {
		auto fileName = GetFileName(file);
		auto filePath = AppendFileName(dir, GetFileName(file));
		
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

void MacroManagerWindow::OnExport(int id)
{
	if(id == 0 || IsGlobalFile()) {
		auto dir = SelectDirectory();
		if(dir.IsEmpty())
			return;
		
		Index<String> list;
		FindNodeFiles(id, list);
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
	RealizeDirectory(GetLocalDir());
	if(FileExists(fullPath)) {
		PromptOK(String() << t_("file") << " \"" << fileName << "\" " << t_("already exists!"));
		return;
	}
	
	if(!SaveFile(fullPath, "macro \"" + GetFileTitle(fileName) + "\" {}")) {
		PromptOK(String() << t_("Error occured while saving file") << " \"" << fileName << "\"");
		return;
	}

	int fileNode = globalTree.Add(0, Image(), fullPath, fileName);
	auto list = UscFileParser(fullPath).Parse();
	for(const auto& element : list) {
		globalTree.Add(fileNode, element.GetImage(), RawToValue(element), element.name);
	}
	
	globalTree.OpenDeep(fileNode);
	globalTree.FindSetCursor(fullPath);
	
	OnGlobalMacrosChanged();
}

void MacroManagerWindow::OnDeleteMacroFile()
{
	auto fileName = static_cast<String>(globalTree.GetValue());
	if(!PromptOKCancel(String(t_("Are you sure you want to remove following macro file")) << " \"" << fileName << "\"?")) {
		return;
	}
	
	FileDelete(AppendFileName(GetLocalDir(), fileName));
	globalTree.Remove(globalTree.GetCursor());
	OnTreeSel();
	
	OnGlobalMacrosChanged();
}

void MacroManagerWindow::OnGlobalMacrosChanged()
{
	exportGlobalsLabel.Enable(globalTree.GetChildCount(0));
	globalMacrosChanged = true;
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
		
		int fileNode = globalTree.Add(0, Image(), ff.GetPath(), fileTitle);

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
	globalTree.Clear();

	LoadUscDir(GetLocalDir());
	LoadUscDir(GetFileFolder(ConfigFile("x")));
	
	globalTree.OpenDeep(0);
}

void MacroManagerWindow::ReloadLocalMacros()
{
	for(int i = 0; i < wspc.GetCount(); i++) {
		const auto& package = wspc.GetPackage(i);
		int packageNode = -1;
		for (const auto& file : package.file) {
			auto filePath = SourcePath(wspc[i], file);

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
				auto& macroElement = list[j];
				localTree.Add(fileNode, macroElement.GetImage(), RawToValue(macroElement), macroElement.name);
			}
		}
	}

	localTree.OpenDeep(0);
}

}
