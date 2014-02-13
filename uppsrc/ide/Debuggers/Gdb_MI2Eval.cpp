#include "Debuggers.h"
#include <ide/ide.h>

#include "TypeSimplify.h"

// now we scan the result and add some info
// so, for example, if we find  tuple like this one:
//    data = simplevalue
// this will be modified as
//    data = { value = simplevalue, expr = evaluable_expression }
// and for a complex value
//    data = { some=complex, not_simple=val }
// woll be modified as
//    data = { <!value> = { some=complex, not_simple=val }, <!expr> = evaluable_expression }
// More attributes will be added by type simplifier phase
static void AddAttribs(String const &expr, MIValue &valExpr)
{
	if(valExpr.IsTuple())
	{
		for(int i = 0; i < valExpr.GetCount(); i++)
		{
			String nam = valExpr.GetKey(i);
			String nExpr;
			if(!nam.StartsWith("<"))
				nExpr = expr + "." + nam;
			else
				nExpr = expr;
			MIValue v = valExpr[i];
			valExpr[i].Clear();
			valExpr[i].Add(SIMPLIFY_VALUE, v);
			valExpr[i].Add(SIMPLIFY_EXPR, nExpr);
			AddAttribs(nExpr, valExpr[i][SIMPLIFY_VALUE]);
		}
	}
	else if(valExpr.IsArray())
	{
	}
}

void Gdb_MI2::TypeSimplify(MIValue &val)
{
	if(val.IsTuple())
	{
		for(int i = 0; i < val.GetCount(); i++)
		{
			// root of variable -- contains VALUE, EXPRESSION and will add HINTS
			MIValue &vRoot = val[i];
			
			// value part of variable
			MIValue &v = vRoot[SIMPLIFY_VALUE];
			
			if(v.IsTuple() && v.GetCount())
			{
				String key = v.GetKey(0);
				if(key.StartsWith("<!"))
					continue;
				if(key.StartsWith("<"))
				{
					TYPE_SIMPLIFIER_HANDLER handler = GetSimplifier(v.GetKey(0));
					if(handler)
						handler(*this, vRoot);
					else
						TypeSimplify(v);
				}
				else
					TypeSimplify(v);
			}
			else
				TypeSimplify(v);
		}
	}
}

// variable inspection support
// returns a MIValue with inspected data and some info fields added
// and known types simplified and cathegorized
// unknown and simple types are left as they are
MIValue Gdb_MI2::Evaluate(String expr)
{
	// add parhentesis around expression... gdb is dumb
	expr = "(" + expr + ")";
	
	// ask gdb to evaluate expression
	// and gather result in a tuple
	MIValue valExpr = MICmd("data-evaluate-expression " + expr);
	
	// return empty value on error
	if(!valExpr.IsTuple())
		return MIValue();

	if(valExpr.Find("value") >= 0)
	{
		// weird but the value is quoted, so must be parsed again...
		MIValue const &tup = valExpr.Get("value");
		if(!tup.IsString())
			return MIValue();
		String s = tup.ToString();
		MIValue parsed(s);

		// enclose it in a dummy container
		MIValue enclosed;
		enclosed.Add("<DUMMY>", parsed);

		// remove spaces from names
		enclosed.PackNames();
		
		// Add attributes to expression data
		AddAttribs(expr, enclosed);
		
		// now we go through the type simplifier, which try to give simple representation
		// for known types
		TypeSimplify(enclosed);

//RLOG(enclosed.Dump());

		// now we can de-enclose it...
		parsed = enclosed["<DUMMY>"][SIMPLIFY_VALUE];
		
//RLOG(parsed.Dump());

		// and finally return the cleaned evaluated data
		return parsed;
	}

	else if(valExpr.Find("variables") >= 0)
	{
RLOG("ARRAY SIDE UNHANDLED...."); 
		return MIValue();
	}
	else
	{
RLOG("WEIRD STUFF HERE....");
		return MIValue();
	}
}

// collects evaluated variables got with Evaluate
// hints are used to choose the visualizer when deep-inspecting members
// 0 for simple values, 1 for arrays, 2 for map
static void Collect0(MIValue &val, Vector<String> &exprs, Vector<String> &vals, Vector<int> &hints)
{
	if(!val.IsTuple())
		return;

	for(int i = 0; i < val.GetCount(); i++)
	{
		MIValue &v = val[i];
		MIValue &data = v[SIMPLIFY_VALUE];
		if(data.IsError())
			continue;
		if(data.IsString())
		{
			String d = data.ToString();
			if(!d.StartsWith("<"))
			{
				vals.Add(data.ToString()),
				exprs.Add(v[SIMPLIFY_EXPR]);
				String hint = v.Get(SIMPLIFY_HINT, SIMPLIFY_SIMPLE);
				if(hint == SIMPLIFY_SIMPLE)
					hints << 0;
				else if(hint == SIMPLIFY_ARRAY)
					hints << 1;
				else if(hint == SIMPLIFY_MAP)
					hints << 2;
				else
					hints << 0;
			}
		}
		else if(data.IsTuple())
			Collect0(data, exprs, vals, hints);
	}
}

void Gdb_MI2::CollectVariables(MIValue &val, Vector<String> &exprs, Vector<String> &vals, Vector<int> &hints)
{
	exprs.Clear();
	vals.Clear();
	hints.Clear();
	
	Collect0(val, exprs, vals, hints);
}

