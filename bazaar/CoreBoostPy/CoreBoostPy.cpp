#include "CoreBoostPy.h"

NAMESPACE_UPP

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
