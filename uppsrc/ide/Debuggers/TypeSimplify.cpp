#include "Debuggers.h"
#include <ide/ide.h>

typedef void (*TYPE_SIMPLIFIER_HANDLER)(MIValue &val);

static void UppStringSimplify(MIValue &val)
{
	// strings are "easy", in debug mode 's' and 'len' members are provided
	// so we shall just look at them in tuple
	try
	{
		MIValue &v = val[0][0][0];
		if(!v.IsTuple())
			return;
		if(v.Find("s") < 0)
			return;
		String s = v["s"];
		
		// strip address part
		int pos = 0;
		while(s[pos] && s[pos] != '"')
			pos++;
		if(!s[pos])
			return;
		val.Set(s.Mid(pos)); 
	}
	catch(...)
	{
		return;
	}
}

struct TYPE_SIMPLIFIER
{
	const char *type;
	TYPE_SIMPLIFIER_HANDLER handler;

};

TYPE_SIMPLIFIER TYPE_SIMPLIFIERS[] =
{
	{ "<Upp::Moveable<Upp::String,Upp::AString<Upp::String0>>>", UppStringSimplify }		
};

#define SIMPLIFIERS_COUNT ((int)(sizeof(TYPE_SIMPLIFIERS) / sizeof(TYPE_SIMPLIFIER)))

void Gdb_MI2::TypeSimplify(MIValue &val)
{
	static VectorMap<String, TYPE_SIMPLIFIER_HANDLER> handlers;
	if(!handlers.GetCount())
		for(int i = 0; i < SIMPLIFIERS_COUNT; i++)
			handlers.Add(TYPE_SIMPLIFIERS[i].type, TYPE_SIMPLIFIERS[i].handler);

	if(val.IsTuple())
	{
		for(int i = 0; i < val.GetCount(); i++)
		{
			MIValue &v = val[i];
			if(v.IsTuple() && v.GetCount())
			{
				int idx = handlers.Find(v.GetKey(0));
				if(idx >= 0)
					handlers[idx](v);
				else
					TypeSimplify(v);
			}
			else
				TypeSimplify(v);
		}
	}
}