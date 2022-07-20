#include "clang.h"

#define LLOG(x)

bool                                   current_file_parsing;
Semaphore                              current_file_event; // TODO?
CurrentFileContext                     current_file;
int64                                  current_file_serial;
int64                                  current_file_done_serial;
Event<const Vector<AnnotationItem>&>   annotations_done;

void CurrentFileThread()
{
	MemoryIgnoreLeaksBlock __;

	CurrentFileContext parsed_file;
	int64 serial;
	
	Clang clang;

	auto DoAnnotations = [&] {
		if(!clang.tu || !annotations_done) return;
		ClangVisitor v;
		v.Do(clang.tu);
	//	DumpDiagnostics(clang.tu);
		Ctrl::Call([&] {
			if(parsed_file.filename == current_file.filename &&
			   parsed_file.real_filename == current_file.real_filename &&
			   parsed_file.includes == current_file.includes &&
			   serial == current_file_serial)
				annotations_done(v.item[0]);
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
				{
					TIMESTOP("CurrentFile parse");
					current_file_parsing = true;
					clang.Parse(fn, f.content, f.includes, f.defines,
					            CXTranslationUnit_DetailedPreprocessingRecord|
					            CXTranslationUnit_PrecompiledPreamble|
					            CXTranslationUnit_CreatePreambleOnFirstParse|
					            CXTranslationUnit_KeepGoing);
					current_file_parsing = false;
				}
				DoAnnotations();
			}
			if(Thread::IsShutdownThreads()) break;
			if(clang.tu && serial != done_serial) {
				TIMESTOP("ReParse");
				current_file_parsing = true;
				bool b = clang.ReParse(fn, f.content);
				current_file_parsing = false;
				if(b)
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

bool IsCurrentFileParsing()
{
	return current_file_parsing;
}

void CancelCurrentFile()
{
	GuiLock __;
	annotations_done.Clear();
}
