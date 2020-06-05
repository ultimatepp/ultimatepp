class PythonSyntax : public EditorSyntax {
public:
	virtual void Highlight(const wchar *start, const wchar *end, HighlightOutput& hls,
	                       CodeEditor *editor, int line, int64 pos);
	virtual void IndentInsert(CodeEditor& e, int chr, int count);

private:
	struct Identation {
		enum Type {
			Tab = 0,
			Space,
			Unknown
		};
	};

	bool             LineHasColon(const WString& line);
	int              CalculateLineIndetations(const WString& line, Identation::Type type);
	int              CalculateSpaceIndetationSize(CodeEditor& editor);
	Identation::Type FindIdentationType(CodeEditor& editor, const WString& line);
	char             GetIdentationByType(Identation::Type type);
};
