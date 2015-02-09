#ifndef _cpp_cpp_h_
#define _cpp_cpp_h_

#include <Core/Core.h>

#include <CppBase/CppBase.h>

using namespace Upp;

struct CppMacro : Moveable<CppMacro> {
	String        body;
	Index<String> param;
	bool          variadic;

	String Expand(const Vector<String>& p) const;
	
	String ToString() const;
	
	CppMacro()    { variadic = false; }
};

struct CppMacroRecord {
	Time                        last_write;
	VectorMap<String, CppMacro> macro;
	Index<String>               includes;
	
	CppMacroRecord() { last_write = Time::Low(); }
};

const VectorMap<String, CppMacro> *GetFileMacros(const String& filepath);

struct Cpp {
	bool                        incomment;
	
	String                      filedir;
	String                      include_path;
	
	VectorMap<String, CppMacro> used_macro;
	Index<String>               not_macro;
	
	Index<String>               header;
	VectorMap<String, CppMacro> macro0;
	VectorMap<String, CppMacro> macro;
	Index<String>               notmacro;
	Index<String>               usedmacro;
	
	void SyncSet();
	
	Vector<const VectorMap<String, CppMacro> *> macro_set;

	String   Define(const char *s);

	static const char *SkipString(const char *s);
	void ParamAdd(Vector<String>& param, const char *b, const char *e);

	String Expand(const char *s);
	void   Include(const char *s);
	String GetIncludePath(const char *s);
	
	String  Do(Stream& in, Index<String>& header);
};


String Preprocess(const String& filename, const String& include_path);

#endif
