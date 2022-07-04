#include "clang.h"

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

	
	if(valid) {
		String m;

		CXCursor parent = clang_getCursorSemanticParent(cursor);
		CXCursorKind cursorKind = clang_getCursorKind(cursor);
		CXCursorKind parentKind = clang_getCursorKind(parent);
	
		String name = GetCursorSpelling(cursor);
		String type = GetTypeSpelling(cursor);
		String display = FetchString(clang_getCursorDisplayName(cursor));
		String scope = GetTypeSpelling(parent);
		int q = scope.Find('('); // 'Struct::(unnamed enum at C:\u\upp.src\upptst\Annotations\main.cpp:47:2)'
		if(q >= 0)
			scope.Trim(q);
		if(scope.GetCount() && *scope.Last() != ':')
			scope << "::";

		auto Dump = [&] {
			#if 1
				LOG("=====================");
				DDUMP((int)cursorKind);
				DDUMP(GetCursorKindName(cursorKind));
				DDUMP(name);
				DDUMP(type);
				DDUMP(display);
				DDUMP(scope);
				DDUMP(clang_isCursorDefinition(cursor));
				static CXPrintingPolicy pp = clang_getCursorPrintingPolicy(cursor);
				clang_PrintingPolicy_setProperty(pp, CXPrintingPolicy_TerseOutput, 1);
				clang_PrintingPolicy_setProperty(pp, CXPrintingPolicy_SuppressScope, 1);
				DDUMP(FetchString(clang_getCursorPrettyPrinted(cursor, pp)));
				DDUMP(GetCursorKindName(parentKind));
				DDUMP(GetCursorSpelling(parent));
				DDUMP(GetTypeSpelling(parent));
	//			DDUMP(location);
			#endif
		};
		
		Dump();

		switch(cursorKind) {
		case CXCursor_VarDecl:
			if(findarg(parentKind, CXCursor_FunctionTemplate, CXCursor_FunctionDecl, CXCursor_CXXMethod) >= 0) {
				valid = false;
				break;
			}
		case CXCursor_FieldDecl:
		case CXCursor_CXXMethod:
			m << scope << display;
			break;
		case CXCursor_StructDecl:
			m = type << "::struct";
			break;
		case CXCursor_UnionDecl:
			m = type << "::union";
			break;
		case CXCursor_ClassTemplate:
			Dump();
		case CXCursor_ClassDecl:
			m = type << "::class";
			break;
		case CXCursor_FunctionTemplate:
		case CXCursor_FunctionDecl:
			m = display;
			break;
		case CXCursor_Constructor:
		case CXCursor_Destructor:
			m << scope << display;
			break;
		case CXCursor_MacroDefinition:
			m = name;
			break;
		case CXCursor_EnumConstantDecl:
			Dump();
			m << scope << name;
			break;
		case CXCursor_EnumDecl:
//		case CXCursor_ParmDecl:
		case CXCursor_TypedefDecl:
		case CXCursor_Namespace:
		case CXCursor_ConversionFunction:
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
				const char *s = m;
				StringBuffer mm;
				while(*s) {
					if(iscid(*s)) {
						while(iscid(*s))
							mm.Cat(*s++);
						while(*s == ' ')
							s++;
						if(iscid(*s))
							mm.Cat(' ');
					}
					else
					if(*s == ' ')
						s++; // filter out spaces that are not necessary
					else
						mm.Cat(*s++);
				}
				r.item = mm;
				static CXPrintingPolicy pp;
				ONCELOCK {
					pp = clang_getCursorPrintingPolicy(cursor);
					for(int i = 0; i <= CXPrintingPolicy_LastProperty; i++)
						clang_PrintingPolicy_setProperty(pp, (CXPrintingPolicyProperty)i, 0);
					clang_PrintingPolicy_setProperty(pp, CXPrintingPolicy_TerseOutput, 1);
					clang_PrintingPolicy_setProperty(pp, CXPrintingPolicy_SuppressScope, 1);
				}
				r.pretty = FetchString(clang_getCursorPrettyPrinted(cursor, pp));
			}
//			DLOG(">> " << GetCursorKindName(cursorKind) << ": " << m);
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
	CurrentFileContext parsed_file;
	CXTranslationUnit tu = nullptr;
	int64 serial;

	auto DisposeTU = [&] {
		if(tu) clang_disposeTranslationUnit(tu);
		tu = nullptr;
	};

	auto DoAnnotations = [&] {
		if(!tu || !annotations_done) return;
		Vector<AnnotationItem> item;
		clang_visitChildren(clang_getTranslationUnitCursor(tu), current_file_visitor, &item);
		Ctrl::Call([&] {
			if(parsed_file.filename == current_file.filename && parsed_file.includes == current_file.includes)
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
		if(f.filename != parsed_file.filename || f.includes != parsed_file.includes || !tu) {
			parsed_file = f;
			DisposeTU();
			String cmdline;
			cmdline << fn << " -DflagDEBUG -DflagDEBUG_FULL -DflagBLITZ -DflagWIN32 -DflagMAIN -DflagGUI -xc++ -std=c++17 ";
			for(String s : Split(f.includes, ';'))
				cmdline << " -I" << s;
			TIMESTOP("Parse");
			tu = Clang(cmdline, { { ~fn, f.content } },
			           CXTranslationUnit_DetailedPreprocessingRecord|
			           CXTranslationUnit_PrecompiledPreamble|
			           CXTranslationUnit_CreatePreambleOnFirstParse|
			           CXTranslationUnit_KeepGoing|
			           CXTranslationUnit_RetainExcludedConditionalBlocks);
			DoAnnotations();
			annotations_do = false;
		}
		if(Thread::IsShutdownThreads()) break;
		if(autocomplete_do && tu) {
			CXUnsavedFile ufile = { ~fn, ~f.content, (unsigned)f.content.GetCount() };
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
		if(annotations_do && tu) {
			TIMESTOP("ReParse");
			clang_reparseTranslationUnit(tu, 0, nullptr, 0);
			DoAnnotations();
		}
		current_file_event.Wait(500);
	}
	DisposeTU();
}

void StartCurrentFileParserThread()
{
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
