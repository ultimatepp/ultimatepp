#ifndef _cpp_cpp_h_
#define _cpp_cpp_h_

#include <Core/Core.h>

using namespace Upp;

struct CppMacro : Moveable<CppMacro> {
	String        body;
	Index<String> param;
	bool          variadic;
	bool          flag; // used when expanding macros to mark those use; MT incompatible (!)

	String Expand(const Vector<String>& p);
	
	String ToString() const;
	
	CppMacro()    { flag = variadic = false; }
};

struct Cpp {
	bool                        incomment;
	VectorMap<String, CppMacro> macro;
	
	String                      filedir;
	String                      include_path;
	int                         level;

	void   Define(const char *s);

	static const char *SkipString(const char *s);
	void ParamAdd(Vector<String>& param, const char *b, const char *e);

	String Expand(const char *s);
	void   Include(const char *s);
	String GetIncludePath(const char *s);
	
	String   Preprocess(Stream& in, bool needresult = true);
};

#endif
