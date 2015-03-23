#ifndef _cpp_cpp_h_
#define _cpp_cpp_h_

#include <Core/Core.h>

#include <CppBase/CppBase.h>

using namespace Upp;

struct CppMacro : Moveable<CppMacro>, DeepCopyOption<CppMacro> {
	String        body;
	Index<String> param;
	bool          variadic;
	
	String Define(const char *s);

	String Expand(const Vector<String>& p) const;
	
	String ToString() const;
	
	CppMacro()    { variadic = false; }
	rval_default(CppMacro);
	CppMacro(const CppMacro& s, int) { body = s.body, param = clone(s.param); variadic = s.variadic; }
};

enum PPItemType {
	PP_DEFINE,
	PP_INCLUDE,
	PP_USING,
	PP_NAMESPACE,
	PP_NAMESPACE_END
};

struct PPItem {
	int      type;
	String   id;
	CppMacro macro;
};

struct PPFile {
	FileTime       filetime;
	Array<PPItem>  item;
	Index<String>  includes;
	
	void Parse(Stream& in);
	
	void Dump() const;

private:
	void CheckEndNamespace(Vector<int>& namespace_block, int level);
};

const PPFile& GetPPFile(const char *path);

String GetIncludePath(const String& s, const String& filedir, const String& include_path);

bool   IncludesFile(const String& include, const String& path, const String& include_path);

struct Cpp {
	bool                        incomment;
	
	String                      path;
	String                      filedir;
	String                      include_path;
	
	Index<String>               header;
	VectorMap<String, CppMacro> macro;
	Index<String>               usedmacro;
	Index<String>               notmacro;
	
	CppBase                     base;
	
	Cpp                        *parent;
	
	void SyncSet();
	
	String   Define(const char *s);

	static const char *SkipString(const char *s);
	void ParamAdd(Vector<String>& param, const char *b, const char *e);

	String Expand(const char *s);
	void   Include(const char *s);
	String GetIncludePath(const char *s);
	
	void   Parse(StringBuffer& result);

	void   Do(Stream& in, Index<String>& header);
	void   DoCpp(Stream& in, Index<String>& header);
	
	Callback3<const String&, int, const String&> WhenError;
	
	void AddError(int ln, const String& s) { WhenError(path, ln, s); }
	
	Cpp() { parent = NULL; }
	
	typedef Cpp CLASSNAME;
};

String Preprocess(const String& filename, const String& include_path);

String Preprocess(const String& sourcefile, Stream& in, const String& surrogatefile,
                  const String& include_path);

#endif
