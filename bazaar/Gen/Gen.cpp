#include "Gen.h"

NAMESPACE_UPP

VectorMap<String, GlobalInstancerType>& GetGlobalInstancerMap()
{
	static VectorMap<String, GlobalInstancerType> map;
	return map;
}

END_UPP_NAMESPACE