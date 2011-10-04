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
	boost::python::object globals;
	mutable boost::python::object locals; //added to by Format
	String expr;

	virtual Value Format(const Value& q) const;
};

void export_Core();

END_UPP_NAMESPACE

#endif
