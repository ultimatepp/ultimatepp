#include <Sic/Sic.h>

SVal SIC_DefFn(Sic& sic, Vector<Sic::SRVal>& arg)
{
	if(!arg[0]->IsString())
		sic.ThrowError("name of function must be a string in call to 'def_fn'");
	sic.global.GetAdd(arg[0]->GetString()) = arg[1];
	return Null;
}

SVal SIC_count(Sic& sic, Vector<Sic::SRVal>& arg)
{
	if(arg[0]->IsMap())
		return arg[0]->GetMap().GetCount();
	if(arg[0]->IsString())
		return arg[0]->GetString().GetLength();
	if(arg[0]->IsVoid())
		return 0;
	return 1;
}

SVal SIC_is_string(Sic& sic, Vector<Sic::SRVal>& arg)
{
	return int(arg[0]->IsString());
}

SVal SIC_is_number(Sic& sic, Vector<Sic::SRVal>& arg)
{
	return int(arg[0]->IsNumber());
}

SVal SIC_get_item(Sic& sic, Vector<Sic::SRVal>& arg)
{
	if(!arg[1]->IsNumber())
		sic.ThrowError("second argument of 'get_item' must be a number");
	int q = arg[1]->GetNumber();
	if(arg[0]->IsMap()) {
		if(q < 0 || q >= arg[0]->GetMap().GetCount())
			sic.ThrowError("out of range in call to 'get_item'");
		return arg[0]->GetMap()[q];
	}
	if(arg[0]->IsString()) {
		String s = arg[0]->GetString();
		if(q < 0 || q >= s.GetLength())
			sic.ThrowError("out of range in call to 'get_item'");
		return s[q];
	}
	sic.ThrowError("argument of 'get_item' must be either a string or container");
	return Null;
}

SVal SIC_get_key(Sic& sic, Vector<Sic::SRVal>& arg)
{
	if(!arg[1]->IsNumber())
		sic.ThrowError("second argument of 'get_key' must be a number");
	int q = arg[1]->GetNumber();
	if(arg[0]->IsMap()) {
		if(q < 0 || q >= arg[0]->GetMap().GetCount())
			sic.ThrowError("out of range in call to 'get_item'");
		return arg[0]->GetMap().GetKey(q);
	}
	return q;
}

void StdLib(ArrayMap<String, SVal>& global)
{
	Escape(global, "is_number(x)", SIC_is_number);
	Escape(global, "is_string(x)", SIC_is_string);

	Escape(global, "def_fn(x, y)", SIC_DefFn);
	Escape(global, "count(x)", SIC_count);
	Escape(global, "get_item(x, i)", SIC_get_item);
	Escape(global, "get_key(x, i)", SIC_get_key);
}
