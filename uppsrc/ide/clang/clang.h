#ifndef _clang_clang_h
#define _clang_clang_h

#include <Core/Core.h>
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

void ClangFile(const String& filename, const String& content, const Vector<String>& includes);

#endif
