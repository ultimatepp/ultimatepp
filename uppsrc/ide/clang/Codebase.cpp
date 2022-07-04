#include "clang.h"

#define LLOG(x)

CXChildVisitResult current_file_visitor2( CXCursor cursor, CXCursor p, CXClientData clientData )
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
	
	CXCursor parent = clang_getCursorSemanticParent(cursor);
	CXCursorKind cursorKind = clang_getCursorKind(cursor);
	CXCursorKind parentKind = clang_getCursorKind(parent);

	unsigned int curLevel  = *( reinterpret_cast<unsigned int*>(clientData));
	unsigned int nextLevel = curLevel + 1;
	
	if(curLevel > 100000 && curLevel - 100000 > 2)
		return CXChildVisit_Continue;

	String name = GetCursorSpelling(cursor);
	String type = GetTypeSpelling(cursor);
	String display = FetchString(clang_getCursorDisplayName(cursor));
	String scope = GetTypeSpelling(parent);
	if(scope.GetCount())
		scope << "::";
	
	String m;

	auto Dump = [&] {
		#if 0
			LOG("=====================");
			DDUMP(GetCursorKindName(cursorKind));
			DDUMP(GetCursorSpelling(cursor));
			DDUMP(GetTypeSpelling(cursor));
			DDUMP(FetchString(clang_getCursorDisplayName(cursor)));
			DDUMP(clang_isCursorDefinition(cursor));
//			DDUMP(FetchString(clang_getCursorPrettyPrinted(cursor, NULL)));
			DDUMP(GetCursorKindName(parentKind));
			DDUMP(GetCursorSpelling(parent));
			DDUMP(GetTypeSpelling(parent));
//			DDUMP(location);
		#endif
	};

	bool valid = clang_Location_isFromMainFile(cxlocation);
	if(valid) {
		switch(cursorKind) {
		case CXCursor_FieldDecl:
		case CXCursor_VarDecl:
		case CXCursor_CXXMethod:
			m << scope << display;
			break;
		case CXCursor_StructDecl:
			m = type << "::struct";
			break;
		case CXCursor_UnionDecl:
			m = type << "::union";
			break;
		case CXCursor_ClassDecl:
			m = type << "::class";
			break;
		case CXCursor_FunctionDecl:
			m = display;
			break;
		case CXCursor_EnumDecl:
		case CXCursor_EnumConstantDecl:
//		case CXCursor_ParmDecl:
		case CXCursor_TypedefDecl:
		case CXCursor_Namespace:
		case CXCursor_Constructor:
		case CXCursor_Destructor:
		case CXCursor_ConversionFunction:
		case CXCursor_FunctionTemplate:
		case CXCursor_ClassTemplate:
		case CXCursor_UnexposedDecl:
//		case CXCursor_NamespaceAlias:
			break;
		default:
			valid = false;
			break;
		}
	}
	if(valid) {
		Dump();
		if(m.GetCount())
			LLOG(">> " << GetCursorKindName(cursorKind) << ": " << m);
	}
	clang_visitChildren(cursor, current_file_visitor2, &nextLevel);
	return CXChildVisit_Continue;
}

// TODO: Remove
void CurrentFileVisit(CXTranslationUnit tu)
{
	CXCursor rootCursor  = clang_getTranslationUnitCursor(tu);
	unsigned int treeLevel = 0;
	clang_visitChildren(rootCursor, current_file_visitor2, &treeLevel);
}
