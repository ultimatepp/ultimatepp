#include "clang.h"

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

String SourceLocation::ToString() const
{
	return String() << filename << " (" << line << ":" << column << ")";
}

SourceLocation::SourceLocation(CXSourceLocation location)
{
	CXFile file;
	unsigned line_;
	unsigned column_;
	unsigned offset_;
	clang_getExpansionLocation(location, &file, &line_, &column_, &offset_);
	line = line_;
	column = column_;
	offset = offset_;
	filename = FetchString(clang_getFileName(file));
}

CXChildVisitResult visitor( CXCursor cursor, CXCursor /* parent */, CXClientData clientData )
{
#if 0
	static Index<unsigned> visited;
	unsigned h = clang_hashCursor(cursor);
	if(visited.Find(h) >= 0)
		return CXChildVisit_Continue;
	visited.Add(h);
#endif
	CXSourceLocation cxlocation = clang_getCursorLocation( cursor );
	SourceLocation location = cxlocation;
//	if( clang_Location_isFromMainFile( location ) == 0 )
//		return CXChildVisit_Continue;
	
	CXCursorKind cursorKind = clang_getCursorKind( cursor );

	unsigned int curLevel  = *( reinterpret_cast<unsigned int*>(clientData));
	unsigned int nextLevel = curLevel + 1;
	
	if(curLevel > 100000 && curLevel - 100000 > 2)
		return CXChildVisit_Continue;

    if (1 || findarg(cursorKind, CXCursorKind::CXCursor_FunctionDecl, CXCursorKind::CXCursor_CXXMethod,
                            CXCursorKind::CXCursor_FunctionTemplate, CXCursorKind::CXCursor_Constructor) >= 0
        || clang_Location_isFromMainFile(cxlocation)) {

		String indent = curLevel > 100000 ? String('=', curLevel - 100000) : String('-', curLevel);
		LOG(indent << " " << GetCursorKindName(cursorKind)
		    << " '" << GetCursorSpelling(cursor) << "' "
			<< " '" << GetTypeSpelling(cursor) << "' " << location);
		String h = FetchString(clang_getCursorUSR(cursor));
		if(h.GetCount())
			LOG(indent << "  USR: " << h);
		CXCursor c2 = clang_getCursorReferenced(cursor);
		if(!clang_Cursor_isNull(c2)) {
			LOG(indent << "Referenced: " << GetCursorSpelling(c2) << ' ' << GetCursorKindName(clang_getCursorKind(c2)));
			String h = FetchString(clang_getCursorUSR(c2));
			if(h.GetCount())
				LOG(indent << "->USR: " << h);
//			int h = nextLevel < 100000 ? nextLevel + 100000 : nextLevel;
//			clang_visitChildren(c2, visitor, &h);
		}
		clang_visitChildren(cursor, visitor, &nextLevel);
    }

	return CXChildVisit_Continue;
}

void ClangFile(const String& filename, const String& content, const Vector<String>& includes)
{
	String cmdline = "c++ " + filename + " -DflagDEBUG -DflagDEBUG_FULL -DflagBLITZ -DflagWIN32 -DflagNOLIB -D_DEBUG -x c++";
	for(String s : includes)
		cmdline << " -I" << s;
	
	DDUMP(cmdline);
	
	Vector<String> sa = Split(cmdline, ' ');
	
	Buffer<const char *> argv(sa.GetCount());
	int i = 0;
	for(const String& s : sa)
		argv[i++] = ~s;
	
    CXIndex index = clang_createIndex(0, 0);
    if (!index)
        Panic("createIndex failed");

    unsigned options = clang_defaultEditingTranslationUnitOptions();
    
    DDUMP(options);

	CXTranslationUnit tu;
	{
		TIMING("Parse");
		CXUnsavedFile file = { ~filename, ~content, (unsigned)content.GetCount() };
		tu = clang_parseTranslationUnit(index, nullptr, argv, sa.GetCount(), nullptr, 0, options);
	
	    if (!tu) {
	        Panic("clang_parseTranslationUnit2FullArgv failed");
	    }
	}

	if(0) {
		TIMING("Reparse");
	    int failure = clang_reparseTranslationUnit(tu, 0, nullptr, options);
	
	    if (failure)
	        Panic("Reparse failed");
	}

    size_t num_diagnostics = clang_getNumDiagnostics(tu);

    for (size_t i = 0; i < num_diagnostics; ++i) {
        CXFile file;
        unsigned line;
        unsigned column;
        unsigned offset;
        CXDiagnostic diagnostic = clang_getDiagnostic(tu, i);
        CXSourceLocation location = clang_getDiagnosticLocation(diagnostic);
        clang_getExpansionLocation(location, &file, &line, &column, &offset);
        DLOG(FetchString(clang_getFileName(file)) <<
             " (" << line << ":" << column << ") " <<
             FetchString(clang_getDiagnosticSpelling(diagnostic)));
        clang_disposeDiagnostic(diagnostic);
    }
    
	CXCursor rootCursor  = clang_getTranslationUnitCursor(tu);
	unsigned int treeLevel = 0;
	clang_visitChildren(rootCursor, visitor, &treeLevel);
}
