#include "clang.h"

#define LLOG(x)
#define LTIMESTOP(x) TIMESTOP(x)

#ifdef DYNAMIC_LIBCLANG
#define DLLFILENAME "libclang.dll"
#define DLIMODULE   LibClang
#define DLIHEADER   <ide/clang/clang.dli>
#include <Core/dli.h>

enum CXCursorKind clang_getCursorKind(CXCursor cursor)
{
	return LibClang().clang_getCursorKind(cursor);
}

CXCursor clang_getCursorSemanticParent(CXCursor cursor)
{
	return LibClang().clang_getCursorSemanticParent(cursor);
}

CXString clang_getCursorPrettyPrinted(CXCursor cursor, CXPrintingPolicy policy)
{
	return LibClang().clang_getCursorPrettyPrinted(cursor, policy);
}

const char *clang_getCString(CXString string)
{
	return LibClang().clang_getCString(string);
}

void clang_disposeString(CXString string)
{
	LibClang().clang_disposeString(string);
}

CXString clang_getCursorKindSpelling(enum CXCursorKind kind)
{
	return LibClang().clang_getCursorKindSpelling(kind);
}

CXString clang_getCursorSpelling(CXCursor cursor)
{
	return LibClang().clang_getCursorSpelling(cursor);
}

CXType clang_getCursorType(CXCursor cursor)
{
	return LibClang().clang_getCursorType(cursor);
}

CXString clang_getTypeSpelling(CXType type)
{
	return LibClang().clang_getTypeSpelling(type);
}

CXIndex clang_createIndex(int excludeDeclarationsFromPCH, int displayDiagnostics)
{
	return LibClang().clang_createIndex(excludeDeclarationsFromPCH, displayDiagnostics);
}

void clang_disposeIndex(CXIndex index)
{
	LibClang().clang_disposeIndex(index);
}

unsigned int clang_visitChildren(CXCursor parent, CXCursorVisitor visitor, CXClientData client_data)
{
	return LibClang().clang_visitChildren(parent, visitor, client_data);
}

void clang_getExpansionLocation(CXSourceLocation location, CXFile *file, unsigned int *line, unsigned int *column, unsigned int *offset)
{
	LibClang().clang_getExpansionLocation(location, file, line, column, offset);
}

CXString clang_getFileName(CXFile SFile)
{
	return LibClang().clang_getFileName(SFile);
}

CXSourceLocation clang_getCursorLocation(CXCursor cursor)
{
	return LibClang().clang_getCursorLocation(cursor);
}

CXCursor clang_getCursorReferenced(CXCursor cursor)
{
	return LibClang().clang_getCursorReferenced(cursor);
}

unsigned int clang_isCursorDefinition(CXCursor cursor)
{
	return LibClang().clang_isCursorDefinition(cursor);
}

unsigned int clang_CXXMethod_isVirtual(CXCursor cursor)
{
	return LibClang().clang_CXXMethod_isVirtual(cursor);
}

int clang_Cursor_isNull(CXCursor cursor)
{
	return LibClang().clang_Cursor_isNull(cursor);
}

CXCursor clang_getTranslationUnitCursor(CXTranslationUnit tu)
{
	return LibClang().clang_getTranslationUnitCursor(tu);
}

CXPrintingPolicy clang_getCursorPrintingPolicy(CXCursor cursor)
{
	return LibClang().clang_getCursorPrintingPolicy(cursor);
}

void clang_PrintingPolicy_setProperty(CXPrintingPolicy Policy, enum CXPrintingPolicyProperty Property, unsigned int Value)
{
	return LibClang().clang_PrintingPolicy_setProperty(Policy, Property, Value);
}

void clang_PrintingPolicy_dispose(CXPrintingPolicy Policy)
{
	LibClang().clang_PrintingPolicy_dispose(Policy);
}

unsigned int clang_getNumCompletionChunks(CXCompletionString completion_string)
{
	return LibClang().clang_getNumCompletionChunks(completion_string);
}

void clang_disposeTranslationUnit(CXTranslationUnit tu)
{
	LibClang().clang_disposeTranslationUnit(tu);
}

CXTranslationUnit clang_parseTranslationUnit(CXIndex CIdx, const char *source_filename, const char *const *command_line_args, int num_command_line_args, CXUnsavedFile *unsaved_files,
                                             unsigned int num_unsaved_files, unsigned int options)
{
	return LibClang().clang_parseTranslationUnit(CIdx, source_filename, command_line_args, num_command_line_args, unsaved_files,
                                              num_unsaved_files, options);
}

int clang_reparseTranslationUnit(CXTranslationUnit TU, unsigned int num_unsaved_files, CXUnsavedFile *unsaved_files, unsigned int options)
{
	return LibClang().clang_reparseTranslationUnit(TU, num_unsaved_files, unsaved_files, options);
}

unsigned int clang_getNumDiagnostics(CXTranslationUnit tu)
{
	return LibClang().clang_getNumDiagnostics(tu);
}

CXDiagnostic clang_getDiagnostic(CXTranslationUnit tu, unsigned int i)
{
	return LibClang().clang_getDiagnostic(tu, i);
}

CXSourceLocation clang_getDiagnosticLocation(CXDiagnostic d)
{
	return LibClang().clang_getDiagnosticLocation(d);
}

CXString clang_getDiagnosticSpelling(CXDiagnostic d)
{
	return LibClang().clang_getDiagnosticSpelling(d);
}

void clang_disposeDiagnostic(CXDiagnostic d)
{
	LibClang().clang_disposeDiagnostic(d);
}

enum CXCompletionChunkKind clang_getCompletionChunkKind(CXCompletionString completion_string, unsigned int chunk_number)
{
	return LibClang().clang_getCompletionChunkKind(completion_string, chunk_number);
}

CXString clang_getCompletionChunkText(CXCompletionString completion_string, unsigned int chunk_number)
{
	return LibClang().clang_getCompletionChunkText(completion_string, chunk_number);
}

CXCompletionString clang_getCompletionChunkCompletionString(CXCompletionString completion_string, unsigned int chunk_number)
{
	return LibClang().clang_getCompletionChunkCompletionString(completion_string, chunk_number);
}

CXString clang_getCompletionParent(CXCompletionString completion_string, enum CXCursorKind *kind)
{
	return LibClang().clang_getCompletionParent(completion_string, kind);
}

unsigned int clang_getCompletionPriority(CXCompletionString completion_string)
{
	return LibClang().clang_getCompletionPriority(completion_string);
}

CXCodeCompleteResults *clang_codeCompleteAt(CXTranslationUnit tu, const char *complete_filename,
                                            unsigned int complete_line, unsigned int complete_column,
                                            CXUnsavedFile *unsaved_files, unsigned int num_unsaved_files,
                                            unsigned int options)
{
	return LibClang().clang_codeCompleteAt(tu, complete_filename, complete_line, complete_column,
	                                       unsaved_files, num_unsaved_files, options);
}

void clang_disposeCodeCompleteResults(CXCodeCompleteResults *Results)
{
	LibClang().clang_disposeCodeCompleteResults(Results);
}

void clang_CXIndex_setGlobalOptions(CXIndex index, unsigned int options)
{
	LibClang().clang_CXIndex_setGlobalOptions(index, options);
}

#endif

String FetchString(CXString cs)
{
	String result = clang_getCString(cs);
	clang_disposeString(cs);
	return result;
}

String GetCursorKindName(CXCursorKind cursorKind)
{
	return FetchString(clang_getCursorKindSpelling(cursorKind));
}

String GetCursorSpelling(CXCursor cursor)
{
	return FetchString(clang_getCursorSpelling(cursor));
}

String GetTypeSpelling(CXCursor cursor)
{
	return FetchString(clang_getTypeSpelling(clang_getCursorType(cursor)));
}

String GetClangInternalIncludes()
{
	static String includes;
	ONCELOCK {
		String dummy = ConfigFile("dummy.cpp");
		Upp::SaveFile(dummy, String());
		String h = Sys(
		#ifdef PLATFORM_WIN32
				GetExeDirFile("bin/clang/bin/c++") +
		#else
				"clang++"
		#endif
				" -v -x c++ -E " + dummy
		);
		DeleteFile(dummy);
		h.Replace("\r", "");
		Vector<String> ln = Split(h, '\n');
		for(int i = 0; i < ln.GetCount(); i++) {
			String dir = TrimBoth(ln[i]);
			if(DirectoryExists(dir))
				MergeWith(includes, ";", NormalizePath(dir));
		}
	}
	return includes;
}

void Clang::Dispose()
{
	if(tu) {
		INTERLOCKED { // Otherwise dispose takes much longer, probably due to clang allocator lock contention
//			TIMESTOP("clang_disposeTranslationUnit");
			clang_disposeTranslationUnit(tu);
		}
	}
	tu = nullptr;
}

bool Clang::Parse(const String& filename, const String& content, const String& includes_, const String& defines, dword options)
{
	MemoryIgnoreLeaksBlock __;
	if(!index) return false;
	
//	LTIMESTOP("Parse " << filename << " " << includes_ << " " << defines);
	
	Dispose();

	String cmdline;

	cmdline << filename << " -DflagDEBUG -DflagDEBUG_FULL -DflagMAIN -xc++ -std=c++17 ";
	
//	cmdline << RedefineMacros();
	
	String includes = includes_;
	MergeWith(includes, ";", GetClangInternalIncludes());

	Vector<String> args;
	for(const String& s : Split(includes, ';'))
		args.Add("-I" + s);

	for(const String& s : Split(defines + ";CLANG", ';'))
		args.Add("-D" + s);

	args.Append(Split(cmdline, ' '));
	
	Vector<const char *> argv;

	for(const String& s : args)
		argv.Add(~s);

	CXUnsavedFile ufile = { ~filename, ~content, (unsigned)content.GetCount() };
	tu = clang_parseTranslationUnit(index, nullptr, argv, argv.GetCount(),
	                                options & PARSE_FILE ? nullptr : &ufile,
	                                options & PARSE_FILE ? 0 : 1,
	                                options);

//	DumpDiagnostics(tu);
	
	return tu;
}

bool Clang::ReParse(const String& filename, const String& content)
{
	MemoryIgnoreLeaksBlock __;
	CXUnsavedFile ufile = { ~filename, ~content, (unsigned)content.GetCount() };
	if(clang_reparseTranslationUnit(tu, 1, &ufile, 0)) {
		Dispose();
		return false;
	}
	return true;
}

Clang::Clang()
{
	MemoryIgnoreLeaksBlock __;
	index = clang_createIndex(0, 0);
}

Clang::~Clang()
{
	MemoryIgnoreLeaksBlock __;
	Dispose();
	clang_disposeIndex(index);
}

void DumpDiagnostics(CXTranslationUnit tu)
{
	size_t num_diagnostics = clang_getNumDiagnostics(tu);

	for (size_t i = 0; i < num_diagnostics; ++i) {
		CXDiagnostic diagnostic = clang_getDiagnostic(tu, i);
		auto Dump = [&](CXDiagnostic diagnostic) {
			CXFile file;
			unsigned line;
			unsigned column;
			unsigned offset;
			CXSourceLocation location = clang_getDiagnosticLocation(diagnostic);
			clang_getExpansionLocation(location, &file, &line, &column, &offset);
			LOG(FetchString(clang_getFileName(file)) << " (" << line << ":" << column << ") " <<
				FetchString(clang_getDiagnosticSpelling(diagnostic)));
		};
		Dump(diagnostic);
	#if 0
		CXDiagnosticSet set = clang_getChildDiagnostics(diagnostic);
		int n = clang_getNumDiagnosticsInSet(set);
		for(int i = 0; i < n; i++) {
			CXDiagnostic d = clang_getDiagnosticInSet(set, i);
			Dump(d);
			clang_disposeDiagnostic(d);
		}
	#endif
		clang_disposeDiagnostic(diagnostic);
	}
}