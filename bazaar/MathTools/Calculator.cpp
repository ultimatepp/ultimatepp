#include "MathTools.h"

#include <Functions4U/Functions4U.h>

void TabCalculator::Init() {
	CtrlLayout(*this);	
	HSizePos().VSizePos();
	
	gridFunctions.AddColumn(t_("Name"));
	gridFunctions.WhenLeftDouble = THISBACK(OnFunction);
	
	for (int i = 0; i < eval.GetFunctionsCount(); ++i)
		gridFunctions.Add(eval.GetFunction(i));
	
	gridConstants.AddColumn(t_("Name"));
	gridConstants.AddColumn(t_("Value"));
	gridConstants.AddColumn(t_("Units"));
	gridConstants.WhenLeftDouble = THISBACK(OnConstant);
	
	gridVariables.AddColumn(t_("Name"));
	gridVariables.AddColumn(t_("Value"));
	gridVariables.AddColumn(t_("Units"));
	
	code.SetFont(Courier(14));
	code.DisableBreakpointing();
	code.LineNumbers(false);
	code.Highlight("cpp");	
	code.WhenChange = THISBACK(OnChange);
	
	numDecimals <<= 5;
	numDecimals.WhenAction = THISBACK(OnChange);
	tabChars <<= 25;
	tabChars.WhenAction = THISBACK(OnChange);
	
	eval.SetErrorUndefined(true);
	
	UpdateVars();
}

void TabCalculator::UpdateVars() {
	String name;
	doubleUnit value;
	
	gridConstants.Clear();
	for (int i = 0; i < eval.GetConstantsCount(); ++i) {
		eval.GetConstant(i, name, value);
		gridConstants.Add(name, value.val, value.unit.GetString());
	}
	gridVariables.Clear();
	for (int i = 0; i < eval.GetVariablesCount(); ++i) {
		eval.GetVariable(i, name, value);
		gridVariables.Add(name, value.val, value.unit.GetString());
	}
}

EvalExpr2::EvalExpr2() {
	units.Add("kg", 	Unit(1, 0, 0));
	units.Add("kg2", 	Unit(2, 0, 0));
	units.Add("kgm", 	Unit(1, 1, 0));
	units.Add("m", 		Unit(0, 1, 0));
	units.Add("m2", 	Unit(0, 2, 0));
	units.Add("m3", 	Unit(0, 3, 0));
	units.Add("seg", 	Unit(0, 0, 1));
	units.Add("s", 		Unit(0, 0, 1));
	units.Add("seg2", 	Unit(0, 0, 2));
	units.Add("s2", 	Unit(0, 0, 2));
	units.Add("N", 		Unit(1, 1, -2));
	for (int i = 0; i < units.GetCount(); ++i)
		units.SetKey(i, ToLower(units.GetKey(i)));
}
	
Unit EvalExpr2::TermUnit(CParser& p) {
	if(!p.IsId()) 
		EvalThrowError(p, t_("Unit id not found"));		
		
	String strId = ToLower(p.ReadId());
	int id = units.Find(strId);
	if (id < 0) 
		EvalThrowError(p, Format(t_("Unknown unit '%s'"), strId));	
	return units[id]; 
}

Unit EvalExpr2::PowUnit(CParser& p) {
	Unit x = TermUnit(p);
	for(;;) {
		if(p.Char('^'))
			x.Exp(p.ReadDouble());
		else
			return x;
	}
}

Unit EvalExpr2::MulUnit(CParser& p) {
	Unit x = PowUnit(p);
	for(;;) {
		if(p.Char('*')) {
			Unit multp = MulUnit(p);
			x.Mult(multp);
		} else if(p.Char('/')) {
			Unit multp = MulUnit(p);
			x.Div(multp);
		} else
			return x;
	}
}

Unit EvalExpr2::EvalUnit(CParser& p) {
	CParser::Pos pos = p.GetPos();
	Unit res;
	try {
		res = MulUnit(p);
	} catch(...) {
		p.SetPos(pos);
	}
	return res;
}

String EvalExpr2::Eval2(String line, int numDecimals, int tabChars) {
	line = TrimBoth(line);
	if (line.IsEmpty())
		return Null;
	
	String newstr;
	try {
		p.Set(line);
		String var, cte;
		int expFrom;
		doubleUnit val;
		
		if (p.IsId()) {
			CParser::Pos pos = p.GetPos();
			String sid = p.ReadId();
			if (noCase)
				sid = ToLower(sid);
			
			if(p.Char('=')) {
				expFrom = int(p.GetPtr() - line.Begin());
				val = Exp(p);	
				if (!IsFunction(sid)) {
					if (IsConstant(sid))
						cte = sid;
					else
						var = sid;			
				}
			} else {
				p.SetPos(pos);
				expFrom = 0;
				val = Exp(p);
			}
		} else {
			expFrom = int(p.GetPtr() - line.Begin());
			val = Exp(p);
		}
							
		int expTo = int(p.GetPtr() - line.Begin() - 1);
		String exp = TrimBoth(line.Mid(expFrom, expTo - expFrom + 1));
		
		String strVal = FormatDoubleFix(val.val, numDecimals);
		bool expIsNum = exp == strVal;
			
 		if (!var.IsEmpty()) 
			newstr << var << " = ";
		else if (!cte.IsEmpty())
			newstr << cte << " = ";
			
		if (!expIsNum)
			newstr << exp << " = ";
		newstr << strVal;
			
		String strUnit;
		Unit unit = EvalUnit(p);
		if (!IsNull(unit)) 
			strUnit = unit.GetString();
		
		CParser::Pos pos = p.GetPos();	
		if (p.Char('=')) {
			if (p.IsDouble2()) {
				p.ReadDouble();
				unit = EvalUnit(p);
				if (!IsNull(unit))
					strUnit = unit.GetString();
			} else
				p.SetPos(pos);	
		} else
			p.SetPos(pos);
		
		newstr << " " << strUnit;
		
		if (!var.IsEmpty()) {
			if (IsNull(val.unit))
				val.unit = unit; 
			SetVariable(var, val);
		}
		
		newstr << " ";
		int newstrlen = newstr.GetCount();	
		int len;
		for (len = tabChars; len < newstrlen; len += tabChars)
			;
		for (; len - newstrlen > 0; --len)
			newstr << " ";	
		
		String comment = TrimBoth(line.Mid(int(p.GetPtr() - line.Begin())));
		newstr << TrimBoth(comment);	
	} catch(CParser::Error e) {
		lastError = e;
		newstr << line << " " << Format(t_("Error %s"), GetLastError());
	} catch(Exc e) {
		lastError = e;
		newstr << line << " " << Format(t_("Error %s"), GetLastError());
	}
	return newstr;
}

void TabCalculator::OnChange() {
	if (justEdit)
		return;
	eval.SetCaseSensitivity(caseSensitivity);
	eval.ClearVariables();
	
	int cursorline = code.GetCursorLine();
	int cursorpos = code.GetCursor() - code.GetPos(cursorline, 0);
	
	for (int line = 0; line < code.GetLineCount(); ++line) {
		String str = code.GetUtf8Line(line);
		if (str.IsEmpty())
			continue;

		int pos0 = code.GetPos(line, 0);
		int pos1 = code.GetPos(line+1, 0);
		code.Remove(pos0, pos1 - pos0 - 1);

		int errPos = str.Find(t_("Error"));
		if (errPos >= 0)
			str = TrimRight(str.Left(errPos));
		
		String newstr = eval.Eval2(str, numDecimals, tabChars);
		code.Insert(pos0, newstr);
	}
	code.SetCursor(code.GetPos(cursorline, cursorpos));
	ClearModify();
	
	UpdateVars();
}

void TabCalculator::OnFunction() {
	int left, right;
	code.GetSelection(left, right);
	code.Insert(right, ")");
	code.Insert(left, String(gridFunctions.Get(0)) + "(");
	code.ClearSelection();
	code.SetFocus();
}

void TabCalculator::OnConstant() {
	int left, right;
	code.GetSelection(left, right);
	code.Remove(left, right - left);
	String constant = gridConstants.Get(0);
	code.Insert(left, constant);
	code.SetCursor(left + constant.GetCount());
	code.SetFocus();	
}

Tool *Construct2()
{
	static TabCalculator tab;
	tab.name = t_("Calculator");
	return &tab;
}

INITBLOCK {
	RegisterExample(Construct2, __FILE__);
}