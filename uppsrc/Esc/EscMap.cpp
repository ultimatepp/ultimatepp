#include "Esc.h"

namespace Upp {

#define LTIMING(x) // RTIMING(x)

VectorMap<EscValue, EscValue>& EscValue::CloneMap()
{
	LTIMING("CloneMap");
	ASSERT(IsMap());
	if(map->refcount != 1) {
		EscMap *c = new EscMap;
		c->map <<= map->map;
		map->Release();
		map = c;
	}
	map->cached_hash = 0;
	return map->map;
}

void  EscValue::SetEmptyMap()
{
	Free();
	type = ESC_MAP;
	map = new EscMap;
}

void EscValue::MapSet(EscValue key, EscValue value)
{
	LTIMING("MapSet");
	if(IsVoid())
		SetEmptyMap();
	VectorMap<EscValue, EscValue>& m = CloneMap();
	int q = m.Find(key);
	if(q >= 0) {
		if(value.IsVoid()) {
			m.Unlink(q);
			map->count--;
		}
		else
			m[q] = value;
	}
	else
		if(!value.IsVoid()) {
			map->count++;
			m.Add(key, value);
		}
}

bool EscValue::HasNumberField(const char *id) const
{
	return IsMap() && GetMap().Find(id) >= 0;
}

int  EscValue::GetFieldInt(const char *id) const
{
	return HasNumberField(id) ? MapGet(id).GetInt() : 0;
}

}
