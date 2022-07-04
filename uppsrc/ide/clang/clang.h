#ifndef _clang_clang_h
#define _clang_clang_h

#include <ide/Common/Common.h>
#include <ide/Browser/Browser.h>
#include <clang-c/Index.h>

using namespace Upp;

String FetchString(CXString cs);
String GetCursorKindName(CXCursorKind cursorKind);
String GetCursorSpelling(CXCursor cursor);
String GetTypeSpelling(CXCursor cursor);

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

Image CxxIcon(int kind);
String SignatureQtf(const String& name, const String& signature, int pari);
Vector<ItemTextPart> ParseSignature(const String& name, const String& signature, int *fn_info = NULL);

struct AutoCompleteItem : Moveable<AutoCompleteItem> {
	String parent;
	String name;
	String signature;
	int    kind;
	int    priority;
};

struct AnnotationItem : Moveable<AnnotationItem> {
	int    line;
	String item;
	String pretty;
};

struct CurrentFileContext {
	String                   filename;
	String                   includes;
	String                   content;
};

// void CurrentFileVisit(CXTranslationUnit tu);

void StartCurrentFileParserThread();

void DumpDiagnostics(CXTranslationUnit tu);

String CleanupSignature(const String& signature);
bool   IsSourceFile(const String& path);

void SetCurrentFile(const CurrentFileContext& ctx, Event<const Vector<AnnotationItem>&> done);
void StartAutoComplete(const CurrentFileContext& ctx, int line, int column, bool macros,
                       Event<const Vector<AutoCompleteItem>&> done);
void CancelAutoComplete();
void CancelCurrentFile();

#endif
