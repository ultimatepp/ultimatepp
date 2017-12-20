class PythonSyntax : public EditorSyntax {
private:
	struct Identation {
		enum Type {
			Tab = 0,
			Space,
			Unknown
		};
	};

public:
	PythonSyntax();

	virtual void Highlight(const wchar *start, const wchar *end, HighlightOutput& hls,
	                       CodeEditor *editor, int line, int64 pos);
	virtual void IndentInsert(CodeEditor& e, int chr, int count);
	
private:
	bool IsSeparator(const wchar* c);

	bool IsKeyword(const WString& w);
	bool IsSpecialVar(const WString& w);
	bool IsNumber(const WString& w);
	
	void InitKeywords();
	void LoadSyntax(const char* keywordsArray[], const char* specialVarsArray[]);
	
	bool             LineHasColon(const WString& line);
	int              CalculateLineIndetations(const WString& line, Identation::Type type);
	int              CalculateSpaceIndetationSize(CodeEditor& editor);
	Identation::Type FindIdentationType(CodeEditor& editor, const WString& line);
	char             GetIdentationByType(Identation::Type type);
	
private:
	Index<String> keywords;
	Index<String> specialVars;
};
