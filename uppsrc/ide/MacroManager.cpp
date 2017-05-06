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
	, fileName(fileName)
	, line(line)
	, comment(comment)
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
}

#define METHOD_NAME "MacroManagerWindow " << UPP_FUNCTION_NAME << "(): "

class MacroManagerWindow final : public WithMacroManagerLayout<TopWindow> {
	using MacroStore = ArrayMap<String, Array<MacroElement>>;

public:
	MacroManagerWindow(Ide& ide);

	void Layout() override;

private:
	void InitButtons();
	
	void LoadUscFile(const String& fileName);
	void LoadUscDir(const String& dir);
	void LoadMacros();
	void ReloadMacros();

	void OnMacroSel();
	void OnImport();
	void OnExport();
	void OnEditFile();

	void ReadFunction(CParser& parser, const String& comment, const char* prototypeBegin);
	void ReadMacro(CParser& parser, const String& comment, const char* prototypeBegin);
	void FinishRead(CParser& parser, const char* prototypeBegin, MacroElement& element);

	MacroElement& ObtainElement(const Value& key);

private:
	static String ReadArgs(CParser& parser);
	static String ReadKeyDesc(CParser& parser);
	
private:
	// TODO: MacroManager shold not depend upon Ide instance.
	// The edit logic should be outside class the same as load macros.
	Ide&       ide;
	
	MacroStore macrosStore;
	TreeCtrl   macrosTree;
	// TODO:
	// - Macro should be organized in two tree roots - global and packages macros.
	// - The path of macro should not be shown in tree - it can be show in corresponding label.
	//   Only file title and package name (if macro belongs to package) should be show.
	
	CodeEditor editor;
};

MacroManagerWindow::MacroManagerWindow(Ide& ide)
	: ide(ide)
{
	CtrlLayout(*this, "Macro Manager");
	Zoomable().Sizeable().MinimizeBox(false);
	macrosTree.NoRoot();
	
	splitter.Horz(macrosTree, editor).SetPos(3500);

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
	if(IsNull(key)) {
		editor.Hide();
		editButton.Enable();
		exportButton.Enable();
		return;
	}

	MacroElement& element = ObtainElement(key);

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
		if(!PromptYesNo("Target file aready exists! Do you want to overwrite it?"))
			return;
	}

	FileCopy(filePath, newFilePath);

	LoadUscFile(newFilePath);
	ReloadMacros();
}

void MacroManagerWindow::OnExport()
{
	String dir = SelectDirectory();
	if(dir.IsEmpty())
		return;

	for(const String& filePath : macrosStore.GetKeys())
		FileCopy(filePath, AppendFileName(dir, GetFileName(filePath)));
}

void MacroManagerWindow::OnEditFile()
{
	if(!macrosTree.IsCursor())
		return;

	Value key = macrosTree.Get();
	if(IsNull(key)) {
		ide.EditFile(macrosTree.GetValue());
		ide.editor.SetCursor(ide.editor.GetPos(0));
		Break();
		return;
	}

	MacroElement& element = ObtainElement(key);

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

void MacroManagerWindow::ReadFunction(CParser& parser, const String& comment, const char* prototypeBegin)
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
	
	FinishRead(parser, prototypeBegin, fn);
}

void MacroManagerWindow::ReadMacro(CParser& parser, const String& comment, const char* prototypeBegin)
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

	FinishRead(parser, prototypeBegin, macro);
}

void MacroManagerWindow::FinishRead(CParser& parser, const char* prototypeBegin, MacroElement& element)
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

	macrosStore.GetAdd(element.fileName).Add(element);
}

MacroElement& MacroManagerWindow::ObtainElement(const Value& key)
{
	ASSERT(!key.IsNull());
	
	Point pos = static_cast<Point>(key);
	return macrosStore[pos.x][pos.y];
}

void MacroManagerWindow::LoadUscFile(const String& fileName)
{
	String fileContent = LoadFile(fileName);
	if(fileContent.IsEmpty()) {
		Logw() << METHOD_NAME << "Following file \"" << fileName << "\" doesn't exist or is empty.";
		return;
	}

	try {
		CParser parser(fileContent, fileName);
		
		while (!parser.IsEof()) {
			String comment = TrimLeft(String(parser.GetSpacePtr(), parser.GetPtr()));

			if (!parser.IsId())
				return;
			
			const char* prototypeBegin = parser.GetPtr();
			String id = parser.ReadId();
			
			if(id.IsEqual("fn"))
				ReadFunction(parser, comment, prototypeBegin);
			else
			if(id.IsEqual("macro"))
				ReadMacro(parser, comment, prototypeBegin);
			else
				return;
		}
	}
	catch (const CParser::Error& error) {
		Logw() << METHOD_NAME << "Parsing file \"" << fileName << "\" failed with error: " << error << ".";
	}
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

void MacroManagerWindow::LoadUscDir(const String& dir)
{
	for(FindFile ff(AppendFileName(dir, "*.usc")); ff; ff.Next())
		LoadUscFile(AppendFileName(dir, ff.GetName()));
}

void MacroManagerWindow::LoadMacros()
{
	const Workspace& wspc = ide.IdeWorkspace();

	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& package = wspc.GetPackage(i);
		
		for (const auto& file : package.file) {
			String filePath = SourcePath(wspc[i], file);

			if (ToLower(GetFileExt(filePath)) == ".usc")
				LoadUscFile(filePath);
		}
	}

	LoadUscDir(GetLocalDir());
	LoadUscDir(GetFileFolder(ConfigFile("x")));

	ReloadMacros();
}

void MacroManagerWindow::ReloadMacros()
{
	macrosTree.Clear();

	for(int i = 0; i < macrosStore.GetCount(); i++) {
		int id = macrosTree.Add(0, Image(), Null, macrosStore.GetKey(i));
		
		for(int j = 0; j < macrosStore[i].GetCount(); j++) {
			MacroElement& element = macrosStore[i][j];
			
			macrosTree.Add(id, MacroElement::GetImage(element.type), Point(i, j), element.name);
		}
	}
}

#undef METHOD_NAME

void Ide::DoMacroManager()
{
	MacroManagerWindow(*this).Execute();
}
