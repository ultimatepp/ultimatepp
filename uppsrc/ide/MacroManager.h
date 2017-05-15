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

using MacroList = Array<MacroElement>;

class UscFileParser final {
public:
	UscFileParser(const String& filePath);
	
	MacroList Parse();
	
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
	MacroManagerWindow(Ide& ide);

	void Layout() override;

private:
	void InitButtons();
	
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

private:
	static String GenFileOverrideMessage(const String& fileName);
	
private:
	// TODO: MacroManager shold not depend upon Ide instance.
	// The edit logic should be outside class the same as load macros.
	Ide&          ide;
	
	TreeCtrl      macrosTree;
	SplitterFrame splitter;
	CodeEditor    editor;
};
