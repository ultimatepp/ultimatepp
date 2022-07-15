#ifndef _clang_clang_h
#define _clang_clang_h

#include <ide/Common/Common.h>
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

String RedefineMacros();
String GetClangInternalIncludes();

enum AdditionalKinds {
	KIND_INCLUDEFILE = -1000,
	KIND_INCLUDEFILE_ANY,
	KIND_INCLUDEFOLDER,
	KIND_COMPLETE,
};

Image CxxIcon(int kind); // TODO: Move here
String SignatureQtf(const String& name, const String& signature, int pari);

bool IsStruct(int kind);
bool IsTemplate(int kind);
bool IsFunction(int kind);
bool IsVariable(int kind);
int  FindId(const char *s, const String& id);

enum {
	ITEM_TEXT,
	ITEM_NAME,
	ITEM_CPP_TYPE,
	ITEM_CPP,
	ITEM_PNAME,
	ITEM_TNAME,
	ITEM_NUMBER,
	ITEM_SIGN,
	ITEM_UPP,
	ITEM_TYPE,
	
	ITEM_PTYPE = ITEM_TYPE + 10000,
};

struct ItemTextPart : Moveable<ItemTextPart> {
	int pos;
	int len;
	int type;
	int ii;
	int pari;
};

Vector<ItemTextPart> ParseSignature(const String& name, const String& signature, int *fn_info = NULL);

struct AutoCompleteItem : Moveable<AutoCompleteItem> {
	String parent;
	String name;
	String signature;
	int    kind;
	int    priority;
};

struct AnnotationItem : Moveable<AnnotationItem> {
	int    kind;
	int    line;
	bool   definition;
	String name;
	String id;
	String pretty;
	String nspace;
};

struct CurrentFileContext {
	String                   filename;
	String                   real_filename;
	String                   includes;
	String                   defines;
	String                   content;
};

struct Clang {
	CXIndex           index = nullptr;
	CXTranslationUnit tu = nullptr;

	void Dispose();
	bool Parse(const String& filename, const String& content, const String& includes, const String& defines, dword options);
	bool ReParse(const String& filename, const String& content);
	
	Clang();
	~Clang();
};

// void CurrentFileVisit(CXTranslationUnit tu);

void StartCurrentFileParserThread();
void StartAutoCompleteThread();

void DumpDiagnostics(CXTranslationUnit tu);

String CleanupId(const char *s);
String CleanupPretty(const String& signature);

bool   IsSourceFile(const String& path);

class ClangVisitor {
	bool initialized = false;
	CXPrintingPolicy pp_id, pp_pretty;
	
	CXCursor cursor;
	int      kind;
	String   name;
	String   id;
	String   type;
	String   scope;
	String   nspace;
	String   pretty;
	bool     annotation;
	int      line;
	int      column;

	bool ProcessNode(CXCursor c);

	friend CXChildVisitResult clang_visitor(CXCursor cursor, CXCursor p, CXClientData clientData);

public:
	virtual void Item() = 0;

	int    GetKind() const      { return kind; }
	String GetId() const        { return id; }
	String GetName() const      { return name; }
	String GetType() const      { return type; }
	String GetScope() const     { return scope; }
	String GetNamespace() const { return nspace; }
	String GetPretty() const    { return pretty; }
	bool   IsDefinition() const { return clang_isCursorDefinition(cursor); }
	bool   IsAnnotation() const { return annotation; }
	int    GetLine() const      { return line; }
	int    GetColumn() const    { return column; }

	void Do(CXTranslationUnit tu);
	~ClangVisitor();
};

void SetCurrentFile(const CurrentFileContext& ctx, Event<const Vector<AnnotationItem>&> done);
bool IsCurrentFileParsing();
void CancelCurrentFile();
bool IsCurrentFileDirty();

void SetAutoCompleteFile(const CurrentFileContext& ctx);
bool IsAutocompleteParsing();
void StartAutoComplete(const CurrentFileContext& ctx, int line, int column, bool macros,
                       Event<const Vector<AutoCompleteItem>&> done);
void CancelAutoComplete();

#endif
