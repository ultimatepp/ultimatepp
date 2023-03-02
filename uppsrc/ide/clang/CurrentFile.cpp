#include "clang.h"

#define LLOG(x) // LOG(x)

bool                                   current_file_parsing;
CoEvent                                current_file_event;
CurrentFileContext                     current_file;
int64                                  current_file_serial;
int64                                  current_file_done_serial;

Event<const CppFileInfo&, const Vector<Diagnostic>&> annotations_done;

CurrentFileContext                     autocomplete_file;

bool                                   do_autocomplete;
bool                                   autocomplete_macros;
Point                                  autocomplete_pos;
int64                                  autocomplete_serial;
Event<const Vector<AutoCompleteItem>&> autocomplete_done;

bool                                   delete_cfc;

struct CurrentFileClang {
	CurrentFileContext parsed_file;
	Clang              clang;
};

static Array<CurrentFileClang> s_cf;

CurrentFileClang& GetCurrentFileClang(const String& filename)
{
	for(int i = 0; i < s_cf.GetCount(); i++)
		if(s_cf[i].parsed_file.filename == filename) {
			s_cf.Move(i, 0); // LRU...
			return s_cf[0];
		}

	return s_cf.GetCount() < ParsedFiles ? s_cf.Add() : s_cf.Top();
}

void CurrentFileDeleteCache()
{
	delete_cfc = true;
}

void ReadAutocomplete(const CXCompletionString& string, String& name, String& signature)
{
	const int chunkCount = clang_getNumCompletionChunks(string);
	for(int j = 0; j < chunkCount; j++) {
		const CXCompletionChunkKind chunkKind = clang_getCompletionChunkKind(string, j);
		String text = FetchString(clang_getCompletionChunkText(string, j));
		if(chunkKind == CXCompletionChunk_Optional)
			for(unsigned i = 0; i < clang_getNumCompletionChunks(string); i++)
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

void DoAnnotations(CurrentFileClang& cfc, int64 serial) {
	{
		GuiLock __;
		current_file_done_serial = serial;
	}
	if(!cfc.clang.tu || !annotations_done) return;
	ClangVisitor v;
	String tpath = current_file.filename;
	if(GetFileExt(tpath) == ".icpp")
		tpath = ForceExt(tpath, ".cpp");
	v.WhenFile = [&] (const String& path) { return path == tpath; };
	v.Do(cfc.clang.tu);
	CppFileInfo f;
	if(v.info.GetCount()) {
		f = pick(v.info[0]);
		f.items.RemoveIf([&](int i) { return f.items[i].pos.y < cfc.parsed_file.line_delta; });
		for(AnnotationItem& m : f.items)
			m.pos.y -= cfc.parsed_file.line_delta;
		f.locals.RemoveIf([&](int i) { return f.locals[i].pos.y < cfc.parsed_file.line_delta; });
		for(AnnotationItem& m : f.locals)
			m.pos.y -= cfc.parsed_file.line_delta;
		f.refs.RemoveIf([&](int i) { return f.refs[i].pos.y < cfc.parsed_file.line_delta; });
		for(ReferenceItem& m : f.refs)
			m.pos.y -= cfc.parsed_file.line_delta;
	}
	
	Vector<Diagnostic> ds;
	Diagnostics(cfc.clang.tu, [&](const String& filename, Point pos, const String& text, bool detail, int severity) {
		Diagnostic& d = ds.Add();
		d.kind = severity;
		d.detail = detail;
		d.path = NormalizePath(filename);
		if(d.path == current_file.filename)
			d.path = current_file.real_filename;
		d.pos = pos;
		d.pos.y -= cfc.parsed_file.line_delta;
		d.text = text;
	});
	Ctrl::Call([&] {
		if(cfc.parsed_file.filename == current_file.filename &&
		   cfc.parsed_file.real_filename == current_file.real_filename &&
		   cfc.parsed_file.includes == current_file.includes &&
		   serial == current_file_serial) {
			annotations_done(f, ds);
			FileAnnotation fa;
			fa.defines = cfc.parsed_file.defines;
			fa.includes = cfc.parsed_file.includes;
			fa.items = pick(f.items);
			fa.refs = pick(f.refs);
			fa.time = Time::Low();
			CodeIndex().GetAdd(NormalizePath(cfc.parsed_file.real_filename)) = pick(fa);
		}
	});
};

void CurrentFileThread()
{
	MemoryIgnoreLeaksBlock __;

	while(!Thread::IsShutdownThreads()) {
		bool was_parsing;
		do {
			was_parsing = false;
			CurrentFileContext f, af;
			int64 serial;
			int64 done_serial;
			int64 aserial;
			bool autocomplete_do;
			bool del_cfc;
			{ // fetch the work to do
				GuiLock __;
				f = current_file;
				af = autocomplete_file;
				serial = current_file_serial;
				done_serial = current_file_done_serial;
				autocomplete_do = do_autocomplete;
				aserial = autocomplete_serial;
				del_cfc = delete_cfc;
				delete_cfc = false;
			}
			if(f.filename.GetCount()) {
				if(del_cfc)
					s_cf.Clear();
				CurrentFileClang& cfc = GetCurrentFileClang(f.filename);
				auto DumpDiagnostics = [&](const char *filename) {
					if(AssistDiagnostics) {
						FileOut out(CacheFile(filename));
						Diagnostics(cfc.clang.tu, out);
					}
				};
				String fn = f.filename;
				if(!IsSourceFile(fn))
					fn.Cat(".cpp");
				if(GetFileExt(fn) == ".icpp")
					fn = ForceExt(fn, ".cpp");
				if(f.filename != cfc.parsed_file.filename || f.real_filename != cfc.parsed_file.real_filename ||
				   f.includes != cfc.parsed_file.includes || f.defines != cfc.parsed_file.defines ||
				   !cfc.clang.tu) {
					cfc.parsed_file = f;
					int tm = msecs();
					current_file_parsing = true;
					cfc.clang.iquote = GetFileFolder(f.real_filename);
					cfc.clang.Parse(fn, f.content, f.includes, f.defines,
					                CXTranslationUnit_PrecompiledPreamble|
					                CXTranslationUnit_CreatePreambleOnFirstParse|
			                        CXTranslationUnit_SkipFunctionBodies|
					                CXTranslationUnit_LimitSkipFunctionBodiesToPreamble|
					                CXTranslationUnit_DetailedPreprocessingRecord|
					                CXTranslationUnit_KeepGoing);
					DumpDiagnostics("parse_errors");
					PutAssist(String()  << cfc.parsed_file.filename<< " parsed in " << msecs() - tm << " ms");
					tm = msecs();
					DoAnnotations(cfc, serial);
					done_serial = serial;
					PutAssist(String() << cfc.parsed_file.filename << " parser output processed in " << msecs() - tm << " ms");
					current_file_parsing = false;
					was_parsing = true;
				}
				if(Thread::IsShutdownThreads()) break;
				if(cfc.clang.tu && autocomplete_do) {
					Vector<AutoCompleteItem> item;
					if(cfc.clang.tu) {
						CXUnsavedFile ufile = { ~fn, ~af.content, (unsigned)af.content.GetCount() };
						CXCodeCompleteResults *results;
						current_file_parsing = true;
						int tm = msecs();
						{
							MemoryIgnoreLeaksBlock __;
							results = clang_codeCompleteAt(cfc.clang.tu, fn, autocomplete_pos.y, autocomplete_pos.x, &ufile, 1,
							                               CXCodeComplete_IncludeMacros);
							DumpDiagnostics("autocomplete_errors");
						}
						PutAssist(String() << cfc.parsed_file.filename << " autocomplete in " << msecs() - tm << " ms");
						// DumpDiagnostics(cfc.clang.tu);
						if(results) {
							int tm = msecs();
							for(unsigned i = 0; i < results->NumResults; i++) {
								const CXCompletionString& string = results->Results[i].CompletionString;
								int kind = results->Results[i].CursorKind;
							//	if(kind == CXCursor_MacroDefinition) // we probably want this only on Ctrl+Space
							//		continue;
								if(kind == CXCursor_NotImplemented)
									continue;
								String name;
								String pretty;
								ReadAutocomplete(string, name, pretty);
								AutoCompleteItem& m = item.Add();
								m.name = name;
								m.parent = FetchString(clang_getCompletionParent(string, NULL));
								m.pretty = CleanupPretty(pretty);
								m.kind = kind;
								m.priority = clang_getCompletionPriority(string);
							}
							{
								MemoryIgnoreLeaksBlock __;
								clang_disposeCodeCompleteResults(results);
							}
							PutAssist(String() << cfc.parsed_file.filename << " autocomplete processed in " << msecs() - tm << " ms");
						}
					}
					Ctrl::Call([&] {
						if(aserial == autocomplete_serial)
							autocomplete_done(item);
					});
					current_file_parsing = false;
					GuiLock __;
					do_autocomplete = false;
				}
				if(serial != done_serial) {
					TIMESTOP("ReParse");
					current_file_parsing = true;
					int tm = msecs();
					bool b = cfc.clang.ReParse(fn, f.content);
					PutAssist(String() << cfc.parsed_file.filename << " reparsed in " << msecs() - tm << " ms");
					tm = msecs();
					if(b) {
						DumpDiagnostics("parse_errors");
						DoAnnotations(cfc, serial);
					}
					PutAssist(String() << cfc.parsed_file.filename << " reparsed output processed in " << msecs() - tm << " ms");
					current_file_parsing = false;
					was_parsing = true;
				}
			}
		}
		while(was_parsing);
		current_file_event.Wait();
		LLOG("Current file Thread::IsShutdownThreads() " << Thread::IsShutdownThreads());
	}
	s_cf.Clear();
	LLOG("Current file thread exit");
}

const char sFakeBuildInfo[] =
R"(
#define bmYEAR   2022
#define bmMONTH  10
#define bmDAY    25
#define bmHOUR   19
#define bmMINUTE 18
#define bmSECOND 32
#define bmTIME   Time(2022, 10, 25, 19, 18, 32)
#define bmMACHINE "DESKTOP-UGRVJFA"
#define bmUSER    "cxl"
#define bmGIT_REVCOUNT "14242"
)";

void SetCurrentFile(const CurrentFileContext& ctx, Event<const CppFileInfo&, const Vector<Diagnostic>&> done)
{
	if(!HasLibClang())
		return;

	ONCELOCK {
		String path = CacheFile("fake_build_info") + "/" + "build_info.h";
		RealizePath(path);
		SaveChangedFile(path, sFakeBuildInfo);

		MemoryIgnoreNonMainLeaks();
		MemoryIgnoreNonUppThreadsLeaks(); // clangs leaks static memory in threads
		Thread t;
		t.StackSize(8192*1024);
		t.Run([] { CurrentFileThread(); });
		t.Detach();
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
	if(!HasLibClang())
		return;

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

bool IsAutocompleteParsing()
{
	return do_autocomplete;
}

void CancelAutoComplete()
{
	GuiLock __;
	autocomplete_done.Clear();
	do_autocomplete = false;
}
