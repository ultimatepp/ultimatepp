#ifndef _clang_clang_h
#define _clang_clang_h

#include <ide/Common/Common.h>
#include <clang-c/Index.h>

using namespace Upp;

String FetchString(CXString cs);
String GetCursorKindName(CXCursorKind cursorKind);
String GetCursorSpelling(CXCursor cursor);

struct SourceLocation {
	int    line;
	int    column;
	int    offset;
	String filename;
	
	String ToString() const;

	SourceLocation(CXSourceLocation location);
};

void ClangFile(const String& filename, const String& content, const Vector<String>& includes, int line, int column);
String RedefineMacros();
CXTranslationUnit Clang(const String& cmdline, Vector<Tuple2<String, String>> file, unsigned options = 0);


struct AutoCompleteItem : Moveable<AutoCompleteItem> {
	String parent;
	String name;
	String signature;
	int    kind;
	int    priority;
};

struct CurrentFileContext {
	String                   filename;
	String                   includes;
	String                   content;
};

void StartCurrentFileParserThread();

String CleanupSignature(const String& signature);

void SetCurrentFile(const CurrentFileContext& ctx);
void StartAutoComplete(const CurrentFileContext& c, int line, int column, Event<const Vector<AutoCompleteItem>&> done);
void CancelAutoComplete();

#endif
