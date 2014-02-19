#include "TypeSimplify.h"

static VectorMap<String, TYPE_SIMPLIFIER_HANDLER> &GetSimplifierMap(void)
{
	static VectorMap<String, TYPE_SIMPLIFIER_HANDLER> map;
	return map;
}

void RegisterSimplifier(const char *pattern, TYPE_SIMPLIFIER_HANDLER handler)
{
	GetSimplifierMap().Add(pattern, handler);
}

TYPE_SIMPLIFIER_HANDLER GetSimplifier(String const &pattern)
{
	VectorMap<String, TYPE_SIMPLIFIER_HANDLER> &map = GetSimplifierMap();
	for(int i = 0; i < map.GetCount(); i++)
	{
		if(pattern.StartsWith(map.GetKey(i)))
			return map[i];
	}
	return NULL;
}

// next index when stepping through containers values
int SIMPLIFIER_NEXT_INDEX(MIValue &val, int total)
{
	int step;
	
	int idx = val.Find(SIMPLIFY_STEP);
	if(idx < 0)
	{
		step = 0;
		val.Add(SIMPLIFY_STEP, "0");
	}
	else
	{
		step = atoi(val[SIMPLIFY_STEP].ToString()) + 1;
		if(step >= total)
			val.Remove(SIMPLIFY_STEP);
		else
			val[SIMPLIFY_STEP].Set(FormatInt(step));
	}
	return step;
}
