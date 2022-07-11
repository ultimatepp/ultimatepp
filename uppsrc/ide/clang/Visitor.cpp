#include "clang.h"

bool ClangVisitor::ProcessNode(CXCursor c)
{
	cursor = c;
	CXSourceLocation cxlocation = clang_getCursorLocation(cursor);

	if(!clang_Location_isFromMainFile(cxlocation))
		return false;

	String m;

	CXCursor parent = clang_getCursorSemanticParent(cursor);
	CXCursorKind cursorKind = clang_getCursorKind(cursor);
	CXCursorKind parentKind = clang_getCursorKind(parent);

	name = GetCursorSpelling(cursor);
	type = GetTypeSpelling(cursor);
	String pid = FetchString(clang_getCursorPrettyPrinted(cursor, pp_id));

	CXCursor p = parent;
	scope.Clear();
	nspace.Clear();
	for(;;) {
		CXCursorKind k = clang_getCursorKind(p);
		if(findarg(k, CXCursor_Namespace, CXCursor_ClassTemplate, CXCursor_StructDecl, CXCursor_UnionDecl, CXCursor_ClassDecl) < 0)
			break;
		String q = GetCursorSpelling(p);
		scope = scope.GetCount() ? q + "::" + scope : q;
		if(k == CXCursor_Namespace)
			nspace = nspace.GetCount() ? q + "::" + nspace : q;
		p = clang_getCursorSemanticParent(p);
	}
	int q = scope.Find('('); // 'Struct::(unnamed enum at C:\u\upp.src\upptst\Annotations\main.cpp:47:2)'
	if(q >= 0)
		scope.Trim(q);
	if(scope.GetCount() && *scope.Last() != ':')
		scope << "::";

	annotation = true;
	external = false;
	if(findarg(parentKind, CXCursor_FunctionTemplate, CXCursor_FunctionDecl, CXCursor_CXXMethod,
	                       CXCursor_Constructor, CXCursor_Destructor) >= 0)
		annotation = false; // local variable
	else
		switch(cursorKind) {
		case CXCursor_StructDecl:
		case CXCursor_UnionDecl:
		case CXCursor_ClassDecl:
		case CXCursor_FunctionTemplate:
		case CXCursor_FunctionDecl:
		case CXCursor_Constructor:
		case CXCursor_Destructor:
		case CXCursor_CXXMethod:
			m = pid;
			break;
		case CXCursor_VarDecl:
			external = clang_Cursor_hasVarDeclExternalStorage(cursor);
		case CXCursor_FieldDecl:
		case CXCursor_ClassTemplate:
			m << scope << name;
			break;
		case CXCursor_ConversionFunction:
			m << scope << "operator " << type;
			break;
		case CXCursor_MacroDefinition:
			m = name;
			break;
		case CXCursor_EnumConstantDecl:
			m << scope << name;
			break;
		case CXCursor_EnumDecl:
//		case CXCursor_ParmDecl:
		case CXCursor_TypedefDecl:
		case CXCursor_Namespace:
		case CXCursor_UnexposedDecl:
//		case CXCursor_NamespaceAlias:
//				break;
		default:
			annotation = false;
			break;
		}
	if(annotation && m.GetCount()) {
		RTIMING("ValidNide");
		CXFile file;
		unsigned line_;
		unsigned column_;
		unsigned offset_;
		clang_getExpansionLocation(cxlocation, &file, &line_, &column_, &offset_);
		line = line_;
		column = column_;
		kind = cursorKind;
		pretty = CleanupPretty(FetchString(clang_getCursorPrettyPrinted(cursor, pp_pretty)));
		id = CleanupId(m);
		Item();
	}
	return true;
}


CXChildVisitResult clang_visitor(CXCursor cursor, CXCursor p, CXClientData clientData) {
	ClangVisitor *v = (ClangVisitor *)clientData;
	v->ProcessNode(cursor);
	clang_visitChildren(cursor, clang_visitor, clientData);
	return CXChildVisit_Continue;
}

void ClangVisitor::Do(CXTranslationUnit tu)
{
	if(!tu) return;
	CXCursor cursor = clang_getTranslationUnitCursor(tu);
	pp_id = clang_getCursorPrintingPolicy(cursor);
	pp_pretty = clang_getCursorPrintingPolicy(cursor);
	for(int i = 0; i <= CXPrintingPolicy_LastProperty; i++) {
		clang_PrintingPolicy_setProperty(pp_id, (CXPrintingPolicyProperty)i, 0);
		clang_PrintingPolicy_setProperty(pp_pretty, (CXPrintingPolicyProperty)i, 0);
	}
	
	for(CXPrintingPolicyProperty p : {
			CXPrintingPolicy_SuppressSpecifiers,
			CXPrintingPolicy_SuppressTagKeyword,
			CXPrintingPolicy_SuppressUnwrittenScope,
			CXPrintingPolicy_SuppressInitializers,
			CXPrintingPolicy_SuppressStrongLifetime,
			CXPrintingPolicy_SuppressLifetimeQualifiers,
			CXPrintingPolicy_SuppressTemplateArgsInCXXConstructors,
			CXPrintingPolicy_TerseOutput,
			CXPrintingPolicy_SuppressImplicitBase,
			CXPrintingPolicy_FullyQualifiedName })
		clang_PrintingPolicy_setProperty(pp_id, p, 1);

	clang_PrintingPolicy_setProperty(pp_pretty, CXPrintingPolicy_TerseOutput, 1);
	clang_PrintingPolicy_setProperty(pp_pretty, CXPrintingPolicy_SuppressScope, 1);
	initialized = true;
	clang_visitChildren(cursor, clang_visitor, this);
}

ClangVisitor::~ClangVisitor()
{
	if(initialized) {
		clang_PrintingPolicy_dispose(pp_id);
		clang_PrintingPolicy_dispose(pp_pretty);
	}
}
