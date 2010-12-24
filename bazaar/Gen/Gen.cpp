#include "Gen.h"

NAMESPACE_UPP

VectorMap<Value, GlobalInstancerType>& GetGlobalInstancerMap()
{
	static VectorMap<Value, GlobalInstancerType> map;
	return map;
}

END_UPP_NAMESPACE