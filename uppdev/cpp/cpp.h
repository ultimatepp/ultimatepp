#ifndef _cpp_cpp_h_
#define _cpp_cpp_h_

#include <Core/Core.h>

using namespace Upp;

class CT {
public:
  struct ST : Moveable<ST> {
    int x, y;
  };
};

struct CppMacro : Moveable<CppMacro> {
	String        body;
	Index<String> param;
	bool          flag;

	String Expand(const Vector<String>& p);
	
	CppMacro()    { flag = false; }
};

struct Cpp {
	VectorMap<String, CppMacro> macro;

	void   Define(const char *s);

	static const char *SkipString(const char *s);
	void ParamAdd(Vector<String>& param, const char *b, const char *e);

	String Expand(const char *s);
};


#endif
