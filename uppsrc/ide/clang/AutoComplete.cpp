#include "clang.h"

// TODO: Remove

#define LLOG(x) // DLOG(x)

#if 0

bool                                   autocomplete_parsing;
CoEvent                                autocomplete_event;
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

void AutocompleteThread()
{
	MemoryIgnoreLeaksBlock __;

	CurrentFileContext parsed_file;
	int64 serial;
	
	Clang clang;

	while(!Thread::IsShutdownThreads()) {
		CurrentFileContext f;
		bool autocomplete_do, macros;
		{
			GuiLock __;
			f = autocomplete_file;
			serial = autocomplete_serial;
			autocomplete_do = do_autocomplete;
			macros = autocomplete_macros;
		}
		if(f.filename.GetCount()) {
			
			String fn = f.filename;
			if(!IsSourceFile(fn))
				fn.Cat(".cpp");
			if(f.filename != parsed_file.filename || f.real_filename != parsed_file.real_filename ||
			   f.includes != parsed_file.includes || f.defines != parsed_file.defines ||
			   !clang.tu) {
				int tm = msecs();
				parsed_file = f;
				TIMESTOP("Autocomplete parse");
				autocomplete_parsing = true;
				clang.Parse(fn, f.content, f.includes, f.defines,
//				            CXTranslationUnit_DetailedPreprocessingRecord|
				            CXTranslationUnit_PrecompiledPreamble|
				            CXTranslationUnit_CreatePreambleOnFirstParse|
				            CXTranslationUnit_SkipFunctionBodies|
//				            CXTranslationUnit_LimitSkipFunctionBodiesToPreamble|
//				            CXTranslationUnit_CacheCompletionResults|
				            CXTranslationUnit_KeepGoing);
//				DumpDiagnostics(clang.tu);
				autocomplete_parsing = false;
				PutVerbose(String() << "Current file autocomplete parsed in " << msecs() - tm << " ms");
			}

			if(Thread::IsShutdownThreads()) break;
			CXUnsavedFile ufile = { ~fn, ~f.content, (unsigned)f.content.GetCount() };
			if(autocomplete_do && clang.tu) {
				CXCodeCompleteResults *results;
				autocomplete_parsing = true;
				int tm = msecs();
				results = clang_codeCompleteAt(clang.tu, fn, autocomplete_pos.y, autocomplete_pos.x, &ufile, 1,
				                               macros ? CXCodeComplete_IncludeMacros : 0);
				PutVerbose(String() << "Autocomplete in " << msecs() - tm << " ms");
//				DumpDiagnostics(clang.tu);
				if(results) {
					int tm = msecs();
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
						m.signature = CleanupPretty(signature);
						m.kind = kind;
						m.priority = clang_getCompletionPriority(string);
					}
					clang_disposeCodeCompleteResults(results);
					Ctrl::Call([&] {
						if(serial == autocomplete_serial)
							autocomplete_done(item);
					});
					PutVerbose(String() << "Autocomplete processed in " << msecs() - tm << " ms");
				}
				autocomplete_parsing = false;
				GuiLock __;
				do_autocomplete = false;
			}
		}
		autocomplete_event.Wait();
		LLOG("Autocomplete Thread::IsShutdownThreads() " << Thread::IsShutdownThreads());
	}
	LLOG("Autocomplete thread exit");
}

void SetAutoCompleteFile(const CurrentFileContext& ctx)
{
	ONCELOCK {
		MemoryIgnoreNonMainLeaks();
		MemoryIgnoreNonUppThreadsLeaks(); // clangs leaks static memory in threads
		Thread::Start([] { AutocompleteThread(); });
		Thread::AtShutdown([] {
			LLOG("Shutdown autocomplete");
			autocomplete_event.Broadcast();
		});
	}
	GuiLock __;
	autocomplete_file = ctx;
	autocomplete_event.Broadcast();
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
	SetAutoCompleteFile(ctx);
}

void CancelAutoComplete()
{
	GuiLock __;
	autocomplete_done.Clear();
	do_autocomplete = false;
}

bool IsAutocompleteParsing()
{
	return autocomplete_parsing;
}

#endif
