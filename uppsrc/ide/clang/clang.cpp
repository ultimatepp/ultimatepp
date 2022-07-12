#include "clang.h"

#define LLOG(x)

String FetchString(CXString cs)
{
	String result = clang_getCString(cs);
	clang_disposeString(cs);
	return result;
}

String GetCursorKindName(CXCursorKind cursorKind)
{
	return FetchString(clang_getCursorKindSpelling(cursorKind));
}

String GetCursorSpelling(CXCursor cursor)
{
	return FetchString(clang_getCursorSpelling(cursor));
}

String GetTypeSpelling(CXCursor cursor)
{
	return FetchString(clang_getTypeSpelling(clang_getCursorType(cursor)));
}

String SourceLocation::ToString() const
{
	return String() << filename << " (" << line << ":" << column << ")";
}

#ifdef PLATFORM_WIN32
String GetClangInternalIncludes()
{
	static String includes;
	ONCELOCK {
		String dummy = ConfigFile("dummy.cpp");
		Upp::SaveFile(dummy, String());
		String h = Sys(GetExeDirFile("bin/clang/bin/c++") + " -v -x c++ -E " + dummy);
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
#endif

SourceLocation::SourceLocation(CXSourceLocation location)
{
	CXFile file;
	unsigned line_;
	unsigned column_;
	unsigned offset_;
	clang_getExpansionLocation(location, &file, &line_, &column_, &offset_);
	line = line_;
	column = column_;
	offset = offset_;
	filename = FetchString(clang_getFileName(file));
}

void Clang::Dispose()
{
	if(tu) clang_disposeTranslationUnit(tu);
	tu = nullptr;
}

bool Clang::Parse(const String& filename, const String& content, const String& includes_, const String& defines, dword options)
{
	if(!index) return false;

	String cmdline;

	cmdline << filename << " -DflagDEBUG -DflagDEBUG_FULL -DflagBLITZ -DflagWIN32 -DflagMAIN -DflagGUI -xc++ -std=c++17 ";
	cmdline << RedefineMacros();
	
	String includes = includes_;
#ifdef PLATFORM_WIN32
	MergeWith(includes, ";", GetClangInternalIncludes());
#endif

	Vector<String> args;
	for(const String& s : Split(includes, ';'))
		args.Add("-I" + s);

	for(const String& s : Split(defines + ";CLANG", ';'))
		args.Add("-D" + s);

	args.Append(Split(cmdline, ' '));
	
	Vector<const char *> argv;

	for(const String& s : args)
		argv.Add(~s);
	
	CXUnsavedFile ufile = { ~filename, ~content, (unsigned)content.GetCount() };

	tu = clang_parseTranslationUnit(index, nullptr, argv, argv.GetCount(), &ufile, 1, options);

//	DumpDiagnostics(tu);
	
	return tu;
}

bool Clang::ReParse(const String& filename, const String& content)
{
	CXUnsavedFile ufile = { ~filename, ~content, (unsigned)content.GetCount() };
	if(clang_reparseTranslationUnit(tu, 1, &ufile, 0)) {
		Dispose();
		return false;
	}
	return true;
}

Clang::Clang()
{
	index = clang_createIndex(0, 0);
}

Clang::~Clang()
{
	Dispose();
	clang_disposeIndex(index);
}

void DumpDiagnostics(CXTranslationUnit tu)
{
	size_t num_diagnostics = clang_getNumDiagnostics(tu);

	for (size_t i = 0; i < num_diagnostics; ++i) {
		CXDiagnostic diagnostic = clang_getDiagnostic(tu, i);
		auto Dump = [&](CXDiagnostic diagnostic) {
			CXFile file;
			unsigned line;
			unsigned column;
			unsigned offset;
			CXSourceLocation location = clang_getDiagnosticLocation(diagnostic);
			clang_getExpansionLocation(location, &file, &line, &column, &offset);
			LOG(FetchString(clang_getFileName(file)) << " (" << line << ":" << column << ") " <<
				FetchString(clang_getDiagnosticSpelling(diagnostic)));
		};
		Dump(diagnostic);
	#if 0
		CXDiagnosticSet set = clang_getChildDiagnostics(diagnostic);
		int n = clang_getNumDiagnosticsInSet(set);
		for(int i = 0; i < n; i++) {
			CXDiagnostic d = clang_getDiagnosticInSet(set, i);
			Dump(d);
			clang_disposeDiagnostic(d);
		}
	#endif
		clang_disposeDiagnostic(diagnostic);
	}
}