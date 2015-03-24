#ifndef _cpp_cpp_h_
#define _cpp_cpp_h_

#include <Core/Core.h>

#include <CppBase/CppBase.h>

using namespace Upp;

struct CppMacro : Moveable<CppMacro>, DeepCopyOption<CppMacro> {
	String        body;
	Index<String> param;
	bool          variadic;

	String Expand(const Vector<String>& p) const;
	
	String ToString() const;
	
	CppMacro()    { variadic = false; }
	rval_default(CppMacro);
	CppMacro(const CppMacro& s, int) { body = s.body, param = clone(s.param); variadic = s.variadic; }
};

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
	
	void   Parse();

	void   Do(Stream& in, Index<String>& header);
	void   DoCpp(Stream& in, Index<String>& header);
	
	Callback3<const String&, int, const String&> WhenError;
	
	void AddError(int ln, const String& s) { WhenError(path, ln, s); }
	
	Cpp() { parent = NULL; }
	
	typedef Cpp CLASSNAME;
};

String Preprocess(const String& filename, const String& include_path);

#endif
