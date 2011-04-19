#include "CoreBoostPy.h"

NAMESPACE_UPP

void export_Core()
{
ONCELOCK
{
	export_GTypes();
	export_String();
	export_Value();
	export_ValueArray();
}
}

END_UPP_NAMESPACE
