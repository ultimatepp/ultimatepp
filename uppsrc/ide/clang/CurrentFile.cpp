#include "clang.h"

CurrentFileContext              current_file;
bool                            do_autocomplete;
Point                           autocomplete_pos;
int64                           autocomplete_serial;
Event<const Vector<AutoCompleteItem>&> autocomplete_done;

void CurrentFileThread()
{
	CurrentFileContext parsed_file;
	CXTranslationUnit tu = nullptr;
	int64 serial;

	auto DisposeTU = [&] {
		if(tu) clang_disposeTranslationUnit(tu);
		tu = nullptr;
	};

	while(!Thread::IsShutdownThreads()) {
		CurrentFileContext f;
		bool autocomplete_do;
		{
			GuiLock __;
			f = current_file;
			serial = autocomplete_serial;
			autocomplete_do = do_autocomplete;
		}
		String fn = f.filename;
		if(!IsSourceFile(fn))
			fn.Cat(".cpp");
		if(f.filename != parsed_file.filename || f.includes != parsed_file.includes) {
			parsed_file = f;
			DisposeTU();
			String cmdline;
			cmdline << fn << " -DflagDEBUG -DflagDEBUG_FULL -DflagBLITZ -DflagWIN32 -DflagMAIN -DflagGUI -xc++ -std=c++17 ";
			for(String s : Split(f.includes, ';'))
				cmdline << " -I" << s;
			TIMESTOP("Translate");
			tu = Clang(cmdline, { { ~fn, f.content } },
			           CXTranslationUnit_PrecompiledPreamble|
			           CXTranslationUnit_CreatePreambleOnFirstParse|
			           CXTranslationUnit_KeepGoing|
			           CXTranslationUnit_RetainExcludedConditionalBlocks);
		}
		if(Thread::IsShutdownThreads()) break;
		if(autocomplete_do && tu) {
			CXUnsavedFile ufile = { ~fn, ~f.content, (unsigned)f.content.GetCount() };
			CXCodeCompleteResults *results;
			{
				TIMESTOP("clang_codeCompleteAt");
				results = clang_codeCompleteAt(tu, fn, autocomplete_pos.y, autocomplete_pos.x, &ufile, 1, 0);
			}
//			DumpDiagnostics(tu);
			if(results) {
				Vector<AutoCompleteItem> item;
				for(int i = 0; i < results->NumResults; i++) {
					const CXCursorKind kind = results->Results[i].CursorKind;
					const CXCompletionString& string = results->Results[i].CompletionString;
					if(kind == CXCursor_MacroDefinition) // we probably want this only on Ctrl+Space
						continue;
					String name;
					String signature;
					int    args = -1;
					const int chunkCount = clang_getNumCompletionChunks(string);
					for(int j = 0; j < chunkCount; j++) {
						const CXCompletionChunkKind chunkKind = clang_getCompletionChunkKind(string, j);
						String text = FetchString(clang_getCompletionChunkText(string, j));
						if (chunkKind == CXCompletionChunk_TypedText) {
							name = text;
							signature << text;
							args = signature.GetCount();
						}
						else
						if (chunkKind == CXCompletionChunk_Placeholder)
							signature << text;
						else {
							signature << text;
							if (chunkKind == CXCompletionChunk_ResultType) {
								signature << ' ';
							}
						}
					}
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
	}
	DisposeTU();
}

void StartCurrentFileParserThread()
{
	Thread::StartNice([] { CurrentFileThread(); });
}

void SetCurrentFile(const CurrentFileContext& ctx)
{
	GuiLock __;
	current_file = ctx;
}

void StartAutoComplete(const CurrentFileContext& c, int line, int column, Event<const Vector<AutoCompleteItem>&> done)
{
	GuiLock __;
	static int64 serial;
	SetCurrentFile(c);
	autocomplete_pos.y = line;
	autocomplete_pos.x = column;
	do_autocomplete = true;
	autocomplete_serial = serial++;
	autocomplete_done = done;
}

void CancelAutoComplete()
{
	GuiLock __;
	autocomplete_done.Clear();
	do_autocomplete = false;
}
