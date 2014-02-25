#include "VarItem.h"
#include "TypeSimplify.h"

// constructor
VarItem::VarItem(Gdb_MI2 *deb)
{
	debugger = deb;
	Clear();
}

VarItem::VarItem(Gdb_MI2 *deb, String const &expr)
{
	debugger = deb;
	Evaluate(expr);
}

// copy
VarItem::VarItem(pick_ VarItem &v)
{
	debugger = v.debugger;
	empty = v.empty;
	simplifyStep = v.simplifyStep;
	
	varName = v.varName;
	((VarItem &)v).varName.Clear();
	
	shortExpression = v.shortExpression;
	evaluableExpression = v.evaluableExpression;
	type = v.type;
	kind = v.kind;
	value = v.value;
	numChildren = v.numChildren;
	items = v.items;
}

// destructor
VarItem::~VarItem()
{
	Clear();
}

VarItem& VarItem::operator=(pick_ VarItem &v)
{
	if(!varName.IsEmpty() && varName.Find('.') < 0)
		debugger->MICmd("var-delete " + varName);

	debugger = v.debugger;
	empty = v.empty;
	simplifyStep = v.simplifyStep;

	varName = v.varName;
	((VarItem &)v).varName.Clear();

	shortExpression = v.shortExpression;
	evaluableExpression = v.evaluableExpression;
	type = v.type;
	kind = v.kind;
	value = v.value;
	numChildren = v.numChildren;
	items = v.items;
	return *this;
}

// clears contents
void VarItem::Clear(void)
{
	// store variable to be deleted later
	if(!varName.IsEmpty() && varName.Find('.') < 0)
		PutDeleted(varName);

	empty = true;
	simplifyStep = -1;
	varName.Clear();
	shortExpression.Clear();
	evaluableExpression.Clear();
	type.Clear();
	value.Clear();
	numChildren = 0;
	items = 0;
	kind = SIMPLE;
}

bool VarItem::Simplify(void)
{
	// if already simplified, return false
	if(!simplifyStep)
		return false;
	
	// lookup for simplifier
	TYPE_SIMPLIFIER_HANDLER simplifier = GetSimplifier(type);
	if(!simplifier)
	{
		// none found, mark as already simplifie and leave
		simplifyStep = 0;
		return false;
	}
	
	// simplifier found
	if(simplifyStep == -1)
	{
		// fast, non-deep simplification
		// set simplified to false if need deep one
		simplifyStep = simplifier(*this, 0);
	}
	else
	{
		// slow, deep simplification
		simplifyStep = simplifier(*this, simplifyStep);
	}
	return (simplifyStep != 0);
}

// evaluate an expression usign gdb variables
bool VarItem::Evaluate(String const &expr)
{
	Clear();

	// create the variable
	MIValue var = debugger->MICmd("var-create - * " + expr);
	if(var.IsError())
		return false;
	empty = false;
	
	// store its name
	varName = var["name"];

	// store its value
	value = var["value"];
	
	// store type
	type = var["type"];

	// store number of children (temporary number...)
	// and set temporary object kind
	numChildren = atoi(var.Get("numchild", "0"));
	kind = numChildren ? COMPLEX : SIMPLE;
	
	// get and store expression
	evaluableExpression = expr;
	shortExpression = expr;

	// fast simplify known types
	Simplify();
	
	return true;
}

// fetch variable children
Vector<VarItem>  VarItem::GetChildren0(MIValue const &val, String const &prePath)
{
	Vector<VarItem> res;
	
	MIValue const &children = val["children"];
	if(!children.IsArray())
		return res;
	
	for(int iChild = 0; iChild < children.GetCount(); iChild++)
	{
		MIValue const &child = children[iChild];
	
		// for private, protected, public and inherited fake childs, just go deeper
		String exp = child["exp"];
		String typ = child.Get("type", "");
		String nam = child.Get("name");
		if(exp == "private" || exp == "protected" || exp == "public" || exp == typ)
		{
			MIValue val2 = debugger->MICmd("var-list-children 1 " + nam);
			if(!val2.IsTuple())
				continue;
			res.AppendPick(GetChildren0(val2, prePath));
		}
		else
		{
			VarItem &v = res.AddPick(VarItem(debugger));
			v.empty = false;
			v.varName = nam;
			v.shortExpression = prePath + "." + exp;
			v.type = typ;

			v.value = child["value"];
			v.numChildren = atoi(child.Get("numchild", "0"));
			v.kind = v.numChildren ? COMPLEX : SIMPLE;

			MIValue vExp = debugger->MICmd("var-info-path-expression " + nam);
			v.evaluableExpression = vExp.Get("path_expr", "");
			
			// fast simplify known types
			Simplify();
		}
	}
	
	return res;
}

// fetch variable children
Vector<VarItem> VarItem::GetChildren(void)
{
	Vector<VarItem> res;
	
	// do not enumerate children for non-complex types
	// (for arrays and maps just use GetArray and GetMap functions)
	if(kind != COMPLEX)
		return res;
	
	// if no variable name, just return empty array
	if(varName.IsEmpty())
		return res;
	
	// get children of current variable
	MIValue val = debugger->MICmd("var-list-children 1 " + varName);
	if(!val.IsTuple())
		return res;

	res = GetChildren0(val, evaluableExpression);
	return res;
}

// fetch array elements
Vector<VarItem> VarItem::GetArray(int start, int count)
{
	Vector<VarItem> res;
	
	return res;
}

// fetch map elements
VectorMap<VarItem, VarItem> VarItem::GetMap(int start, int count)
{
	VectorMap<VarItem, VarItem> res;
	
	return res;
}

// helpers for simplifiers
		// helper for simplifiers
/*
void VarItem::ListChildren0(String const &varName, MIValue &res) const
{
	MIValue val = debugger->MICmd("var-list-children " + varName);
	if(!val.IsTuple())
		return;
	MIValue const &children = val["children"];
	if(!children.IsArray())
		return;

	for(int iChild = 0; iChild < children.GetCount(); iChild++)
	{
		MIValue const &child = children[iChild];
	
		String exp = child["exp"];
		String typ = child.Get("type", "");
		String nam = child.Get("name");
		if(exp == "private" || exp == "protected" || exp == "public" || exp == typ)
		{
			MIValue val2 = debugger->MICmd("var-list-children 1 " + nam);
			if(!val2.IsTuple())
				continue;
			ListChildren0(val2, prePath));
		}
		else
		{
	
	
}

MIValue VarItem::ListChildren(void) const
{
}
*/

MIValue VarItem::EvaluateExpression(String const &exp) const
{
	MIValue val = debugger->MICmd("data-evaluate-expression " + exp);
	if(!val.IsTuple())
		return MIValue();
	MIValue v = val["value"];
	if(v.IsError() || !v.IsString())
		return MIValue();
	String s = v.ToString();
	return MIValue(s);
}


// cleanup support -- variables can't be deleted in destructor
// as usually destructor is called when thread is hard-stopped
// by main thread with an exception.
// so we store all var names in a static index and delete them
// when back to main thread
StaticMutex VarItem::varMutex;
Vector<String> VarItem::deletedVars;

void VarItem::PutDeleted(String const &name)
{
	INTERLOCKED_(varMutex) {
		deletedVars.Add(name);
	}
}

void VarItem::CleanVariables(Gdb_MI2 *deb)
{
	ASSERT(IsMainThread());
	INTERLOCKED_(varMutex) {
		for(int i = 0; i < deletedVars.GetCount(); i++)
			deb->MICmd("var-delete " + deletedVars[i]);
	}
}

