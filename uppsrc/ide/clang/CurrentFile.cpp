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

struct CurrentFileVisitor : ClangVisitor {
	Vector<AnnotationItem> item;
	virtual void Item() {
		AnnotationItem& r = item.Add();
		r.kind = GetKind();
		r.name = GetName();
		r.line = GetLine();
		r.id = GetId();
		r.definition = IsDefinition();
		r.external = IsExtern();
		r.nspace = GetNamespace();
	}
};

void CurrentFileThread()
{
	MemoryIgnoreLeaksBlock __;

	CurrentFileContext parsed_file;
	int64 serial;
	
	Clang clang;

	auto DoAnnotations = [&] {
		if(!clang.tu || !annotations_done) return;
		CurrentFileVisitor v;
		v.Do(clang.tu);
		Ctrl::Call([&] {
			if(parsed_file.filename == current_file.filename &&
			   parsed_file.real_filename == current_file.real_filename &&
			   parsed_file.includes == current_file.includes)
				annotations_done(v.item);
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
		if(f.filename.GetCount()) {
			String fn = f.filename;
			if(!IsSourceFile(fn))
				fn.Cat(".cpp");
			if(f.filename != parsed_file.filename || f.includes != parsed_file.includes ||
			   f.real_filename != parsed_file.real_filename || !clang.tu) {
				parsed_file = f;
				clang.Dispose();
				clang.Parse(fn, f.content, f.includes, String(),
				            CXTranslationUnit_DetailedPreprocessingRecord|
				            CXTranslationUnit_PrecompiledPreamble|
				            CXTranslationUnit_CreatePreambleOnFirstParse|
				            CXTranslationUnit_KeepGoing|
				            CXTranslationUnit_RetainExcludedConditionalBlocks);
				DoAnnotations();
				annotations_do = false;
	//			DumpDiagnostics(tu);
			}
			if(Thread::IsShutdownThreads()) break;
			CXUnsavedFile ufile = { ~fn, ~f.content, (unsigned)f.content.GetCount() };
			if(autocomplete_do && clang.tu) {
				CXCodeCompleteResults *results;
				{
					TIMESTOP("clang_codeCompleteAt");
					results = clang_codeCompleteAt(clang.tu, fn, autocomplete_pos.y, autocomplete_pos.x, &ufile, 1,
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
						m.signature = CleanupPretty(signature);
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
			if(annotations_do && clang.tu) {
				TIMESTOP("ReParse");
				if(clang.ReParse(fn, f.content))
					DoAnnotations();
			}
		}
		current_file_event.Wait(500);
	}
}

void StartCurrentFileParserThread()
{
	MemoryIgnoreNonMainLeaks();
	MemoryIgnoreNonUppThreadsLeaks(); // clangs leaks static memory in threads
	Thread::Start([] { CurrentFileThread(); });
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
