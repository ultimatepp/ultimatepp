#include "Gen.h"

NAMESPACE_UPP

GlobalInstancerMapType& GetGlobalInstancerMap()
{
	static GlobalInstancerMapType _;
	return _;
}

END_UPP_NAMESPACE