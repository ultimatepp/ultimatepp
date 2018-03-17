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
	gridConstants.WhenLeftDouble = THISBACK(OnConstant);
	
	gridVariables.AddColumn(t_("Name"));
	gridVariables.AddColumn(t_("Value"));
	
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
	double value;
	
	gridConstants.Clear();
	for (int i = 0; i < eval.GetConstantsCount(); ++i) {
		eval.GetConstant(i, name, value);
		gridConstants.Add(name, value);
	}
	gridVariables.Clear();
	for (int i = 0; i < eval.GetVariablesCount(); ++i) {
		eval.GetVariable(i, name, value);
		gridVariables.Add(name, value);
	}
}

/*double EvalExpr2::TermUnit(CParser& p) {
	if(p.IsId()) {
		//Estan almacenadas laslas unidades
		String strId = p.ReadId();
		String strsearch;
		if (noCase)
			strsearch = ToUpper(strId);
		else
			strsearch = strId;
		double ret = constants.Get(strsearch, Null);
		if (IsNull(ret)) {
			ret = variables.Get(strsearch, Null);
			if (IsNull(ret)) {
				if (errorIfUndefined)
					EvalThrowError(p, Format(t_("Unknown var '%s'"), strId));	
				ret = variables.GetAdd(strsearch, 0);
			}
		}
		return ret;
	}  
	EvalThrowError(p, t_("Unit id not found"));		
}

double EvalExpr2::PowUnit(CParser& p) {
	Unit x = TermUnit(p);
	for(;;) {
		if(p.Char('^'))
			x = x.pow(p.ReadDouble());
		else
			return x;
	}
}

Unit EvalExpr2::MulUnit(CParser& p) {
	Unit x = Pow(p);
	for(;;) {
		if(p.Char('*'))
			x = x * MulUnit(p);
		else if(p.Char('/')) {
			Unit y = PowUnit(p);
			x = x / y;
		} else
			return x;
	}
}

Unit EvalExpr2::EvalUnit(CParser& p) {
	return MulUnit(p);
}*/

String EvalExpr2::Eval2(String line, int numDecimals, int tabChars) {
	line = TrimBoth(line);
	if (line.IsEmpty())
		return Null;
	
	String newstr;
	try {
		p.Set(line);
		String var, cte;
		int expFrom;
		double val;
		
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
		
		String strVal = FormatDoubleFix(val, numDecimals);
		bool expIsNum = exp == strVal;
			
 		if (!var.IsEmpty()) {
			newstr << var << " = ";
			variables.GetAdd(var) = val;
		} else if (!cte.IsEmpty())
			newstr << cte << " = ";
			
		if (!expIsNum)
			newstr << exp << " = ";
		newstr << strVal;
			
		newstr << " ";
		int newstrlen = newstr.GetCount();	
		int len;
		for (len = tabChars; len < newstrlen; len += tabChars)
			;
		for (; len - newstrlen > 0; --len)
			newstr << " ";	
		
		CParser::Pos pos = p.GetPos();	
		if (p.Char('=')) {
			if (p.IsDouble2())
				p.ReadDouble();
			else
				p.SetPos(pos);	
		} else
			p.SetPos(pos);

		String comment = TrimBoth(line.Mid(int(p.GetPtr() - line.Begin())));
		newstr << TrimBoth(comment);	
	}
	catch(CParser::Error e) {
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
		code.Remove(pos0, str.GetCount());

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