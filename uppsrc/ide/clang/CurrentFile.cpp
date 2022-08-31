#include "clang.h"

#define LLOG(x) // LOG(x)

bool                                   current_file_parsing;
CoEvent                                current_file_event;
CurrentFileContext                     current_file;
int64                                  current_file_serial;
int64                                  current_file_done_serial;
Event<const CppFileInfo&>              annotations_done;

CurrentFileContext                     autocomplete_file;

bool                                   do_autocomplete;
bool                                   autocomplete_macros;
Point                                  autocomplete_pos;
int64                                  autocomplete_serial;
Event<const Vector<AutoCompleteItem>&> autocomplete_done;

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
	int64 serial;
	
	Clang clang;

	auto DoAnnotations = [&] {
		if(!clang.tu || !annotations_done) return;
		ClangVisitor v;
		v.dolocals = true;
		v.WhenFile = [&] (const String& path) { return path == current_file.filename; };
		v.Do(clang.tu);
		CppFileInfo f;
		if(v.info.GetCount()) {
			f = pick(v.info[0]);
			f.items.RemoveIf([&](int i) { return f.items[i].pos.y < parsed_file.line_delta; });
			for(AnnotationItem& m : f.items)
				m.pos.y -= parsed_file.line_delta;
			f.locals.RemoveIf([&](int i) { return f.locals[i].pos.y < parsed_file.line_delta; });
			for(AnnotationItem& m : f.locals)
				m.pos.y -= parsed_file.line_delta;
			f.refs.RemoveIf([&](int i) { return f.refs[i].pos.y < parsed_file.line_delta; });
			for(ReferenceItem& m : f.refs)
				m.pos.y -= parsed_file.line_delta;
		}
		Ctrl::Call([&] {
			if(parsed_file.filename == current_file.filename &&
			   parsed_file.real_filename == current_file.real_filename &&
			   parsed_file.includes == current_file.includes &&
			   serial == current_file_serial) {
				annotations_done(f);
				FileAnnotation fa;
				fa.defines = parsed_file.defines;
				fa.includes = parsed_file.includes;
				fa.items = pick(f.items);
				fa.refs = pick(f.refs);
				fa.time = Time::Low();
				CodeIndex().GetAdd(NormalizePath(parsed_file.real_filename)) = pick(fa);
			}
			current_file_done_serial = serial;
		});
	};

	while(!Thread::IsShutdownThreads()) {
		bool was_parsing;
		do {
			was_parsing = false;
			CurrentFileContext f, af;
			int64 done_serial;
			int64 aserial;
			bool autocomplete_do;
			{
				GuiLock __;
				f = current_file;
				af = autocomplete_file;
				serial = current_file_serial;
				done_serial = current_file_done_serial;
				autocomplete_do = do_autocomplete;
				aserial = autocomplete_serial;
			}
			if(f.filename.GetCount()) {
				String fn = f.filename;
				if(!IsSourceFile(fn))
					fn.Cat(".cpp");
				if(f.filename != parsed_file.filename || f.real_filename != parsed_file.real_filename ||
				   f.includes != parsed_file.includes || f.defines != parsed_file.defines ||
				   !clang.tu) { // TODO: same is in autocomplete
					parsed_file = f;
					int tm = msecs();
					current_file_parsing = true;
					DDUMP(f.includes);
					DDUMP(f.defines);
					clang.Parse(fn, f.content, f.includes, f.defines,
					            CXTranslationUnit_PrecompiledPreamble|
					            CXTranslationUnit_CreatePreambleOnFirstParse|
			                    CXTranslationUnit_SkipFunctionBodies|
					            CXTranslationUnit_LimitSkipFunctionBodiesToPreamble|
				//	            CXTranslationUnit_CacheCompletionResults|
					            CXTranslationUnit_KeepGoing);
					DumpDiagnostics(clang.tu); _DBG_
					PutVerbose(String() << "Current file parsed in " << msecs() - tm << " ms");
					tm = msecs();
					DoAnnotations();
					PutVerbose(String() << "Current file parser output processed in " << msecs() - tm << " ms");
					current_file_parsing = false;
					was_parsing = true;
				}
				if(Thread::IsShutdownThreads()) break;
				if(clang.tu && autocomplete_do) {
					CXUnsavedFile ufile = { ~fn, ~af.content, (unsigned)af.content.GetCount() };
					CXCodeCompleteResults *results;
					current_file_parsing = true;
					int tm = msecs();
					{
						MemoryIgnoreLeaksBlock __;
						results = clang_codeCompleteAt(clang.tu, fn, autocomplete_pos.y, autocomplete_pos.x, &ufile, 1, 0);
					}
					PutVerbose(String() << "Autocomplete in " << msecs() - tm << " ms");
	//				DumpDiagnostics(clang.tu);
					if(results) {
						int tm = msecs();
						Vector<AutoCompleteItem> item;
						for(int i = 0; i < results->NumResults; i++) {
							const CXCompletionString& string = results->Results[i].CompletionString;
							int kind = results->Results[i].CursorKind;
							if(kind == CXCursor_MacroDefinition) // we probably want this only on Ctrl+Space
								continue;
							if(kind == CXCursor_NotImplemented)
								continue;
							String name;
							String signature;
							ReadAutocomplete(string, name, signature);
							AutoCompleteItem& m = item.Add();
							m.name = name;
							m.parent = FetchString(clang_getCompletionParent(string, NULL));
							m.signature = CleanupPretty(signature);
							m.kind = kind;
							m.priority = clang_getCompletionPriority(string);
						}
						{
							MemoryIgnoreLeaksBlock __;
							clang_disposeCodeCompleteResults(results);
						}
						Ctrl::Call([&] {
							if(aserial == autocomplete_serial)
								autocomplete_done(item);
						});
						PutVerbose(String() << "Autocomplete processed in " << msecs() - tm << " ms");
					}
					current_file_parsing = false;
					GuiLock __;
					do_autocomplete = false;
				}
				if(clang.tu && serial != done_serial) {
					TIMESTOP("ReParse");
					current_file_parsing = true;
					int tm = msecs();
					bool b = clang.ReParse(fn, f.content);
					PutVerbose(String() << "Current file reparsed in " << msecs() - tm << " ms");
					tm = msecs();
					if(b)
						DoAnnotations();
					PutVerbose(String() << "Current file reparsed output processed in " << msecs() - tm << " ms");
					current_file_parsing = false;
					was_parsing = true;
				}
			}
		}
		while(was_parsing);
		current_file_event.Wait();
		LLOG("Current file Thread::IsShutdownThreads() " << Thread::IsShutdownThreads());
	}
	LLOG("Current file thread exit");
}

void SetCurrentFile(const CurrentFileContext& ctx, Event<const CppFileInfo&> done)
{
	ONCELOCK {
		MemoryIgnoreNonMainLeaks();
		MemoryIgnoreNonUppThreadsLeaks(); // clangs leaks static memory in threads
		Thread::Start([] { CurrentFileThread(); });
		Thread::AtShutdown([] {
			LLOG("Shutdown current file");
			current_file_event.Broadcast();
		});
	}

	GuiLock __;
	annotations_done = done;
	current_file_serial++;
	current_file = ctx;
	current_file_event.Broadcast();
}

bool IsCurrentFileDirty()
{
	GuiLock __;
	return current_file_serial != current_file_done_serial;
}

bool IsCurrentFileParsing()
{
	return current_file_parsing;
}

void CancelCurrentFile()
{
	GuiLock __;
	annotations_done.Clear();
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
	autocomplete_file = ctx;
	current_file_event.Broadcast();
}

void CancelAutoComplete()
{
	GuiLock __;
	autocomplete_done.Clear();
	do_autocomplete = false;
}
