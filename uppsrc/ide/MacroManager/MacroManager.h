#ifndef _MacroManager_MacroManager_h_
#define _MacroManager_MacroManager_h_

#include <CtrlLib/CtrlLib.h>
#include <CodeEditor/CodeEditor.h>
#include <ide/Core/Core.h>

namespace Upp {

#define LAYOUTFILE <ide/MacroManager/MacroManager.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS MacroManagerImg
#define IMAGEFILE  <ide/MacroManager/MacroManager.iml>
#include <Draw/iml_header.h>

class MacroElement final {
public:
	enum class Type {
		MACRO,
		FUNCTION,
		UNKNOWN
	};

public:
	MacroElement(Type type, const String& fileName, int line, const String& comment);
	
	Image  GetImage() const;
	String GetContent() const;
	
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

class UscFileParser final {
public:
	using MacroList = Array<MacroElement>;
	
public:
	UscFileParser(const String& filePath);
	
	MacroList Parse();
	bool      IsValid();
	
private:
	void ReadFunction(CParser& parser, const String& comment, const char* prototypeBegin, MacroList& list);
	void ReadMacro(CParser& parser, const String& comment, const char* prototypeBegin, MacroList& list);
	void FinishRead(CParser& parser, const char* prototypeBegin, MacroElement& element, MacroList& list);

private:
	static void   FindNextElement(CParser& parser);
	static String ReadArgs(CParser& parser);
	static String ReadKeyDesc(CParser& parser);
	
private:
	String filePath;
};

class MacroManagerWindow final : public WithMacroManagerLayout<TopWindow> {
public:
	using MacroStore = ArrayMap<String, Array<MacroElement>>;
	
public:
	MacroManagerWindow(const Workspace& wspc, const String& hlStyles);

	void Layout() override;
	
public:
	Event<String, int> WhenEdit;

private:
	void InitButtons();
	
	void LoadUscDir(const String& dir);
	void LoadMacros();
	void ReloadGlobalMacros();
	void ReloadLocalMacros();

	void OnMacroBar(Bar& bar);
	void OnMacroSel();
	void OnImport();
	void OnExport();
	void OnEditFile();
	void OnNewMacroFile();
	void OnDeleteMacroFile();

	void ExportFiles(Index<String>& files, const String& dir);
	void FindNodeFiles(int id, Index<String>& list);

private:
	static String GenFileOverrideMessage(const String& fileName);

	bool IsGlobalRoot() const     { return macrosTree.GetCursor() == globalNode; }
	bool IsGlobalFile() const     { return macrosTree.Get().Is<String>(); }
	bool IsFile() const           { return macrosTree.Get().Is<String>() || macrosTree.Get().Is<WString>(); }
	bool IsFile(int id) const     { return macrosTree.Get(id).Is<String>() || macrosTree.Get(id).Is<WString>(); }
	bool IsMacro() const          { return macrosTree.Get().Is<MacroElement>();}
	bool IsEditPossible() const   { return IsFile() || IsMacro(); }
	
private:
	const Workspace& wspc;
	
	TreeCtrl         macrosTree;
	SplitterFrame    splitter;
	CodeEditor       editor;
	
	int              globalNode;
};

}

#endif
