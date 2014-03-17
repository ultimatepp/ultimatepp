#include <Core/Core.h>

using namespace Upp;

String If(String s, String txt)
{
	return IsNull(s) ? String() : txt;
}

/*
#ifdef CPP_11
	Callback& operator=(const std::function<void ()>& fn) { Release(); action = new LambdaCallbackAction(fn); return *this; }
#endif
*/

void CallbackGen(String name, String rettype, int n, String extension, String atest = Null)
{
	LOG("// -----------------------------------------------------------");
	String classdef, classlist, paramdef, paramlist;
	for(int i = 1; i <= n; i++) {
		if(i > 1) {
			classdef << ", ";
			classlist << ", ";
			paramdef << ", ";
			paramlist << ", ";
		}
		classdef << Format("class P%d", i);
		classlist << Format("P%d", i);
		paramdef << Format("P%d p%d", i, i);
		paramlist << Format("p%d", i);
	}
	String cl_list = If(classlist, "<" + classlist + ">");
	String cl_temp = String("template <class OBJECT, class METHOD").Cat() << If(classdef, ", " + classdef) << ">";
	String return_ = rettype == "void" ? "" : "return ";
	String name_cl = name + cl_list;
	LOG("");
	if(!IsNull(classdef))
		LOG("template <" << classdef << ">");
	LOG("struct " << name << "Action {");
	LOGBEGIN();
		LOG("Atomic  count;");
		LOG("");
		LOG("virtual " << rettype << " Execute(" + paramdef + ") = 0;");
		LOG("virtual bool IsValid() const { return true; }");
//		LOG("virtual bool IsEqual(const " << name << "Action *other) const = 0;");
//		LOG("virtual unsigned GetHashValue() const = 0;");
	  	LOG("");
		LOG(name << "Action()          { count = 1; }");
		LOG("virtual ~" << name << "Action() {}");
	LOGEND();
	LOG("};");
	LOG("");

	LOG("#ifdef HAS_LAMBDA");
	if(!IsNull(classdef))
		LOG("template <" << classdef << ">");
	LOG("struct Lambda" << name << " : public " << name << "Action" << cl_list + " {");
	LOGBEGIN();
		LOG("std::function<" << rettype << " (" << classlist << ")> fn;");
		if(rettype == "void")
			LOG("virtual void Execute(" << paramdef << ") { fn(" << paramlist << "); }");
		else
			LOG("virtual " << rettype << " Execute(" << paramdef << ") { return fn(" << paramlist << "); }");
		LOG("");
		LOG("Lambda" << name << "(std::function<" << rettype << " (" << classlist << ")> fn) : fn(fn) {}");
		LOGEND();
	LOG("};");
	LOG("#endif");
	LOG("");

#if 0
	LOG(cl_temp);
	LOG("struct " << name << "MethodActionPte : public " << name << "Action" << cl_list + " {");
	LOGBEGIN();
		LOG("Ptr<OBJECT>  object;");
		LOG("METHOD       method;");
		LOG("");
		if(rettype == "void")
			LOG(rettype << " Execute(" << paramdef << ") { if(object) (object->*method)(" << paramlist << "); }");
		else
			LOG(rettype << " Execute(" << paramdef << ") { return object ? (object->*method)(" << paramlist << ") : false; }");
		LOG("bool IsValid() const { return object; }");
//		LOG("bool IsEqual(const " << name << "Action" << cl_list << "  *other) const {");
//		LOGBEGIN();
//			LOG("const " << name << "MethodActionPte *q = dynamic_cast<const " << name <<
//			    "MethodActionPte *>(other);");
//			LOG("return q ? q->object == object && q->method == method : false;");
//		LOGEND();
//		LOG("}");
//		LOG("unsigned GetHashValue() const {");
//		LOG("\treturn (unsigned)(uintptr_t)~object ^ (unsigned)brutal_cast<uintptr_t>(method);");
//		LOG("}");
		LOG("");
		LOG(name << "MethodActionPte(OBJECT *object, METHOD method) "
		    ": object(object), method(method) {}");
	LOGEND();
	LOG("};");
	LOG("");

	LOG(cl_temp);
	LOG("struct " << name << "MethodAction : public " << name << "Action" << cl_list + " {");
	LOGBEGIN();
		LOG("OBJECT  *object;");
		LOG("METHOD   method;");
		LOG("");
		LOG(rettype << " Execute(" << paramdef << ") { " <<
		    return_ << "(object->*method)(" << paramlist << "); }");
/*		LOG("bool IsEqual(const " << name << "Action" << cl_list << "  *other) const {");
		LOGBEGIN();
			LOG("const " << name << "MethodAction *q = dynamic_cast<const " << name <<
			    "MethodAction *>(other);");
			LOG("return q ? q->object == object && q->method == method : false;");
		LOGEND();
		LOG("}");
		LOG("unsigned GetHashValue() const {");
		LOG("\treturn (unsigned)(uintptr_t)object ^ (unsigned)brutal_cast<uintptr_t>(method);");
		LOG("}");
*/		LOG("");
		LOG(name << "MethodAction(OBJECT *object, METHOD method) "
		    ": object(object), method(method) {}");
	LOGEND();
	LOG("};");
	LOG("");

	if(!IsNull(classdef))
		LOG("template <" << classdef << ">");
	LOG("struct " << name << "FnAction : public " << name << "Action" << cl_list << " {");
	LOGBEGIN();
		LOG(rettype << " (*fn)(" << paramdef << ");");
		LOG("");
		LOG(rettype << " Execute(" << paramdef << ") { " << return_ << "(*fn)(" <<
		    paramlist << "); }");
/*		LOG("bool IsEqual(const " << name << "Action" << cl_list << " *other) const {");
		LOGBEGIN();
			LOG("const " << name << "FnAction *q = dynamic_cast<const " << name <<
		    	"FnAction *>(other);");
			LOG("return q ? q->fn == fn : false;");
		LOGEND();
		LOG("}");
		LOG("unsigned GetHashValue() const {");
		LOG("\treturn (unsigned)(uintptr_t)fn;");
		LOG("}");
*/		LOG("");
		LOG(name << "FnAction(" << rettype << " (*fn)(" << paramdef << ")) : fn(fn) {}");
	LOGEND();
	LOG("};");
	LOG("");
#endif

	if(!IsNull(classdef))
		LOG("template <" << classdef << ">");
	LOG("class " << name << " : Moveable< " << name_cl << " > {");
	LOGBEGIN();
		LOG(name << "Action" << cl_list << " *action;");
 		LOG("");
		LOG("void Retain() const { if(action " << atest << ") AtomicInc(action->count); }");
		LOG("void Release()      { if(action " << atest << " && AtomicDec(action->count) == 0) delete action; }");
  		LOG("");
		LOG("bool operator==(const " << name << "&);");
		LOG("bool operator!=(const " << name << "&);");
  		LOG("");
  	LOGEND();
	LOG("public:");
	LOGBEGIN();
		LOG("typedef " << name << " CLASSNAME;");
		LOG("");
		LOG(name << "& operator=(const " << name << "& c);");
		LOG(name << "(const " << name << "& c);");
		LOG("void Clear()        { Release(); action = NULL; }");
		LOG("");
//		LOG("unsigned GetHashValue() const            { return action->GetHashValue(); }");
		LOG("");
		LOG(extension);
		LOG("");
		LOG("explicit " << name << "(" << name << "Action " << cl_list <<
		    " *newaction) { action = newaction; }");
//		LOG(name << "& operator=(" << name << "Action" << cl_list <<
//		    " *newaction) { action = newaction; return *this; }");
		LOG(name << "() { action = NULL; }");
		LOG(name << "(_CNULL) { action = NULL; }");
		LOG("~" << name << "();");
	LOGEND();
	LOG("#ifdef HAS_LAMBDA");
	LOGBEGIN();
		LOG(name << "& operator=(std::function<" << rettype << " (" << classlist << ")> l) { Clear(); action = new Lambda" << name << "<" << classlist << ">(l); return *this; }");
	LOGEND();
	LOG("#endif");
	LOGBEGIN();
		LOG("");
		LOG("static " << name << " Empty() { return CNULL; }");
		LOG("");
//		LOG("friend bool operator==(const " << name << "& a, const " << name << "& b)");
//		LOG("\t{ return a.action ? a.action->IsEqual(b.action) : !b.action; }");
//		LOG("friend bool operator!=(const " << name << "& a, const " << name << " & b)");
//		LOG("\t{ return !(a == b); }");
	LOGEND();
	LOG("};");
	LOG("");

#if 0
	LOG(cl_temp);
	LOG(name_cl << " pteback(OBJECT *object, " << rettype <<
	    " (METHOD::*method)(" << paramdef << ")) {");
	LOG("\treturn " << name_cl <<
		"(new " << name << "MethodActionPte<OBJECT, " << rettype << " (METHOD::*)(" <<
		paramdef << ")" << If(classlist, ", " + classlist) << ">(object, method));");
	LOG("}");
	LOG("");

	LOG(cl_temp);
	LOG(name_cl << " callback(OBJECT *object, " << rettype <<
	    " (METHOD::*method)(" << paramdef << ")) {");
	LOG("\treturn " << name_cl <<
		"(new " << name << "MethodAction<OBJECT, " << rettype << " (METHOD::*)(" <<
		paramdef << ")" << If(classlist, ", " + classlist) << ">(object, method));");
	LOG("}");
	LOG("");

	LOG(cl_temp);
	LOG(name_cl << " callback(const OBJECT *object, " << rettype <<
	    " (METHOD::*method)(" << paramdef << ") const) {");
	LOG("\treturn " << name_cl <<
		"(new " << name << "MethodAction<const OBJECT, " << rettype << " (METHOD::*)("
		<< paramdef << ") const" << If(classlist, ", " + classlist) << ">(object, method));");
	LOG("}");
	LOG("");

	if(!IsNull(classdef))
		LOG("template <" << classdef << ">");
	LOG("inline " << name_cl << " callback(" << rettype << " (*fn)(" << paramdef << ")) {");
	LOG("\treturn " << name_cl << "(new " << name << "FnAction " << cl_list << "(fn));");
	LOG("}");
	LOG("");


	if(!IsNull(classdef))
		LOG("template <" << classdef << ">");
	LOG("struct " << name << "ForkAction : public " << name << "Action" << cl_list << " {");
	LOGBEGIN();
		LOG(name_cl << " cb1, cb2;");
		LOG("");
		LOG(rettype << " Execute(" << paramdef << ") { cb1(" << paramlist <<
		    "); " << return_ << "cb2(" << paramlist << "); }");
//		LOG("bool IsEqual(const " << name << "Action " << cl_list << " *other) const {");
//		LOG("\tconst " << name << "ForkAction *q = dynamic_cast<const " << name
//		    << "ForkAction *>(other);");
//		LOG("\treturn q ? q->cb1 == cb1 && q->cb2 == cb2 : false;");
//		LOG("}");
//		LOG("unsigned GetHashValue() const {");
//		LOG("\treturn ::GetHashValue(cb1) ^ ::GetHashValue(cb2);");
//		LOG("}");
		LOG("");
		LOG(name << "ForkAction(" << name_cl << " cb1, " << name_cl << " cb2)"
		    "\n\t : cb1(cb1), cb2(cb2) {}");
	LOGEND();
	LOG("};");
	LOG("");

	if(!IsNull(classdef))
		LOG("template <" << classdef << ">");
	LOG("inline " << name_cl << " Proxy(" << name_cl << "& cb)");
	LOG("{");
	LOG("\treturn callback(&cb, &" << name_cl << "::Execute);");
	LOG("}");
	LOG("");

	if(IsNull(classdef)) {
		LOG(name_cl << " callback(" << name_cl << " cb1, " << name_cl << " cb2);");
		LOG(name_cl << "& operator<<(" << name_cl << "& a, " << name_cl << " b);");
		LOG("");
		LOG("#endif");
		LOG("#ifdef CPP_PART__");
		LOG("");
	}

	if(!IsNull(classdef))
		LOG("template <" << classdef << ">");
	LOG(name_cl << " callback(" << name_cl << " cb1, " << name_cl << " cb2)");
	LOG("{");
	LOG("\treturn " << name_cl << "(new " << name << "ForkAction " << cl_list << "(cb1, cb2));");
	LOG("}");
	LOG("");

	if(!IsNull(classdef))
		LOG("template <" << classdef << ">");
	LOG(name_cl << "& operator<<(" << name_cl << "& a, " << name_cl << " b)");
	LOG("{");
	LOG("\tif(a)");
	LOG("\t\ta = callback(a, b);");
	LOG("\telse");
	LOG("\t\ta = b;");
	LOG("\treturn a;");
	LOG("}");
	LOG("");

	if(!IsNull(classdef))
		LOG("template <" << classdef << ">");
	LOG(name_cl << "& " << name_cl << "::operator=(const " << name << "& c)");
	LOG("{");
	LOG("\tc.Retain();");
	LOG("\tRelease();");
	LOG("\taction = c.action;");
	LOG("\treturn *this;");
	LOG("}");
	LOG("");
	if(!IsNull(classdef))
		LOG("template <" << classdef << ">");
	LOG(name_cl << "::" << name << "(const " << name << "& c)");
	LOG("{");
	LOG("\taction = c.action;");
	LOG("\tRetain();");
	LOG("}");
	LOG("");
	if(!IsNull(classdef))
		LOG("template <" << classdef << ">");
	LOG(name_cl << "::~" << name << "()");
	LOG("{");
	LOG("\tRelease();");
	LOG("}");
	LOG("");
	if(IsNull(classdef)) {
		LOG("#endif");
		LOG("#ifndef CPP_PART__");
		LOG("");
	}
#endif
};

CONSOLE_APP_MAIN
{
	LOG("#ifndef CPP_PART__");
	LOG("");
	
	CallbackGen("Callback", "void", 0,
		"operator bool() const   { return action && action->IsValid(); }\n"
		"void Execute() const;\n"
	    "void operator()() const { Execute(); }"
	);
	CallbackGen("Callback1", "void", 1,
		"operator bool() const          { return action && action->IsValid(); }\n"
		"void Execute(P1 p1) const      { if(action) action->Execute(p1); }\n"
		"void operator()(P1 p1) const   { Execute(p1); }"
	);
	CallbackGen("Callback2", "void", 2,
		"operator bool() const                { return action && action->IsValid(); }\n"
		"void Execute(P1 p1, P2 p2) const     { if(action) action->Execute(p1, p2); }\n"
		"void operator()(P1 p1, P2 p2) const  { Execute(p1, p2); }"
	);
	CallbackGen("Callback3", "void", 3,
		"operator bool() const                       { return action && action->IsValid(); }\n"
		"void Execute(P1 p1, P2 p2, P3 p3) const     { if(action) action->Execute(p1, p2, p3); }\n"
		"void operator()(P1 p1, P2 p2, P3 p3) const  { Execute(p1, p2, p3); }"
	);
	CallbackGen("Callback4", "void", 4,
		"operator bool() const                              { return action && action->IsValid(); }\n"
		"void Execute(P1 p1, P2 p2, P3 p3, P4 p4) const     { if(action) action->Execute(p1, p2, p3, p4); }\n"
		"void operator()(P1 p1, P2 p2, P3 p3, P4 p4) const  { Execute(p1, p2, p3, p4); }"
	);

	CallbackGen("Gate", "bool", 0,
	    "operator bool() const        { return (void *)action != (void *)1 && action && action->IsValid(); }\n"
		"bool Execute() const;\n"
		"bool operator()() const      { return Execute(); }\n"
		"void ClearTrue()             { Clear(); action = (GateAction *)1; }\n"
		"void ClearFalse()            { Clear(); }\n\n"
		"Gate(bool b)                 { action = (GateAction *)(int)b; }",
		"&& (void *)action != (void *)1"
	);
	CallbackGen("Gate1", "bool", 1,
	    "operator bool() const        { return (void *)action != (void *)1 && action && action->IsValid(); }\n"
		"bool Execute(P1 p1) const;\n"
		"bool operator()(P1 p1) const { return Execute(p1); }\n"
		"void ClearTrue()             { Clear(); action = (Gate1Action<P1> *)1; }\n"
		"void ClearFalse()            { Clear(); }\n\n"
		"Gate1(bool b)                { action = (Gate1Action<P1> *)(uintptr_t)b; }",
		"&& (void *)action != (void *)1"
	);
	CallbackGen("Gate2", "bool", 2,
	    "operator bool() const        { return (void *)action != (void *)1 && action && action->IsValid(); }\n"
		"bool Execute(P1 p1, P2 p2) const;\n"
		"bool operator()(P1 p1, P2 p2) const { return Execute(p1, p2); }\n"
		"void ClearTrue()             { Clear(); action = (Gate2Action<P1, P2> *)1; }\n"
		"void ClearFalse()            { Clear(); }\n\n"
		"Gate2(bool b)                { action = (Gate2Action<P1, P2> *)(uintptr_t)b; }",
		"&& (void *)action != (void *)1"
	);
	CallbackGen("Gate3", "bool", 3,
	    "operator bool() const        { return (void *)action != (void *)1 && action && action->IsValid(); }\n"
		"bool Execute(P1 p1, P2 p2, P3 p3) const;\n"
		"bool operator()(P1 p1, P2 p2, P3 p3) const { return Execute(p1, p2, p3); }\n"
		"void ClearTrue()             { Clear(); action = (Gate3Action<P1, P2, P3> *)1; }\n"
		"void ClearFalse()            { Clear(); }\n\n"
		"Gate3(bool b)                { action = (Gate3Action<P1, P2, P3> *)(uintptr_t)b; }",
		"&& (void *)action != (void *)1"
	);
	CallbackGen("Gate4", "bool", 4,
	    "operator bool() const        { return (void *)action != (void *)1 && action && action->IsValid(); }\n"
		"bool Execute(P1 p1, P2 p2, P3 p3, P4 p4) const;\n"
		"bool operator()(P1 p1, P2 p2, P3 p3, P4 p4) const { return Execute(p1, p2, p3, p4); }\n"
		"void ClearTrue()             { Clear(); action = (Gate4Action<P1, P2, P3, P4> *)1; }\n"
		"void ClearFalse()            { Clear(); }\n\n"
		"Gate4(bool b)                { action = (Gate4Action<P1, P2, P3, P4> *)(uintptr_t)b; }",
		"&& (void *)action != (void *)1"
	);
	LOG("");
	LOG("#endif");
}
