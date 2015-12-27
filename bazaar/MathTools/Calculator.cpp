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
	
	code.SetFont(Courier(12));
	code.DisableBreakpointing();
	code.LineNumbers(false);
	code.Highlight("cpp");	
	code.WhenEnter = THISBACK(OnEnter);
	
	resultRight = false;
	numDecimals = 5;
	
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

void TabCalculator::OnEnter() {
	if (justEdit)
		return;
		
	int line = code.GetCursorLine();
	int pos = code.GetPos(line, code.GetLineLength(line));
	String str = code.GetUtf8Line(line);
	double ret = eval.Eval(str);
	String strRet;
	if (resultRight)
	 	strRet = " = ";
	else
		strRet = "\n";
	strRet += FormatDoubleFix(ret, numDecimals);
	code.Insert(pos, strRet);
	code.SetCursor(pos + strRet.GetCount());
	
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
	return &tab;
}

INITBLOCK {
	RegisterExample(t_("Calculator"), Construct2, __FILE__);
}