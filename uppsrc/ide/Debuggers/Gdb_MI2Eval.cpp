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
void Gdb_MI2::AddAttribs(String const &expr, MIValue &valExpr)
{
	if(valExpr.IsTuple())
	{
		for(int i = 0; i < valExpr.GetCount(); i++)
		{
			String nam = valExpr.GetKey(i);
			String nExpr;
			if(!nam.StartsWith("<"))
			{
				if(expr != "")
					nExpr = expr + "." + nam;
				else
					nExpr = nam;
			}
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
		for(int i = 0; i < valExpr.GetCount(); i++)
			AddAttribs(expr, valExpr[i]);
	}
}

bool Gdb_MI2::TypeSimplify(MIValue &val, bool deep)
{
	bool needMore = false;
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
					{
						needMore |= handler(*this, vRoot, deep);
						if(deep)
						{
							if(needMore)
							{
								// we shall remove the temporary value now...
								vRoot.Remove(SIMPLIFY_TEMPVAL);
								return needMore;
							}
						}
						else if(needMore)
							vRoot.FindAdd(SIMPLIFY_TEMPVAL, "<evaluating...>");
					}
					else
						needMore |= TypeSimplify(v, deep);
				}
				else
					needMore |= TypeSimplify(v, deep);
			}
			else
				needMore |= TypeSimplify(v, deep);
		}
	}
	else if(val.IsArray())
	{
		for(int i = 0; i < val.GetCount(); i++)
		{
			// encapsulate each element inside a tuple
			MIValue tuple;
			tuple.Add("<!DUMMY>", val[i]);

			// simplify
			needMore |= TypeSimplify(tuple, deep);
			
			// de-encapsulate the element
			val[i] = tuple[0];
			if(deep && needMore)
				return needMore;
		}
	}
	return needMore;
}

// variable inspection support
// returns a MIValue with inspected data and some info fields added
// and known types simplified and cathegorized
// unknown and simple types are left as they are
// if deep is true, partial evaluation of containers is done
MIValue Gdb_MI2::Evaluate(String expr, bool deep)
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

		// pack tuple names--remove spaces
		parsed.PackNames();

//RLOG(parsed.Dump());
		AddAttribs(expr, parsed);

		// fix arrays
		parsed.FixArrays();

//RLOG(parsed.Dump());		
		// now we go through the type simplifier, which try to give simple representation
		// for known types -- first step, just type simplification and NOT deep evaluation
		bool needMore = TypeSimplify(parsed, false);
		
		// deep container evaluation on request
		if(deep)
			while(needMore)
				needMore = TypeSimplify(parsed, true);
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
static void Collect0(MIValue &val, Index<String> &exprs, Vector<String> &vals, Vector<int> &hints)
{
	if(!val.IsTuple())
		return;

	for(int i = 0; i < val.GetCount(); i++)
	{
		MIValue &v = val[i];

		// variables still to be evaluated...
		if(v.Find(SIMPLIFY_TEMPVAL) >= 0)
		{
			vals.Add(v[SIMPLIFY_TEMPVAL].ToString());
			exprs.Add(v[SIMPLIFY_EXPR].ToString());
			hints << 0;
		}
		else
		{
			MIValue &data = v[SIMPLIFY_VALUE];
			if(data.IsError())
				continue;
			if(data.IsString())
			{
				String d = data.ToString();
				if(!d.StartsWith("<"))
				{
					vals.Add(data.ToString()),
					exprs.Add(v[SIMPLIFY_EXPR].ToString());
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
}

void Gdb_MI2::CollectVariables(MIValue &val, Index<String> &exprs, Vector<String> &vals, Vector<int> &hints)
{
	exprs.Clear();
	vals.Clear();
	hints.Clear();
	
	Collect0(val, exprs, vals, hints);
}

static void CollectShort0(MIValue &val, String &s)
{
	if(val.IsTuple())
	{
		for(int i = 0; i < val.GetCount(); i++)
		{
			MIValue &v = val[i];
			String expr = v[SIMPLIFY_EXPR].ToString();
	
			// variables still to be evaluated...
			if(v.Find(SIMPLIFY_TEMPVAL) >= 0)
				s << expr << "=" << v[SIMPLIFY_TEMPVAL].ToString() << " , ";
			else
			{
				MIValue &data = v[SIMPLIFY_VALUE];
				if(data.IsError())
					return;
				if(data.IsString())
				{
					// skip vtbls... quite useless
					if(expr.StartsWith("_vptr."))
						return;
	
					String d = data.ToString();
					if(!d.StartsWith("<"))
						s << v[SIMPLIFY_EXPR].ToString() << "=" << d << " , ";
				}
				else if(data.IsTuple())
					CollectShort0(data, s);
			}
		}
	}
	else if(val.IsArray())
	{
		for(int i = 0; i < val.GetCount(); i++)
		{
			MIValue &v = val[i];

			if(v.Find(SIMPLIFY_TEMPVAL) >= 0)
				s << v[SIMPLIFY_TEMPVAL].ToString() << " , ";
			else
			{
				MIValue &data = v[SIMPLIFY_VALUE];
				if(data.IsError())
					return;
				if(data.IsString())
				{
					String d = data.ToString();
					if(!d.StartsWith("<"))
						s <<  d << " , ";
				}
				else if(data.IsTuple())
					CollectShort0(data, s);
			}
		}
	}
}

// collect evaluated variables got with Evaluate
// into a vector of single-line string for short display
String Gdb_MI2::CollectVariablesShort(MIValue &val)
{
	String s;

	CollectShort0(val, s);
	if(s == "")
		s = "<can't evaluate>";
	else if(s.EndsWith(" , "))
		s = s.Left(s.GetCount()-3);
	return s;
}
