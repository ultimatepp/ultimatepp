#include "clang.h"

#define LLOG(x)

Semaphore                              current_file_event;
CurrentFileContext                     current_file;
int64                                  current_file_serial;
int64                                  current_file_done_serial;
Event<const Vector<AnnotationItem>&>   annotations_done;

struct CurrentFileVisitor : ClangVisitor {
	Vector<AnnotationItem> item;
	virtual void Item() {
		AnnotationItem& r = item.Add();
		r.kind = GetKind();
		r.name = GetName();
		r.line = GetLine();
		r.id = GetId();
		r.pretty = GetPretty();
		r.definition = IsDefinition();
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
	//	DumpDiagnostics(clang.tu);
		Ctrl::Call([&] {
			if(parsed_file.filename == current_file.filename &&
			   parsed_file.real_filename == current_file.real_filename &&
			   parsed_file.includes == current_file.includes &&
			   serial == current_file_serial)
				annotations_done(v.item);
			current_file_done_serial = serial;
		});
	};

	while(!Thread::IsShutdownThreads()) {
		CurrentFileContext f;
		int64 done_serial;
		{
			GuiLock __;
			f = current_file;
			serial = current_file_serial;
			done_serial = current_file_done_serial;
		}
		if(f.filename.GetCount()) {
			String fn = f.filename;
			if(!IsSourceFile(fn))
				fn.Cat(".cpp");
			if(f.filename != parsed_file.filename || f.real_filename != parsed_file.real_filename ||
			   f.includes != parsed_file.includes || f.defines != parsed_file.defines ||
			   !clang.tu) { // TODO: same is in autocomplete
				parsed_file = f;
				clang.Dispose();
				{
					TIMESTOP("CurrentFile parse");
					clang.Parse(fn, f.content, f.includes, f.defines,
					            CXTranslationUnit_DetailedPreprocessingRecord|
					            CXTranslationUnit_PrecompiledPreamble|
					            CXTranslationUnit_CreatePreambleOnFirstParse|
					            CXTranslationUnit_KeepGoing);
				}
				DoAnnotations();
			}
			if(Thread::IsShutdownThreads()) break;
			if(clang.tu && serial != done_serial) {
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
	annotations_done = done;
	current_file_event.Release();
	current_file_serial++;
}

bool IsCurrentFileDirty()
{
	GuiLock __;
	return current_file_serial != current_file_done_serial;
}

void CancelCurrentFile()
{
	GuiLock __;
	annotations_done.Clear();
}
