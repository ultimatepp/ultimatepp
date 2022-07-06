#include "clang.h"

#include <cxxabi.h>

#define LLOG(x)

Semaphore                              current_file_event;
CurrentFileContext                     current_file;
bool                                   do_autocomplete;
bool                                   autocomplete_macros;
Point                                  autocomplete_pos;
int64                                  autocomplete_serial;
Event<const Vector<AutoCompleteItem>&> autocomplete_done;
bool                                   do_annotations;
Event<const Vector<AnnotationItem>&>   annotations_done;

CXChildVisitResult current_file_visitor( CXCursor cursor, CXCursor p, CXClientData clientData)
{
	CXSourceLocation cxlocation = clang_getCursorLocation(cursor);
	bool valid = clang_Location_isFromMainFile(cxlocation);


	// TODO: change this for MT
	static CXPrintingPolicy pp_id = clang_getCursorPrintingPolicy(cursor);
	ONCELOCK {
		for(int i = 0; i <= CXPrintingPolicy_LastProperty; i++)
			clang_PrintingPolicy_setProperty(pp_id, (CXPrintingPolicyProperty)i, 0);
		
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
	};
	
	if(valid) {
		String m;

		CXCursor parent = clang_getCursorSemanticParent(cursor);
		CXCursorKind cursorKind = clang_getCursorKind(cursor);
		CXCursorKind parentKind = clang_getCursorKind(parent);
	
		String name = GetCursorSpelling(cursor);
		String type = GetTypeSpelling(cursor);
		String id = FetchString(clang_getCursorPrettyPrinted(cursor, pp_id));
		String scope = GetTypeSpelling(parent);
		if(scope.GetCount() == 0) { // type is fully qualified, otherwise scan up for namespaces
			CXCursor p = parent;
			for(;;) {
				CXCursorKind k = clang_getCursorKind(p);
				if(findarg(k, CXCursor_Namespace, CXCursor_ClassTemplate) < 0)
					break;
				String q = GetCursorSpelling(p);
				if(scope.GetCount())
					q << "::" << scope;
				scope = q;
				p = clang_getCursorSemanticParent(p);
			}
		}
		int q = scope.Find('('); // 'Struct::(unnamed enum at C:\u\upp.src\upptst\Annotations\main.cpp:47:2)'
		if(q >= 0)
			scope.Trim(q);
		if(scope.GetCount() && *scope.Last() != ':')
			scope << "::";

		auto Dump = [&] {
				SourceLocation location(cxlocation);
				LOG("=====================");
//				DDUMP(location);
//				DDUMP((int)cursorKind);
				DDUMP(GetCursorKindName(cursorKind));
				DDUMP(name);
				DDUMP(type);
				DDUMP(id);
				DDUMP(CleanupId(id));
				DDUMP(scope);
				DDUMP(clang_isCursorDefinition(cursor));
			#if 0
				static CXPrintingPolicy pp = clang_getCursorPrintingPolicy(cursor);
				ONCELOCK {
					for(int i = 0; i <= CXPrintingPolicy_LastProperty; i++)
						clang_PrintingPolicy_setProperty(pp, (CXPrintingPolicyProperty)i, 0);
					
					for(CXPrintingPolicyProperty p : {
							CXPrintingPolicy_SuppressSpecifiers,
							CXPrintingPolicy_SuppressTagKeyword,
//							CXPrintingPolicy_IncludeTagDefinition,
//							CXPrintingPolicy_SuppressScope,
							CXPrintingPolicy_SuppressUnwrittenScope,
							CXPrintingPolicy_SuppressInitializers,
//							CXPrintingPolicy_ConstantArraySizeAsWritten,
//							CXPrintingPolicy_AnonymousTagLocations,
							CXPrintingPolicy_SuppressStrongLifetime,
							CXPrintingPolicy_SuppressLifetimeQualifiers,
							CXPrintingPolicy_SuppressTemplateArgsInCXXConstructors,
//							CXPrintingPolicy_Bool,
//							CXPrintingPolicy_Restrict,
//							CXPrintingPolicy_Alignof,
//							CXPrintingPolicy_UnderscoreAlignof,
//							CXPrintingPolicy_UseVoidForZeroParams,
							CXPrintingPolicy_TerseOutput,
//							CXPrintingPolicy_PolishForDeclaration,
//							CXPrintingPolicy_Half,
//							CXPrintingPolicy_MSWChar,
//							CXPrintingPolicy_IncludeNewlines,
//							CXPrintingPolicy_MSVCFormatting,
//							CXPrintingPolicy_ConstantsAsWritten,
							CXPrintingPolicy_SuppressImplicitBase,
							CXPrintingPolicy_FullyQualifiedName })
					clang_PrintingPolicy_setProperty(pp, p, 1);
				};
				{
//					DTIMING("PrettyPrinting");
//					DDUMP(FetchString(clang_getCursorPrettyPrinted(cursor, pp)));
				}
//				DDUMP(FetchString(clang_Cursor_getMangling(cursor)));
//				DDUMP(CxxDemangle(FetchString(clang_Cursor_getMangling(cursor))));
//				DDUMP(GetCursorKindName(parentKind));
//				DDUMP(GetCursorSpelling(parent));
//				DDUMP(GetTypeSpelling(parent));
			#endif
		};

//		Dump();

		switch(cursorKind) {
		case CXCursor_StructDecl:
		case CXCursor_UnionDecl:
		case CXCursor_ClassDecl:
		case CXCursor_FunctionTemplate:
		case CXCursor_FunctionDecl:
		case CXCursor_Constructor:
		case CXCursor_Destructor:
		case CXCursor_CXXMethod:
			m = id;
			break;
		case CXCursor_VarDecl:
			if(findarg(parentKind, CXCursor_FunctionTemplate, CXCursor_FunctionDecl, CXCursor_CXXMethod) >= 0) {
				valid = false; // local variable
				break;
			}
		case CXCursor_ClassTemplate:
		case CXCursor_FieldDecl:
			m << scope << name;
			break;
		case CXCursor_ConversionFunction:
			m = "operator " + type;
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
			break;
		default:
			valid = false;
			break;
		}
		if(valid) {
			Dump();
			if(m.GetCount()) {
				CXFile file;
				unsigned line_;
				unsigned column_;
				unsigned offset_;
				clang_getExpansionLocation(cxlocation, &file, &line_, &column_, &offset_);
				AnnotationItem& r = static_cast<Vector<AnnotationItem> *>(clientData)->Add();
				r.line = line_;
				r.id = CleanupId(m);
				static CXPrintingPolicy pp;
				ONCELOCK {
					pp = clang_getCursorPrintingPolicy(cursor);
					for(int i = 0; i <= CXPrintingPolicy_LastProperty; i++)
						clang_PrintingPolicy_setProperty(pp, (CXPrintingPolicyProperty)i, 0);
					clang_PrintingPolicy_setProperty(pp, CXPrintingPolicy_TerseOutput, 1);
					clang_PrintingPolicy_setProperty(pp, CXPrintingPolicy_SuppressScope, 1);
				}
				r.pretty = CleanupSignature(FetchString(clang_getCursorPrettyPrinted(cursor, pp)));
				DDUMP(r.id);
				DDUMP(r.pretty);
			}
		}
	}
	clang_visitChildren(cursor, current_file_visitor, clientData);
	return CXChildVisit_Continue;
}

void ReadAutocomplete(const CXCompletionString& string, String& name, String& signature)
{
	const int chunkCount = clang_getNumCompletionChunks(string);
	for(int j = 0; j < chunkCount; j++) {
		const CXCompletionChunkKind chunkKind = clang_getCompletionChunkKind(string, j);
		String text = FetchString(clang_getCompletionChunkText(string, j));
		if(chunkKind == CXCompletionChunk_Optional)
			for(int i = 0; i < clang_getNumCompletionChunks(string); i++)
				ReadAutocomplete(clang_getCompletionChunkCompletionString(string, i), name, signature);
		else
		if(chunkKind == CXCompletionChunk_TypedText) {
			name = text;
			signature << text;
		}
		else {
			signature << text;
			if (chunkKind == CXCompletionChunk_ResultType) {
				signature << ' ';
			}
		}
	}
}

void CurrentFileThread()
{
	MemoryIgnoreLeaksBlock __;

	CurrentFileContext parsed_file;
	CXTranslationUnit tu = nullptr;
	int64 serial;

	auto DisposeTU = [&] {
		if(tu) clang_disposeTranslationUnit(tu);
		if(tu) DLOG("DisposeTU" << tu);
		tu = nullptr;
	};

	auto DoAnnotations = [&] {
		if(!tu || !annotations_done) return;
		Vector<AnnotationItem> item;
		DLOG("DoAnnotations " << tu);
		clang_visitChildren(clang_getTranslationUnitCursor(tu), current_file_visitor, &item);
		Ctrl::Call([&] {
			if(parsed_file.filename == current_file.filename &&
			   parsed_file.real_filename == current_file.real_filename &&
			   parsed_file.includes == current_file.includes)
				annotations_done(item);
			do_annotations = false;
		});
	};

	while(!Thread::IsShutdownThreads()) {
		CurrentFileContext f;
		bool autocomplete_do, annotations_do, macros;
		{
			GuiLock __;
			f = current_file;
			serial = autocomplete_serial;
			autocomplete_do = do_autocomplete;
			annotations_do = do_annotations;
			macros = autocomplete_macros;
		}
		String fn = f.filename;
		if(!IsSourceFile(fn))
			fn.Cat(".cpp");
		if(f.filename != parsed_file.filename || f.includes != parsed_file.includes ||
		   f.real_filename != parsed_file.real_filename || !tu) {
			parsed_file = f;
			DDUMP(f.real_filename);
			DisposeTU();
			String cmdline;
			cmdline << fn << " -DflagDEBUG -DflagDEBUG_FULL -DflagBLITZ -DflagWIN32 -DflagMAIN -DflagGUI -xc++ -std=c++17 ";
			for(String s : Split(f.includes, ';'))
				cmdline << " -I" << s;
			TIMESTOP("Parse");
			#ifdef PLATFORM_WIN32
			tu = Clang(cmdline, { { ~fn, f.content } },
			           CXTranslationUnit_DetailedPreprocessingRecord|
			           CXTranslationUnit_PrecompiledPreamble|
			           CXTranslationUnit_CreatePreambleOnFirstParse|
			           CXTranslationUnit_KeepGoing|
			           CXTranslationUnit_RetainExcludedConditionalBlocks);
			#else
			tu = Clang(cmdline, { { ~fn, f.content } },
			           CXTranslationUnit_KeepGoing|
			           CXTranslationUnit_RetainExcludedConditionalBlocks);
			#endif
			DLOG("Parsed " << f.real_filename << " " << tu);
			DoAnnotations();
			annotations_do = false;
//			DumpDiagnostics(tu);
		}
		if(Thread::IsShutdownThreads()) break;
		CXUnsavedFile ufile = { ~fn, ~f.content, (unsigned)f.content.GetCount() };
		if(autocomplete_do && tu) {
			CXCodeCompleteResults *results;
			{
				TIMESTOP("clang_codeCompleteAt");
				results = clang_codeCompleteAt(tu, fn, autocomplete_pos.y, autocomplete_pos.x, &ufile, 1,
				                               macros ? CXCodeComplete_IncludeMacros : 0);
			}
//			DumpDiagnostics(tu);
			if(results) {
				Vector<AutoCompleteItem> item;
				for(int i = 0; i < results->NumResults; i++) {
					const CXCompletionString& string = results->Results[i].CompletionString;
					int kind = results->Results[i].CursorKind;
					if(kind == CXCursor_MacroDefinition && !macros) // we probably want this only on Ctrl+Space
						continue;
					if(kind == CXCursor_NotImplemented)
						continue;
					String name;
					String signature;
					ReadAutocomplete(string, name, signature);
					AutoCompleteItem& m = item.Add();
					m.name = name;
					m.parent = FetchString(clang_getCompletionParent(string, NULL));
					m.signature = CleanupSignature(signature);
					m.kind = kind;
					m.priority = clang_getCompletionPriority(string);
				}
				clang_disposeCodeCompleteResults(results);
				Ctrl::Call([&] {
					if(serial == autocomplete_serial)
						autocomplete_done(item);
				});
			}
			GuiLock __;
			do_autocomplete = false;
		}
		if(Thread::IsShutdownThreads()) break;
		if(annotations_do && tu) {
			TIMESTOP("ReParse");
			if(clang_reparseTranslationUnit(tu, 1, &ufile, 0))
				DisposeTU();
			else
				DoAnnotations();
		}
		current_file_event.Wait(500);
	}
	DisposeTU();
}

void StartCurrentFileParserThread()
{
	MemoryIgnoreNonMainLeaks();
	MemoryIgnoreNonUppThreadsLeaks(); // clangs leaks static memory in threads
	Thread::StartNice([] { CurrentFileThread(); });
}

void SetCurrentFile(const CurrentFileContext& ctx, Event<const Vector<AnnotationItem>&> done)
{
	GuiLock __;
	current_file = ctx;
	do_annotations = true;
	annotations_done = done;
	current_file_event.Release();
}

void StartAutoComplete(const CurrentFileContext& ctx, int line, int column, bool macros,
                       Event<const Vector<AutoCompleteItem>&> done)
{
	GuiLock __;
	static int64 serial;
	autocomplete_pos.y = line;
	autocomplete_pos.x = column;
	do_autocomplete = true;
	autocomplete_macros = macros;
	autocomplete_serial = serial++;
	autocomplete_done = done;
	current_file = ctx;
	current_file_event.Release();
}

void CancelAutoComplete()
{
	GuiLock __;
	autocomplete_done.Clear();
	do_autocomplete = false;
}

void CancelCurrentFile()
{
	GuiLock __;
	CancelAutoComplete();
	annotations_done.Clear();
}
