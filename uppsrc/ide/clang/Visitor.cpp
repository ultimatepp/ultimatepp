#include "clang.h"

// #define DUMPTREE

bool ClangVisitor::ProcessNode(CXCursor cursor)
{
	CXSourceLocation cxlocation = clang_getCursorLocation(cursor);

	CXCursorKind cursorKind = clang_getCursorKind(cursor);

#ifdef DUMPTREE
	_DBG_
	DLOG("=====================================");
	DDUMP(GetCursorKindName(cursorKind));
	DDUMP(SourceLocation(cxlocation));
#endif

	bool     position_loaded = false;
	unsigned line;
	unsigned column;
	unsigned offset;
	String   path;
	
	auto LoadPosition = [&] {
		if(position_loaded) return;
		CXFile file;
		clang_getExpansionLocation(cxlocation, &file, &line, &column, &offset);
		path = FetchString(clang_getFileName(file));
		position_loaded = true;
	};

	if(WhenFile)
		LoadPosition();

	if(!(WhenFile ? WhenFile(path) : clang_Location_isFromMainFile(cxlocation))) {
		return findarg(cursorKind, CXCursor_StructDecl, CXCursor_UnionDecl, CXCursor_ClassDecl,
		                           CXCursor_FunctionTemplate, CXCursor_FunctionDecl, CXCursor_Constructor,
		                           CXCursor_Destructor, CXCursor_CXXMethod, CXCursor_ClassTemplate,
		                           CXCursor_ClassTemplatePartialSpecialization, CXCursor_UnexposedDecl,
		                           CXCursor_UsingDeclaration, CXCursor_VarDecl, CXCursor_EnumConstantDecl,
		                           CXCursor_TypeAliasTemplateDecl, CXCursor_EnumDecl, CXCursor_ConversionFunction) < 0;
	}

	String m;

	CXCursor parent = clang_getCursorSemanticParent(cursor);
	CXCursorKind parentKind = clang_getCursorKind(parent);

	String name = GetCursorSpelling(cursor);
	String type = GetTypeSpelling(cursor);
	String pid = FetchString(clang_getCursorPrettyPrinted(cursor, pp_id));
	
#ifdef DUMPTREE
	_DBG_
	CXCursor ref = clang_getCursorReferenced(cursor);
	CXCursorKind refKind = clang_getCursorKind(ref);

	String rs = FetchString(clang_getCursorPrettyPrinted(ref, pp_pretty));
	if(rs.GetCount()) {
		DDUMP(FetchString(clang_getCursorPrettyPrinted(cursor, pp_pretty)));
		DDUMP(FetchString(clang_getCursorPrettyPrinted(parent, pp_pretty)));
		DDUMP(GetCursorKindName(refKind));
		DDUMP(FetchString(clang_getCursorUSR(ref)));
		DDUMP(rs);
	}
#endif

	CXCursor p = parent;
	String scope;
	String nspace;
	for(;;) {
		CXCursorKind k = clang_getCursorKind(p);
		if(k != CXCursor_EnumDecl) {
			if(findarg(k, CXCursor_Namespace, CXCursor_ClassTemplate, CXCursor_StructDecl, CXCursor_UnionDecl, CXCursor_ClassDecl) < 0)
				break;
			String q = GetCursorSpelling(p);
			scope = scope.GetCount() ? q + "::" + scope : q;
			if(k == CXCursor_Namespace)
				nspace = nspace.GetCount() ? q + "::" + nspace : q;
		}
		p = clang_getCursorSemanticParent(p);
	}
	int q = scope.Find('('); // 'Struct::(unnamed enum at C:\u\upp.src\upptst\Annotations\main.cpp:47:2)'
	if(q >= 0)
		scope.Trim(q);
	if(scope.GetCount() && *scope.Last() != ':')
		scope << "::";

	bool annotation = true;
	if(findarg(parentKind, CXCursor_FunctionTemplate, CXCursor_FunctionDecl, CXCursor_CXXMethod,
	                       CXCursor_Constructor, CXCursor_Destructor) >= 0)
		annotation = false; // local variable
	else
		switch(cursorKind) {
		case CXCursor_StructDecl:
		case CXCursor_UnionDecl:
		case CXCursor_ClassDecl:
			m = type;
			break;
		case CXCursor_FunctionTemplate:
		case CXCursor_FunctionDecl:
		case CXCursor_Constructor:
		case CXCursor_Destructor:
		case CXCursor_CXXMethod:
			m = pid;
			break;
		case CXCursor_VarDecl:
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
		LoadPosition();
		AnnotationItem& r = item.GetAdd(path).Add();
		r.kind = cursorKind;
		r.name = name;
		r.line = line - 1;
		r.id = CleanupId(m);
		if(cursorKind == CXCursor_MacroDefinition) {
			DLOG("#define " << r.name << " " << path << " " << line);
		}
		r.pretty = cursorKind == CXCursor_MacroDefinition ? r.name
                   : CleanupPretty(FetchString(clang_getCursorPrettyPrinted(cursor, pp_pretty)));
		r.definition = clang_isCursorDefinition(cursor);
		r.nspace = nspace;
		if(findarg(r.kind, CXCursor_Constructor, CXCursor_Destructor) >= 0) {
			int q = r.id.Find('(');
			if(q >= 0) {
				q = r.id.ReverseFind("::", q);
				if(q >= 0)
					r.nest = r.id.Mid(0, q);
			}
		}
		else {
			int q = FindId(r.id, r.name);
			if(q >= 0) {
				r.nest = r.id.Mid(0, q);
				r.nest.TrimEnd("::");
			}
			if(IsStruct(cursorKind))
				MergeWith(r.nest, "::", name);
		}
		r.uname = ToUpper(name);
		r.unest = ToUpper(r.nest);
	}
	return true;
}


CXChildVisitResult clang_visitor(CXCursor cursor, CXCursor p, CXClientData clientData) {
#ifdef DUMPTREE
	LOGBEGIN();
#endif
	ClangVisitor *v = (ClangVisitor *)clientData;
	if(v->ProcessNode(cursor))
		clang_visitChildren(cursor, clang_visitor, clientData);
#ifdef DUMPTREE
	LOGEND();
#endif
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
	clang_PrintingPolicy_setProperty(pp_pretty, CXPrintingPolicy_Bool, 1);
	clang_PrintingPolicy_setProperty(pp_pretty, CXPrintingPolicy_SuppressScope, 1);
	initialized = true;
	clang_visitChildren(cursor, clang_visitor, this);
	
	for(Vector<AnnotationItem>& f : item) // sort by line because macros are first
		Sort(f, [](const AnnotationItem& a, const AnnotationItem& b) { return a.line < b.line; });
}

ClangVisitor::~ClangVisitor()
{
	if(initialized) {
		clang_PrintingPolicy_dispose(pp_id);
		clang_PrintingPolicy_dispose(pp_pretty);
	}
}
