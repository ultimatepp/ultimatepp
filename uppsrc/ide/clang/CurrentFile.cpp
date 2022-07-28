#include "clang.h"

#define LLOG(x)

bool                                   current_file_parsing;
CoEvent                                current_file_event;
CurrentFileContext                     current_file;
int64                                  current_file_serial;
int64                                  current_file_done_serial;

Event<const CppFileInfo&>  annotations_done;

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
		CppFileInfo f;
		if(v.item.GetCount()) {
			f.items = pick(v.item[0]);
			f.items.RemoveIf([&](int i) { return f.items[i].line < parsed_file.line_delta; });
			for(AnnotationItem& m : f.items)
				m.line -= parsed_file.line_delta;
		}
		if(v.refs.GetCount()) {
			f.refs = pick(v.refs[0]);
			f.refs.RemoveIf([&](int i) { return f.refs[i].pos.y < parsed_file.line_delta; });
			for(ReferenceItem& m : f.refs)
				m.pos.y -= parsed_file.line_delta;
		}
		Ctrl::Call([&] {
			if(parsed_file.filename == current_file.filename &&
			   parsed_file.real_filename == current_file.real_filename &&
			   parsed_file.includes == current_file.includes &&
			   serial == current_file_serial &&
			   v.item.GetCount()) {
				annotations_done(f);
				FileAnnotation fa;
				fa.defines = parsed_file.defines;
				fa.includes = parsed_file.includes;
				fa.items = pick(f.items);
				fa.time = Time::Low();
				CodeIndex().GetAdd(NormalizePath(parsed_file.real_filename)) = pick(fa);
			}
			current_file_done_serial = serial;
		});
	};

	while(!Thread::IsShutdownThreads()) {
		bool was_parsing;
		do {
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
						DumpDiagnostics(clang.tu); _DBG_
					}
					DoAnnotations();
					was_parsing = true;
				}
				if(Thread::IsShutdownThreads()) break;
				if(clang.tu && serial != done_serial) {
					TIMESTOP("ReParse");
					current_file_parsing = true;
					bool b = clang.ReParse(fn, f.content);
					current_file_parsing = false;
					if(b)
						DoAnnotations();
					was_parsing = true;
				}
			}
		}
		while(was_parsing);
		current_file_event.Wait();
		DLOG("Current file Thread::IsShutdownThreads() " << Thread::IsShutdownThreads());
	}
	DLOG("Current file thread exit");
}

void SetCurrentFile(const CurrentFileContext& ctx, Event<const CppFileInfo&> done)
{
	ONCELOCK {
		MemoryIgnoreNonMainLeaks();
		MemoryIgnoreNonUppThreadsLeaks(); // clangs leaks static memory in threads
		Thread::Start([] { CurrentFileThread(); });
		Thread::AtShutdown([] {
			DLOG("Shutdown current file");
			current_file_event.Broadcast();
		});
	}
	GuiLock __;
	current_file = ctx;
	annotations_done = done;
	current_file_event.Broadcast();
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
