#include "clang.h"

#ifdef DYNAMIC_LIBCLANG

#define DLLFILENAME "libclang.dll"
#define DLIMODULE   LibClang
#define DLIHEADER   <ide/clang/clang.dli>
#include <Core/dli.h>

bool   hasLibClang = false;
String LibClangPath;

bool LoadLibClang0(const char *path)
{
	hasLibClang = LibClang(path);
	if(hasLibClang) {
		LibClangPath = path;
		Logi() << UPP_FUNCTION_NAME << "(): libclang path: \"" << path << "\"";
	}
	return hasLibClang;
}

bool LoadLibClang(const String& dir)
{
#ifdef PLATFORM_MACOS // it does not seem to work for some reason, block it for now
	if(LoadLibClang0(dir + "/libclang.dylib"))
		return true;
#endif
	if(LoadLibClang0(dir + "/libclang.so"))
		return true;
	Vector<String> ps;
	for(FindFile ff(dir + "/libclang.so*"); ff; ff.Next())
		ps << ff.GetPath();
	Sort(ps, StdGreater<String>());
	for(String p : ps)
		if(LoadLibClang0(p))
			return true;
	return false;
}

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

unsigned int clang_CXXMethod_isStatic(CXCursor cursor)
{
	return LibClang().clang_CXXMethod_isStatic(cursor);
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

CXType clang_getTypedefDeclUnderlyingType(CXCursor C)
{
	return LibClang().clang_getTypedefDeclUnderlyingType(C);
}

CXCursor clang_getTypeDeclaration(CXType T)
{
	return LibClang().clang_getTypeDeclaration(T);
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

CXDiagnosticSet clang_getChildDiagnostics(CXDiagnostic d)
{
	return LibClang().clang_getChildDiagnostics(d);
}

unsigned clang_getNumDiagnosticsInSet(CXDiagnosticSet diags)
{
	return LibClang().clang_getNumDiagnosticsInSet(diags);
}

CXDiagnostic clang_getDiagnosticInSet(CXDiagnosticSet diags, unsigned index)
{
	return LibClang().clang_getDiagnosticInSet(diags, index);
}

enum CXDiagnosticSeverity clang_getDiagnosticSeverity(CXDiagnostic d)
{
	return LibClang().clang_getDiagnosticSeverity(d);
}

enum CX_StorageClass clang_Cursor_getStorageClass(CXCursor c)
{
	return LibClang().clang_Cursor_getStorageClass(c);
}

unsigned clang_getNumOverloadedDecls(CXCursor cursor)
{
	return LibClang().clang_getNumOverloadedDecls(cursor);
}

CXCursor clang_getOverloadedDecl(CXCursor cursor, unsigned index)
{
	return LibClang().clang_getOverloadedDecl(cursor, index);
}

#endif
