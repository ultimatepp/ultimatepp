#include "clang.h"

// #define DUMPTREE

class ClangCursorInfo {
	CXCursor     cursor;
	CXCursorKind cursorKind;
	CXCursor     parent;
	CXCursorKind parentKind;
	CXPrintingPolicy pp_id;
	

	bool         hasraw_id = false;
	String       raw_id;

	bool         hasscope = false;
	String       scope;
	String       nspace;
	
	bool         hastype = false;
	String       type;
	
	bool         hasname = false;
	String       name;
	
	bool         hasid = false;
	String       id;

public:
	int          Kind()                      { return cursorKind; }
	String       RawId();
	String       Scope();
	String       Nspace()                    { Scope(); return nspace; }
	String       Type();
	String       Name();
	bool         NoId();
	String       Id();

	ClangCursorInfo(CXCursor cursor, CXPrintingPolicy pp_id);
};

ClangCursorInfo::ClangCursorInfo(CXCursor cursor, CXPrintingPolicy pp_id)
:	cursor(cursor), pp_id(pp_id) {
	cursorKind = clang_getCursorKind(cursor);
	parent = clang_getCursorSemanticParent(cursor);
	parentKind = clang_getCursorKind(parent);
}

force_inline
String ClangCursorInfo::RawId()
{
	if(!hasraw_id) {
		raw_id = FetchString(clang_getCursorPrettyPrinted(cursor, pp_id));
		hasraw_id = true;
	}
	return raw_id;
}

force_inline
String ClangCursorInfo::Type()
{
	if(!hastype) {
		type = GetTypeSpelling(cursor);
		hastype = true;
	}
	return type;
}

force_inline
String ClangCursorInfo::Name()
{
	if(!hasname) {
		name = GetCursorSpelling(cursor);
		hasname = true;
	}
	return name;
}

String ClangCursorInfo::Scope()
{
	if(!hasscope) {
		CXCursor p = parent;
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
		hasscope = true;
	}
	return scope;
}

bool   ClangCursorInfo::NoId()
{ // TODO: remove
	return findarg(parentKind, CXCursor_FunctionTemplate, CXCursor_FunctionDecl, CXCursor_CXXMethod,
		                       CXCursor_Constructor, CXCursor_Destructor) >= 0 ||
		   findarg(cursorKind, CXCursor_StructDecl, CXCursor_UnionDecl, CXCursor_ClassDecl, CXCursor_FunctionTemplate,
		                       CXCursor_FunctionDecl, CXCursor_Constructor, CXCursor_Destructor, CXCursor_CXXMethod,
		                       CXCursor_VarDecl, CXCursor_FieldDecl, CXCursor_ClassTemplate,
		                       CXCursor_ConversionFunction, CXCursor_MacroDefinition,
		                       CXCursor_EnumConstantDecl) < 0;
}

String ClangCursorInfo::Id()
{
	if(!hasid) {
		String m;
		if(findarg(parentKind, CXCursor_FunctionTemplate, CXCursor_FunctionDecl, CXCursor_CXXMethod,
		                       CXCursor_Constructor, CXCursor_Destructor) < 0) { // local variable, TODO (members of local structure)
			switch(cursorKind) {
			case CXCursor_StructDecl:
			case CXCursor_UnionDecl:
			case CXCursor_ClassDecl:
				m = Type();
				break;
			case CXCursor_FunctionTemplate:
			case CXCursor_FunctionDecl:
			case CXCursor_Constructor:
			case CXCursor_Destructor:
			case CXCursor_CXXMethod:
				m = RawId();
				break;
			case CXCursor_VarDecl:
			case CXCursor_FieldDecl:
			case CXCursor_ClassTemplate:
				m << Scope() << Name();
				break;
			case CXCursor_ConversionFunction:
				m << Scope() << "operator " << Type();
				break;
			case CXCursor_MacroDefinition:
				m = Name();
				break;
			case CXCursor_EnumConstantDecl:
				m << Scope() << Name();
				break;
	/*
			case CXCursor_EnumDecl:
			case CXCursor_ParmDecl:
			case CXCursor_TypedefDecl:
			case CXCursor_Namespace:
			case CXCursor_UnexposedDecl:
			case CXCursor_NamespaceAlias:
					break;
	*/
			default:;
			}
		}
		id = CleanupId(m);
		hasid = true;
	}
	return id;
}

SourceLocation ClangVisitor::GetLocation(CXSourceLocation cxlocation)
{
	CXFile file;
	SourceLocation p;
	unsigned line;
	unsigned column;
	unsigned offset;
	clang_getExpansionLocation(cxlocation, &file, &line, &column, &offset);
	p.pos.x = int(column - 1);
	p.pos.y = int(line - 1);

	int q = cxfile.Find(file);
	if(q >= 0)
		p.path = cxfile[q];
	else {
		p.path = NormalizePath(FetchString(clang_getFileName(file)));
		cxfile.Add(file, p.path);
	}
	return p;
}

bool ClangVisitor::ProcessNode(CXCursor cursor)
{
	CXSourceLocation cxlocation = clang_getCursorLocation(cursor);
	
	ClangCursorInfo ci(cursor, pp_id);

#ifdef DUMPTREE
	_DBG_
	DLOG("=====================================");
	DDUMP((int)cursorKind);
	DDUMP(GetCursorKindName(cursorKind));
	DDUMP(SourceLocation(cxlocation));
	DDUMP(GetCursorSpelling(cursor));
	DDUMP(FetchString(clang_getCursorDisplayName(cursor)));
	DDUMP(FetchString(clang_getCursorPrettyPrinted(cursor, pp_id)));
	DDUMP(FetchString(clang_getCursorPrettyPrinted(cursor, pp_pretty)));

	{
		CXCursor ref = clang_getCursorReferenced(cursor);
	
		DDUMP(clang_Cursor_isNull(ref));
	
		String rs = FetchString(clang_getCursorPrettyPrinted(ref, pp_id));
		if(rs.GetCount()) {
			DDUMP(GetCursorKindName(clang_getCursorKind(ref)));
			DDUMP(FetchString(clang_getCursorPrettyPrinted(ref, pp_pretty)));
			DDUMP(FetchString(clang_getCursorUSR(ref)));
			DDUMP(rs);
		}
	}
#endif

	
	SourceLocation loc;
	bool loc_loaded = false;
	auto LoadLocation = [&] {
		if(loc_loaded) return;
		loc = GetLocation(cxlocation);
		loc_loaded = true;
	};
	
	if(findarg(ci.Kind(), CXCursor_CXXMethod, CXCursor_FunctionTemplate) >= 0) {
		LoadLocation();
		tfn.GetAdd(loc).cursor = cursor;
	}

	if(WhenFile)
		LoadLocation();

	if(!(WhenFile ? WhenFile(loc.path) : clang_Location_isFromMainFile(cxlocation)))
		return findarg(ci.Kind(), CXCursor_StructDecl, CXCursor_UnionDecl, CXCursor_ClassDecl,
		                           CXCursor_FunctionTemplate, CXCursor_FunctionDecl, CXCursor_Constructor,
		                           CXCursor_Destructor,
		                           CXCursor_ClassTemplatePartialSpecialization, CXCursor_UnexposedDecl,
		                           CXCursor_UsingDeclaration, CXCursor_VarDecl, CXCursor_EnumConstantDecl,
		                           CXCursor_TypeAliasTemplateDecl, CXCursor_EnumDecl, CXCursor_ConversionFunction) < 0;

	CXCursor ref = clang_getCursorReferenced(cursor);

	String id = ci.Id();
	DHITCOUNT("Resolved ID");
	if(id.GetCount()) {
		DTIMING("Has ID");
		LoadLocation();
		AnnotationItem& r = item.GetAdd(loc.path).Add();
		r.kind = ci.Kind();
		r.name = ci.Name();
		r.line = loc.pos.y;
		r.id = id;
		r.pretty = ci.Kind() == CXCursor_MacroDefinition ? r.name
                   : CleanupPretty(FetchString(clang_getCursorPrettyPrinted(cursor, pp_pretty)));
		r.definition = clang_isCursorDefinition(cursor);
		r.nspace = ci.Nspace();
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
			if(IsStruct(ci.Kind()))
				MergeWith(r.nest, "::", r.name);
		}
		r.uname = ToUpper(r.name);
		r.unest = ToUpper(r.nest);
		ReferenceItem rm; // prevent self-references
		rm.pos = loc.pos;
		rm.id = r.id;
		ref_done.GetAdd(loc.path).FindAdd(rm);
	}

	if(!clang_Cursor_isNull(ref)) {
		DTIMING("Ref");
		LoadLocation();
		SourceLocation ref_loc = GetLocation(clang_getCursorLocation(ref));
		int q = tfn.Find(ref_loc);
		
		ClangCursorInfo ref_ci(q >= 0 ? tfn[q].cursor : ref, pp_id);

		ReferenceItem rm;
		rm.pos = ref_loc.pos;
		rm.id = ref_ci.Id();
		Index<ReferenceItem>& rd = ref_done.GetAdd(ref_loc.path);
		if(rm.id.GetCount() && rd.Find(rm) < 0) {
			rd.Add(rm);
			refs.GetAdd(ref_loc.path).Add(rm);
		}
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