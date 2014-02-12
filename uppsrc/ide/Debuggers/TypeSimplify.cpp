#include "Debuggers.h"
#include <ide/ide.h>

typedef bool (*TYPE_SIMPLIFIER_HANDLER)(MIValue &val);

static bool UppStringSimplify(MIValue &val)
{
	// strings are "easy", in debug mode 's' and 'len' members are provided
	// so we shall just look at them in tuple
	// see Upp::String code for how it works....
	try
	{
		MIValue &v = val[0][0][0];
		MIValue &unn = v[1];
		if(!v.IsTuple() || !unn.IsTuple())
			return false;
		
		if(unn.Find("chr") < 0)
			return false;
		String chr = unn["chr"];
		bool isSmall = (chr[14] == 0);
		String s;
		if(isSmall)
			s = chr;
		else
		{
			if(unn.Find("ptr") < 0)
				return false;
			s = unn["ptr"];
			
			// strip address...
			int i = s.Find('"');
			if(i >= 0)
			{
				s = s.Mid(i+1);
				s = s.Left(s.GetCount()-1);
			}
		}
		val.Set(s); 
		return true;
	}
	catch(...)
	{
		return false;
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