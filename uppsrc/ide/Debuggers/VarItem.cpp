#include "Debuggers.h"
#include <ide/ide.h>

// check if value contains an error
bool Gdb_MI2::VarItem::IsError(void) const
{
	return error;
}

// constructor
Gdb_MI2::VarItem::VarItem()
{
	error = true;
	dynamic = false;
	numChildren = 0;
	kind = SIMPLE;
}

// evaluate an expression usign gdb variables
Gdb_MI2::VarItem Gdb_MI2::EvalGdb(String const &expr)
{
	VarItem v;
	
	MIValue val = MICmd("var-create - @ " + expr);
	if(val.IsError())
		return v;
	v.error = false;
	v.varName = val["name"];
	v.evaluableExpression = expr;
	v.shortExpression = expr;
	v.value = val["value"];
	v.numChildren = atoi(val.Get("numchild", "0"));
	v.type = val["type"];
	String dispHint = val.Get("displayhint", "");
	if(ToUpper(dispHint) == "ARRAY")
		v.kind = VarItem::ARRAY;
	else if(ToUpper(dispHint) == "MAP")
		v.kind = VarItem::MAP;
	else
		v.kind = VarItem::SIMPLE;
	v.dynamic = (val.Get("dynamic", "") != "");
	return v;
}

// remove GDB variable for item
void Gdb_MI2::KillVariable(Gdb_MI2::VarItem &v)
{
	MICmd("var-delete " + v.varName);
}

// fetch variable children
Vector<Gdb_MI2::VarItem> Gdb_MI2::GetChildren(MIValue const &val, String const &prePath)
{
	Vector<VarItem> res;
	
	if(val.IsError() || !val.IsTuple())
		return res;
	MIValue const &children = val["children"];
	if(!children.IsArray())
		return res;

	for(int i = 0; i < children.GetCount(); i++)
	{
		MIValue const &child = children[i];

		// for private, protected, public and inherited fake childs, just go deeper
		String exp = child["exp"];
		String typ = child.Get("type", "");
		String nam = child.Get("name");
		if(exp == "private" || exp == "protected" || exp == "public" || exp == typ)
		{
			MIValue val2 = MICmd("var-list-children 1 " + nam);
			res.Append(GetChildren(val2, prePath));
		}
		else
		{
			VarItem &v = res.Add();
			v.error = false;
			v.varName = nam;
			v.shortExpression = prePath + "." + exp;
			v.type = typ;
			v.value = child["value"];
			v.dynamic = (child.Get("dynamic", "") != "");
			v.numChildren = atoi(child.Get("numchild", "0"));

			String dispHint = child.Get("displayhint", "");
			if(ToUpper(dispHint) == "ARRAY")
				v.kind = VarItem::ARRAY;
			else if(ToUpper(dispHint) == "MAP")
				v.kind = VarItem::MAP;
			else
				v.kind = VarItem::SIMPLE;
/*
			MIValue vExp = MICmd("var-info-path-expression " + nam);
			v.evaluableExpression = vExp.Get("path_expr", "");
*/
		}
	}
	return res;		
}

// fetch variable children
Vector<Gdb_MI2::VarItem> Gdb_MI2::GetChildren(Gdb_MI2::VarItem &v, int minChild, int maxChild)
{
	// for 'simple' (i.e. not maps or arrays..) get all children
	// otherwise get the given range
	int first, last;
	if(v.kind == VarItem::SIMPLE)
	{
		first = 0;
		last = INT_MAX;
	}
	else
	{
		if(minChild < 0)
			first = 0;
		else
			first = minChild;
		if(maxChild < 0)
			last = v.numChildren;
		else
			last = maxChild;
	}
	MIValue val = MICmd(Format("var-list-children 1 %s %d %d", v.varName, first, last));
	return GetChildren(val, v.shortExpression);
}


