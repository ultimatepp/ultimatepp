#include "clang.h"

#define LLOG(x)
#define LTIMESTOP(x) TIMESTOP(x)

String FetchString(CXString cs)
{
	if(!HasLibClang())
		return Null;
	String result = clang_getCString(cs);
	clang_disposeString(cs);
	return result;
}

String GetCursorKindName(CXCursorKind cursorKind)
{
	if(!HasLibClang())
		return Null;
	return FetchString(clang_getCursorKindSpelling(cursorKind));
}

String GetCursorSpelling(CXCursor cursor)
{
	if(!HasLibClang())
		return Null;
	return FetchString(clang_getCursorSpelling(cursor));
}

String GetTypeSpelling(CXCursor cursor)
{
	if(!HasLibClang())
		return Null;
	return FetchString(clang_getTypeSpelling(clang_getCursorType(cursor)));
}

String GetClangInternalIncludes()
{
	INTERLOCKED {
		static String includes;
		ONCELOCK {
			String dummy = ConfigFile("dummy.cpp");
			Upp::SaveFile(dummy, String());
			String h = Sys(
			#ifdef PLATFORM_WIN32
					GetExeDirFile("bin/clang/bin/c++") +
			#else
					"clang++"
			#endif
					" -v -x c++ -E " + dummy
			);
			DeleteFile(dummy);
			h.Replace("\r", "");
			Vector<String> ln = Split(h, '\n');
			for(int i = 0; i < ln.GetCount(); i++) {
				String dir = TrimBoth(ln[i]);
				if(DirectoryExists(dir))
					MergeWith(includes, ";", NormalizePath(dir));
			}
		}
		return includes;
	}
	return String();
}

void Clang::Dispose()
{
	if(!HasLibClang())
		return;
	if(tu) {
		INTERLOCKED { // Otherwise dispose takes much longer, probably due to clang allocator lock contention
//			TIMESTOP("clang_disposeTranslationUnit");
			clang_disposeTranslationUnit(tu);
		}
	}
	tu = nullptr;
}

bool Clang::Parse(const String& filename_, const String& content,
                  const String& includes_, const String& defines,
                  dword options,
                  const String& filename2, const String& content2)
{
	if(!HasLibClang())
		return false;

	MemoryIgnoreLeaksBlock __;
	if(!index) return false;
	
	Dispose();

	String cmdline;
	
	String filename = filename_;
	if((options & PARSE_FILE) && GetFileExt(filename) == ".icpp") {
		String src = "#include \"" + filename + "\"";
		filename = CacheFile(GetFileName(filename) + "$" + SHA1String(src) + ".cpp");
		SaveChangedFile(filename, src);
	}

	cmdline << filename << " -DflagDEBUG -DflagDEBUG_FULL -DflagMAIN -DflagCLANG ";
	
	if(IsCppSourceFile(filename))
		cmdline << " -std=c++14 -xc++ " << LibClangCommandLine() << " ";
	else
		cmdline << " -xc " << LibClangCommandLineC() << " ";
	
	String cmdline0 = cmdline;

	cmdline << RedefineMacros() << " ";
	
	String includes = includes_;
	MergeWith(includes, ";", GetClangInternalIncludes());

	Vector<String> args;

	for(const String& s : Split(includes, ';'))
		args.Add("-I" + s);

	if(iquote.GetCount()) // path to real_filename for #include "xxx" handling
		args.Add("-I" + iquote);
	
	args.Add("-I" + CacheFile("fake_build_info"));
	
	for(const String& s : Split(defines + ";CLANG", ';'))
		args.Add("-D" + s);

	args.Append(Split(cmdline, ' '));
	
	Vector<const char *> argv;

	for(const String& s : args)
		argv.Add(~s);
	
	CXUnsavedFile ufile[2] = {
		{ ~filename, ~content, (unsigned)content.GetCount() },
		{ ~filename2, ~content2, (unsigned)content2.GetCount() },
	};
	tu = clang_parseTranslationUnit(index, nullptr, argv, argv.GetCount(),
	                                options & PARSE_FILE ? nullptr : ufile,
	                                options & PARSE_FILE ? 0 : (filename2.GetCount() ? 2 : 1),
	                                options);

	if(!tu)
		PutAssist("Failed commandline: " + cmdline0);
//	DumpDiagnostics(tu);
	
	return tu;
}

bool Clang::ReParse(const String& filename, const String& content,
                    const String& filename2, const String& content2)
{
	if(!HasLibClang())
		return false;

	MemoryIgnoreLeaksBlock __;
	CXUnsavedFile ufile[2] = {
		{ ~filename, ~content, (unsigned)content.GetCount() },
		{ ~filename2, ~content2, (unsigned)content2.GetCount() },
	};
	if(clang_reparseTranslationUnit(tu, filename2.GetCount() ? 2 : 1, ufile, 0)) {
		Dispose();
		return false;
	}
	return true;
}

Clang::Clang()
{
	if(!HasLibClang())
		return;

	MemoryIgnoreLeaksBlock __;
	index = clang_createIndex(0, 0);
}

Clang::~Clang()
{
	if(!HasLibClang())
		return;

	MemoryIgnoreLeaksBlock __;
	Dispose();
	clang_disposeIndex(index);
}

void Diagnostics(CXTranslationUnit tu, Event<const String&, Point, const String&, bool, int> out)
{
	if(!HasLibClang())
		return;

	unsigned num_diagnostics = clang_getNumDiagnostics(tu);

	for (unsigned i = 0; i < num_diagnostics; ++i) {
		CXDiagnostic diagnostic = clang_getDiagnostic(tu, i);
		auto Dump = [&](CXDiagnostic diagnostic, bool detail) {
			CXFile file;
			unsigned line;
			unsigned column;
			unsigned offset;
			CXSourceLocation location = clang_getDiagnosticLocation(diagnostic);
			clang_getExpansionLocation(location, &file, &line, &column, &offset);
			out(FetchString(clang_getFileName(file)), Point(column - 1, line - 1), FetchString(clang_getDiagnosticSpelling(diagnostic)),
			                detail, clang_getDiagnosticSeverity(diagnostic));
		};
		Dump(diagnostic, false);
		CXDiagnosticSet set = clang_getChildDiagnostics(diagnostic);
		int n = clang_getNumDiagnosticsInSet(set);
		for(int i = 0; i < n; i++) {
			CXDiagnostic d = clang_getDiagnosticInSet(set, i);
			Dump(d, true);
			clang_disposeDiagnostic(d);
		}
		clang_disposeDiagnostic(diagnostic);
	}
}

void Diagnostics(CXTranslationUnit tu, Stream& out)
{
	Diagnostics(tu, [&](const String& filename, Point pos, const String& text, bool detail, int severity) {
		if(detail)
			out << "\t";
		if(IsWarning(severity))
			out << "WARNING: ";
		else
		if(IsError(severity))
			out << "ERROR: ";
		else
			out << "NOTE: ";
		out << filename << " (" << pos.y + 1 << "): " << text << "\r\n";
	});
}