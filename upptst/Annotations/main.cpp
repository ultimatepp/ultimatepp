#include <CtrlLib/CtrlLib.h>

// this is for testing TheIDE annotations

using namespace Upp;
/*
	STRUCT,
	STRUCTTEMPLATE,
	TYPEDEF,
	CONSTRUCTOR,
	DESTRUCTOR,
	FUNCTION,
	INSTANCEFUNCTION,
	CLASSFUNCTION,
	FUNCTIONTEMPLATE,
	INSTANCEFUNCTIONTEMPLATE,
	CLASSFUNCTIONTEMPLATE,
	INLINEFRIEND,
	VARIABLE,
	INSTANCEVARIABLE,
	CLASSVARIABLE,
	ENUM,
	MACRO,
	FRIENDCLASS,
	NAMESPACE,
	FLAGTEST,
*/

void *Bar();
int& Bar2();

#define MACRO

#define FN_MACRO(x)

enum Enum {
	EnumItem,
	EnumItem2 = 2,

};

void Function();

void Function()
{
}

String global_variable;
static String static_variable;

struct Struct {
	enum { NAME };

	operator String();

	String attribute;
	static String meta_attribute;
	
	void Method(String param, int foo);
	static void ClassMethod(int param);

	void Method();
	static void ClassMethod();
	
	Struct();
	~Struct();
};

String Struct::meta_attribute;

void Struct::ClassMethod(int param)
{
}

void Struct::Method()
{
}

void Struct::ClassMethod()
{
}

Struct::Struct()
{
}

Struct::~Struct()
{
}

class Class {
	int x;
	
	Class();
	~Class();
};

union Union {
	int x, y;
};

template <class T>
struct TemplateFoo {
	int y;
};

struct Foo { int bar; };

typedef Foo Typedef;

using UsingTypedef = Foo;

namespace Namespace {
	#define MACRO
	
	#define FN_MACRO(x)
	
	enum Enum {
		EnumItem,
		EnumItem2 = 2,
	
	};
	
	void Function();
	
	void Function()
	{
	}
	
	struct Struct {
		String attribute;
		static String meta_attribute;
		
		void Method(String param, int foo);
		void ClassMethod(int param);
	
		void Method();
		void ClassMethod();
		
		Struct();
		~Struct();
	};
	
	String Struct::meta_attribute;
	
	void Struct::ClassMethod(int param)
	{
	}
	
	void Struct::Method()
	{
	}
	
	void Struct::ClassMethod()
	{
	}
	
	Struct::Struct()
	{
	}
	
	Struct::~Struct()
	{
	}
	
	class Class {
		int x;
		
		Class();
		~Class();
	};
	
	union Union {
		int x, y;
	};
	
	template <class T>
	struct TemplateFoo {
		int y;
	};
	
	typedef Foo Typedef;
	
	using UsingTypedef = Foo;
};

GUI_APP_MAIN
{
}
