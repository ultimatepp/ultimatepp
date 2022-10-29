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
	String       Id();
	String       Bases();

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

String ClangCursorInfo::Id()
{
	if(!hasid) {
		String m;
		switch(cursorKind) {
		case CXCursor_StructDecl:
		case CXCursor_ClassDecl:
		case CXCursor_UnionDecl:
		case CXCursor_TypedefDecl:
		case CXCursor_TypeAliasDecl:
		case CXCursor_EnumDecl:
			m = Type();
			break;
		case CXCursor_FunctionDecl:
		case CXCursor_Constructor:
		case CXCursor_Destructor:
		case CXCursor_CXXMethod:
#ifdef UBUNTU2204_WORKAROUND
			{
				String h = RawId();
				int q = 0;
				while(findarg(h[q], ':', '*', '&', '(', ')', ' ') >= 0)
					q++;
				m = Scope();
				m.Cat(~h + q, h.GetCount() - q);
			}
#else
			m = RawId();
#endif
			break;
		case CXCursor_FunctionTemplate:
			hasid = true;
			id = Scope() + CleanupId(RawId());
			return id;
		case CXCursor_ClassTemplate:
		case CXCursor_VarDecl:
		case CXCursor_FieldDecl:
			m << Scope() << Name();
			break;
		case CXCursor_ConversionFunction:
			m << Scope() << "operator " << Type();
			break;
		case CXCursor_MacroDefinition:
			id = Name();
			hasid = true;
			return id;
		case CXCursor_EnumConstantDecl:
			m << Scope() << Name();
			break;
			
/*
		case CXCursor_ParmDecl:
		case CXCursor_Namespace:
		case CXCursor_UnexposedDecl:
		case CXCursor_NamespaceAlias:
				break;
*/
		default:;
		}
		id = CleanupId(m);
		hasid = true;
	}
	return id;
}

String ClangCursorInfo::Bases()
{
	String result;
	if(findarg(cursorKind, CXCursor_StructDecl, CXCursor_ClassDecl, CXCursor_ClassTemplate) >= 0)
		clang_visitChildren(cursor,
			[](CXCursor cursor, CXCursor p, CXClientData clientData) -> CXChildVisitResult {
				if(clang_getCursorKind(cursor) == CXCursor_CXXBaseSpecifier) {
					String& result = *(String *)clientData;
					MergeWith(result, ";", GetTypeSpelling(cursor));
				}
				return CXChildVisit_Continue;
			},
			&result
		);
	return result;
}

ClangVisitor::CXLocation ClangVisitor::GetLocation(CXSourceLocation cxlocation)
{
	CXFile file;
	CXLocation p;
	unsigned line;
	unsigned column;
	unsigned offset;
	clang_getExpansionLocation(cxlocation, &file, &line, &column, &offset);
	p.pos.x = int(column - 1);
	p.pos.y = int(line - 1);
	p.file = file;
	return p;
}

SourceLocation ClangVisitor::GetSourceLocation(const CXLocation& p)
{
	SourceLocation l;
	l.pos = p.pos;
	int q = cxfile.Find(p.file);
	if(q >= 0)
		l.path = cxfile[q];
	else {
		l.path = NormalizePath(FetchString(clang_getFileName(p.file)));
		cxfile.Add(p.file, l.path);
	}
	return l;
}

bool ClangVisitor::ProcessNode(CXCursor cursor)
{
	CXSourceLocation cxlocation = clang_getCursorLocation(cursor);
	
	ClangCursorInfo ci(cursor, pp_id);

#ifdef DUMPTREE
	_DBG_
	{
		DLOG("=====================================");
		DDUMP(ci.Kind());
		DDUMP(GetCursorKindName((CXCursorKind)ci.Kind()));
		DDUMP(GetCursorSpelling(cursor));
		DDUMP(ci.RawId());
		DDUMP(ci.Type());
		DDUMP(FetchString(clang_getCursorDisplayName(cursor)));
		DDUMP(FetchString(clang_getCursorPrettyPrinted(cursor, pp_id)));
		DDUMP(FetchString(clang_getCursorPrettyPrinted(cursor, pp_pretty)));
		DDUMP(clang_Cursor_isNull(clang_getCursorReferenced(cursor)));
		DDUMP(clang_Location_isFromMainFile(cxlocation));
	}

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

	
	CXLocation loc = GetLocation(cxlocation);

	SourceLocation sl;
	bool sl_loaded = false;
	auto LoadSourceLocation = [&] {
		if(sl_loaded) return;
		sl = GetSourceLocation(loc);
		sl_loaded = true;
	};
	
	if(findarg(ci.Kind(), CXCursor_CXXMethod, CXCursor_FunctionTemplate) >= 0) {
		LoadSourceLocation();
		tfn.GetAdd(sl).cursor = cursor;
	}
	
	bool active_file;
	int q = do_file.Find(loc.file);
	if(q >= 0)
		active_file = do_file[q];
	else {
		LoadSourceLocation();
		active_file = WhenFile(sl.path);
		do_file.Add(loc.file, active_file);
	}

	if(WhenFile)
		LoadSourceLocation();

	if(!active_file)
		return findarg(ci.Kind(), CXCursor_FunctionTemplate, CXCursor_FunctionDecl, CXCursor_Constructor,
		                          CXCursor_Destructor, CXCursor_CXXMethod, CXCursor_UsingDeclaration, CXCursor_VarDecl, CXCursor_EnumConstantDecl,
		                          CXCursor_TypeAliasTemplateDecl, CXCursor_EnumDecl, CXCursor_ConversionFunction) < 0;

	CXCursor ref = clang_getCursorReferenced(cursor);

	String id = ci.Id();
	int kind = ci.Kind();
	if(id.GetCount()) {
		LoadSourceLocation();
		CppFileInfo& f = info.GetAdd(sl.path);
		AnnotationItem& r = locals ? f.locals.Add() : f.items.Add();
		r.kind = kind;
		r.name = ci.Name();
		r.type = ci.Type();
		r.pos = loc.pos;
		r.id = id;
		r.pretty = kind == CXCursor_MacroDefinition ? r.name
	               : CleanupPretty(FetchString(clang_getCursorPrettyPrinted(cursor, pp_pretty)));
		r.definition = clang_isCursorDefinition(cursor);
		r.nspace = ci.Nspace();
		r.bases = ci.Bases();
		r.isvirtual = kind == CXCursor_CXXMethod && clang_CXXMethod_isVirtual(cursor);
		if(findarg(r.kind, CXCursor_Constructor, CXCursor_Destructor) >= 0) {
			int q = r.id.Find('(');
			if(q >= 0) {
				q = r.id.ReverseFind("::", q);
				if(q >= 0)
					r.nest = r.id.Mid(0, q);
			}
		}
		else {
			static String op = "operator";
			int q = FindId(r.id, r.kind == CXCursor_ConversionFunction ? "operator" : r.name);
			if(q >= 0) {
				r.nest = r.id.Mid(0, q);
				r.nest.TrimEnd("::");
			}
			if(IsStruct(kind))
				MergeWith(r.nest, "::", r.name);
		}
		r.uname = ToUpper(r.name);
		r.unest = ToUpper(r.nest);
		ReferenceItem rm; // prevent self-references
		rm.pos = loc.pos;
		rm.id = r.id;
		ref_done.GetAdd(sl.path).FindAdd(rm);
	}

	if(!clang_Cursor_isNull(ref)) {
		LoadSourceLocation();
		SourceLocation ref_loc = GetSourceLocation(GetLocation(clang_getCursorLocation(ref)));
		int q = tfn.Find(ref_loc);
	
		ClangCursorInfo ref_ci(q >= 0 ? tfn[q].cursor : ref, pp_id);

		ReferenceItem rm;
		rm.pos = sl.pos;
		rm.id = ref_ci.Id();
		Index<ReferenceItem>& rd = ref_done.GetAdd(ref_loc.path);
		if(rm.id.GetCount() && rd.Find(rm) < 0) {
			rd.Add(rm);
			info.GetAdd(sl.path).refs.Add(rm);
		}
	}

	if(findarg(kind, CXCursor_FunctionTemplate, CXCursor_FunctionDecl, CXCursor_Constructor,
		                  CXCursor_Destructor, CXCursor_CXXMethod, CXCursor_ConversionFunction) >= 0)
		locals = true;

	return true;
}

CXChildVisitResult clang_visitor(CXCursor cursor, CXCursor p, CXClientData clientData) {
#ifdef DUMPTREE
	LOGBEGIN();
#endif
	ClangVisitor *v = (ClangVisitor *)clientData;
	bool bak_locals = v->locals;
	if(v->ProcessNode(cursor))
		clang_visitChildren(cursor, clang_visitor, clientData);
	v->locals = bak_locals;
#ifdef DUMPTREE
	LOGEND();
#endif
	return CXChildVisit_Continue;
}

void ClangVisitor::Do(CXTranslationUnit tu)
{
	if(!HasLibClang())
		return;

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
#ifndef UBUNTU2204_WORKAROUND
			CXPrintingPolicy_FullyQualifiedName,
#endif
			CXPrintingPolicy_Bool })
		clang_PrintingPolicy_setProperty(pp_id, p, 1);

	clang_PrintingPolicy_setProperty(pp_pretty, CXPrintingPolicy_TerseOutput, 1);
	clang_PrintingPolicy_setProperty(pp_pretty, CXPrintingPolicy_Bool, 1);
	clang_PrintingPolicy_setProperty(pp_pretty, CXPrintingPolicy_SuppressScope, 1);
	initialized = true;
	clang_visitChildren(cursor, clang_visitor, this);

	for(CppFileInfo& f : info) { // sort by line because macros are first TODO move it after macros are by HDepend
		Sort(f.items, [](const AnnotationItem& a, const AnnotationItem& b) {
			return CombineCompare(a.pos.y, b.pos.y)(a.pos.x, b.pos.x) < 0;
		});
		// remove duplicates
		Vector<int> toremove;
		for(int i = 1; i < f.items.GetCount(); i++) {
			AnnotationItem& a = f.items[i - 1];
			AnnotationItem& b = f.items[i];
			if(a.pos == b.pos && a.id == b.id)
				toremove.Add(i);
		}
		f.items.Remove(toremove);
	}
}

ClangVisitor::~ClangVisitor()
{
	if(!HasLibClang())
		return;

	if(initialized) {
		clang_PrintingPolicy_dispose(pp_id);
		clang_PrintingPolicy_dispose(pp_pretty);
	}
}