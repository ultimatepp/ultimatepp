// TODO: Move to separate package...

class MacroElement {
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

using MacroList = Array<MacroElement>;

class UscFileParser final {
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
	MacroManagerWindow(const Workspace& wspc);

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
