#include "CppBase.h"
#include "Internal.h"

namespace Upp {

bool HasCPPFileKeyword(const String& path, const String& id)
{
	const PPFile& f = GetPPFile(NormalizeSourcePath(path));
	return FindIndex(f.keywords, id) >= 0;
}

String GetDependeciesMD5(const String& path, Index<String>& visited)
{
	Cpp pp;
	FileIn in(path);
	String npath = NormalizeSourcePath(path);
	pp.Preprocess(npath, in, GetMasterFile(npath), true);
	String md5 = pp.GetDependeciesMd5(GetPPFile(path).keywords);
	visited = pick(pp.visited);
	return md5;
}

String GetPPMD5(const String& fn)
{
	return GetPPFile(fn).md5sum;
}

void Parse(CppBase& base,
           const String& src, int file, int filetype, const String& path,
           Event<int, const String&> error, const Vector<String>& namespace_stack,
	       const Index<String>& namespace_using)
{
	StringStream pin(src);
	Parser p;
	p.Do(pin, base, file, filetype, GetFileName(path), error, Vector<String>(),
	     namespace_stack, namespace_using);
}

void PreprocessParse(CppBase& base, Stream& in, int file, const String& path,
                     Event<int, const String&> error)
{
	Cpp cpp;
	cpp.Preprocess(path, in, GetMasterFile(path));
	int filetype = decode(ToLower(GetFileExt(path)), ".h", FILE_H, ".hpp", FILE_HPP,
	                       ".cpp", FILE_CPP, ".icpp", FILE_CPP, ".c", FILE_C, FILE_OTHER);
	Parse(base, cpp.output, file, filetype, path, error, cpp.namespace_stack, cpp.namespace_using);
}

String PreprocessCpp(const String& src, const String& path)
{
	Cpp cpp;
	StringStream in(src);
	String p = NormalizeSourcePath(path);
	cpp.Preprocess(p, in, GetMasterFile(p));
	return cpp.output;
}

ParserContext AssistParse(const String& src, const String& path_, Event<int, const String&> error)
{
	String path = NormalizeSourcePath(path_);
	Cpp cpp;
	StringStream ss(src);
	cpp.Preprocess(path, ss, GetMasterFile(path));
	Parser parser;
	parser.dobody = true; // will do bodies and not write anything to base
	StringStream pin(cpp.output);
	CppBase dummy;
	parser.Do(pin, dummy, Null, Null, GetFileTitle(path), error,
	          Vector<String>(), cpp.namespace_stack, cpp.namespace_using); // needs CodeBase to identify type names
	return pick(parser);
}

void SimpleParse(CppBase& cpp, const String& txt, const String& cls)
{
	Parser parser; // we do not need/want preprocessing here
	StringStream ss(txt);
	parser.Do(ss, cpp, Null, Null, Null, CNULL, Split(cls, ':'),
	          Vector<String>(), Index<String>());
}

};