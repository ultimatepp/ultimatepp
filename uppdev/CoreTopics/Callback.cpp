#include "Core.h"

NAMESPACE_UPP

//#BLITZ_APPROVE
#define CPP_PART__
#include "Cbgen.h"

void Callback::Execute() const
{
	if(action) action->Execute();
}

bool Gate::Execute() const
{
	return (void *)action == (void *)1 ? true : action ? action->Execute() : false;
}

END_UPP_NAMESPACE
