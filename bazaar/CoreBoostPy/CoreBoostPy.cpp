#include "CoreBoostPy.h"
using namespace boost::python;

NAMESPACE_UPP

Value PyEvalConvert::Format(const Value& q) const
{
	Value v;
	try
	{
		object arg(q);
		locals["arg"] = arg;
		object o = eval(expr.Begin(), locals, globals);
		v = extract<Value>(o);
	}
	catch(boost::python::error_already_set const &)
	{
		// Parse and output the exception
		std::string perror_str = parse_py_exception();
		String es = perror_str;
		v = ErrorValue(es);
	}
	return v;
}

void export_Core()
{
ONCELOCK
{
	export_Callback();
	export_Gate();

	export_GTypes();
	export_String();
	export_Value();
	export_ValueArray();
	export_ValueMap();

	export_Log();
}
}

END_UPP_NAMESPACE
