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
	export_ValueMap();
}
}

END_UPP_NAMESPACE
