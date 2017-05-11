#include "ide.h"

class MacroElement {
public:
	enum class Type {
		MACRO,
		FUNCTION,
		UNKNOWN
	};

public:
	MacroElement(Type type, const String& fileName, int line, const String& comment);
	
	static Image GetImage(Type type);
	
public:
	Type   type;
	String comment;
	String name;
	String prototype;
	String args;
	String code;
	String fileName;
	int    line;
};

MacroElement::MacroElement(Type type, const String& fileName, int line, const String& comment)
	: type(type)
	, comment(comment)
	, fileName(fileName)
	, line(line)
{}

Image MacroElement::GetImage(Type type)
{
	switch(type)
	{
		case(Type::MACRO):
			return IdeImg::Macro();
		case(Type::FUNCTION):
			return IdeImg::Fn();
		case(Type::UNKNOWN):
			return Image();
	}
	return Image();
}

#define METHOD_NAME "MacroManagerWindow " << UPP_FUNCTION_NAME << "(): "

using MacroList = Array<MacroElement>;

class MacroManagerWindow final : public WithMacroManagerLayout<TopWindow> {
	using MacroStore = ArrayMap<String, Array<MacroElement>>;
	
public:
	MacroManagerWindow(Ide& ide);

	void Layout() override;

private:
	void InitButtons();
	
	MacroList LoadUscFile(const String& fileName);
	void LoadUscDir(const String& dir, MacroList& store);
	void LoadMacros();
	void ReloadGlobalMacros();
	void ReloadLocalMacros();

	void OnMacroSel();
	void OnImport();
	void OnExport();
	void OnEditFile();

	void ExportFiles(Index<String>& files, const String& dir);
	void FindNodeFiles(int id, Index<String>& list);

	void ReadFunction(CParser& parser, const String& comment, const char* prototypeBegin, MacroList& list);
	void ReadMacro(CParser& parser, const String& comment, const char* prototypeBegin, MacroList& list);
	void FinishRead(CParser& parser, const char* prototypeBegin, MacroElement& element, MacroList& list);

private:
	static String ReadArgs(CParser& parser);
	static String ReadKeyDesc(CParser& parser);
	
private:
	// TODO: MacroManager shold not depend upon Ide instance.
	// The edit logic should be outside class the same as load macros.
	Ide&          ide;
	
	TreeCtrl      macrosTree;
	SplitterFrame splitter;
	
	CodeEditor    editor;
};

MacroManagerWindow::MacroManagerWindow(Ide& ide)
	: ide(ide)
{
	CtrlLayout(*this, "Macro Manager");
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
		exportButton.Enable();
		return;
	}

	MacroElement element = ValueTo<MacroElement>(key);

	String macroContent = TrimBoth(element.comment);
	if(macroContent.GetCount())
		macroContent << "\n";

	macroContent << element.prototype <<  element.code;

	editButton.Enable();
	exportButton.Enable();
	editor.Show();
	editor.Set(macroContent);
}

void MacroManagerWindow::OnImport()
{
	String filePath = SelectFileOpen("*.usc");

	if(IsNull(filePath))
		return;

	String newFilePath = AppendFileName(GetLocalDir(), GetFileName(filePath));

	RealizeDirectory(GetLocalDir());
	if(!DirectoryExists(GetLocalDir())) {
		Loge() << METHOD_NAME << "Realize directory \"" << GetLocalDir() << "\" failed.";
		return;
	}
	
	if(FileExists(newFilePath)) {
		if(!PromptYesNo(DeQtf(newFilePath << " : Target file aready exists! Do you want to overwrite it?")))
			return;
	}

	FileCopy(filePath, newFilePath);
	ReloadGlobalMacros();
	OnMacroSel();
}

void MacroManagerWindow::ExportFiles(Index<String>& files, const String& dir)
{
	for(const String& file : files)
	{
		String newFilePath = AppendFileName(dir, GetFileName(file));
		if(FileExists(newFilePath)) {
			if(!PromptYesNo(DeQtf(newFilePath << " : Target file aready exists! Do you want to overwrite it?")))
				continue;
		}
		
		FileCopy(file, newFilePath);
	}
}

void MacroManagerWindow::FindNodeFiles(int id, Index<String>& list)
{
	Value key = macrosTree.Get(id);
	if(IsNumber(key)) {
		for(int i = 0; i < macrosTree.GetChildCount(id); i++)
		{
			int n = macrosTree.GetChild(id, i);
			Value k = macrosTree.Get(n);
			FindNodeFiles(n, list);
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
	else {
		String fileName = ValueTo<MacroElement>(key).fileName;
		String newFilePath = AppendFileName(dir, GetFileName(fileName));
		
		if(FileExists(newFilePath)) {
			if(!PromptYesNo(DeQtf(newFilePath << " : Target file aready exists! Do you want to overwrite it?")))
				return;
		}
		
		FileCopy(fileName, newFilePath);
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

static void FindNext(CParser& parser)
{
	while(!parser.IsEof() && !parser.IsId("fn") && !parser.IsId("macro"))
		parser.SkipTerm();
}

void MacroManagerWindow::ReadFunction(CParser& parser, const String& comment, const char* prototypeBegin, MacroList& list)
{
	String fileName = parser.GetFileName();
	MacroElement fn(MacroElement::Type::FUNCTION, fileName, parser.GetLine(), comment);
	
	if(!parser.IsId()) {
		FindNext(parser);
		return;
	}
	fn.name = parser.ReadId();
	if(!parser.Char('(')) {
		FindNext(parser);
		return;
	}
	fn.args = ReadArgs(parser);
	
	FinishRead(parser, prototypeBegin, fn, list);
}

void MacroManagerWindow::ReadMacro(CParser& parser, const String& comment, const char* prototypeBegin, MacroList& list)
{
	String fileName = parser.GetFileName();
	MacroElement macro(MacroElement::Type::MACRO, fileName, parser.GetLine(), comment);
	
	if(!parser.IsString()) {
		FindNext(parser);
		return;
	}
	macro.name = String() << (parser.IsString() ? parser.ReadString() : "");
	if(parser.Char(':')) {
		if(!parser.IsString()) {
			FindNext(parser);
			return;
		}

		macro.name << " : " << (parser.IsString() ? parser.ReadString() : "");
	}
	if (!parser.IsChar('{'))
		ReadKeyDesc(parser);

	FinishRead(parser, prototypeBegin, macro, list);
}

void MacroManagerWindow::FinishRead(CParser& parser, const char* prototypeBegin, MacroElement& element, MacroList& list)
{
	const char* bodyBegin = parser.GetPtr();

	element.prototype = String(prototypeBegin, bodyBegin);
	
	if (!parser.Char('{')) {
		FindNext(parser);
		return;
	}

	Upp::SkipBlock(parser);

	if(parser.GetSpacePtr() > bodyBegin)
		element.code = String(bodyBegin, parser.GetSpacePtr());

	list.Add(element);
}

MacroList MacroManagerWindow::LoadUscFile(const String& fileName)
{
	MacroList ret;
	String fileContent = LoadFile(fileName);
	if(fileContent.IsEmpty()) {
		Logw() << METHOD_NAME << "Following file \"" << fileName << "\" doesn't exist or is empty.";
		return ret;
	}

	try {
		CParser parser(fileContent, fileName);
		
		while (!parser.IsEof()) {
			String comment = TrimLeft(String(parser.GetSpacePtr(), parser.GetPtr()));

			if (!parser.IsId())
				return ret;
			
			const char* prototypeBegin = parser.GetPtr();
			String id = parser.ReadId();
			
			if(id.IsEqual("fn"))
				ReadFunction(parser, comment, prototypeBegin, ret);
			else
			if(id.IsEqual("macro"))
				ReadMacro(parser, comment, prototypeBegin, ret);
			else
				return ret;
		}
	}
	catch (const CParser::Error& error) {
		Logw() << METHOD_NAME << "Parsing file \"" << fileName << "\" failed with error: " << error << ".";
	}
	
	return ret;
}

String MacroManagerWindow::ReadArgs(CParser& parser)
{
	int level = 1;
	parser.Char('(');
	
	String ret = "(";
	while(level > 0 && !parser.IsEof()) {
		if (parser.Char('('))
			level++;
		else
		if (parser.Char(')'))
			level--;
		else
			ret << parser.GetChar();
	}
	ret << ")";

	return ret;
}

String MacroManagerWindow::ReadKeyDesc(CParser& parser)
{
	if(!parser.IsId())
		return String();

	String ret = parser.ReadId();

	while(!parser.IsEof() && parser.Char('+')) {
		if(parser.IsId())
			ret << "+" << parser.ReadId();

		else
		if(parser.IsNumber())
			ret << "+" << parser.ReadInt();
		else
			break;
	}

	return ret;
}

void MacroManagerWindow::LoadUscDir(const String& dir, MacroList& store)
{
	for(FindFile ff(AppendFileName(dir, "*.usc")); ff; ff.Next())
	{
		MacroList list = LoadUscFile(AppendFileName(dir, ff.GetName()));
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
		if(!file.IsEqual(element.fileName))
		{
			file = element.fileName;
			fileNode = macrosTree.Add(globalNode, Image(), 3, file.Mid(configFolderNameLength));
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
				
			MacroList list = LoadUscFile(filePath);
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

#undef METHOD_NAME

void Ide::DoMacroManager()
{
	MacroManagerWindow(*this).Execute();
}
