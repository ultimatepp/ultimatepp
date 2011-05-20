#ifndef _CoreBoostPy_CoreBoostPy_h_
#define _CoreBoostPy_CoreBoostPy_h_

#include "Misc.h"
#include "GTypes.h"
#include "String.h"
#include "Value.h"
#include "Callback.h"

NAMESPACE_UPP

class PyEvalConvert : public Convert
{
public:
	object globals;
	mutable object locals; //added to by Format
	String expr;

	virtual Value  Format(const Value& q) const
	{
		object arg(q);
		locals["arg"] = arg;
		object o = eval(expr.Begin(), locals, globals);
		Value v = extract<Value>(o);
		return v;
	}
};

void export_Core();

END_UPP_NAMESPACE

#endif
