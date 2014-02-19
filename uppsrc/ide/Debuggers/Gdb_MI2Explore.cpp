#include "Debuggers.h"
#include <ide/ide.h>

void Gdb_MI2::doExplore(String const &expr, bool appendHistory)
{
	// set the expression inside expression editor
	explorerExprEdit = expr;

	// update the history : trim it from past current position
	// and append it at end
	if(appendHistory)
	{
		if(explorerHistoryPos >= 0)
		{
			explorerHistoryPos++;
			explorerHistoryExpressions.Trim(explorerHistoryPos);
		}
		else
			explorerHistoryPos = 0;
		explorerHistoryExpressions.Add(expr);
	}
	
	// evaluate the expression, direct deep evaluation here
	String s = "<can't evaluate>";
	MIValue valExpr = MICmd("data-evaluate-expression " + expr);

	if(valExpr.IsTuple() && valExpr.Find("value") >= 0)
	{
		MIValue const &tup = valExpr.Get("value");
		if(tup.IsString())
			s = tup.ToString();
	}
	
	// special behaviour for pointers and references
	// try to de-reference them
	if(s.StartsWith("@0x") || s.StartsWith("0x"))
	{
		MIValue valExpr;
		
		if(s.StartsWith("@0x"))
			// reference
			valExpr = MICmd("data-evaluate-expression *&" + expr);
		else
			// pointer
			valExpr = MICmd("data-evaluate-expression *" + expr);
		
		if(valExpr.IsTuple() && valExpr.Find("value") >= 0)
		{
			MIValue const &tup = valExpr.Get("value");
			if(tup.IsString())
				s = tup.ToString();
		}
	}
	
	s = expr + "=" + s;
	MIValue val(s);

	val.PackNames();
	AddAttribs("", val);
	val.FixArrays();

	bool more = TypeSimplify(val, false);
	while(more)
		more = TypeSimplify(val, true);

	// collect results
	Vector<String> vals;
	Vector<int> hints;
	CollectVariables(val, explorerChildExpressions, vals, hints);

	// update locals pane
	FillPane(explorer, explorerChildExpressions, vals);

	// update history buttons visibility
	explorerBackBtn.Enable(explorerHistoryPos > 0);
	explorerForwardBtn.Enable(explorerHistoryPos < explorerHistoryExpressions.GetCount() - 1);
}

void Gdb_MI2::onExploreExpr(ArrayCtrl *what)
{
	String expr;
	if(!what)
	{
		// if expression don't come from another ArrayCtrl
		// we use the expression editbox
		expr = ~explorerExprEdit;
	}
	else if(what == &members)
	{
		int line = what->GetCursor();
		if(line >= 0)
			expr = thisExpressions[line];
	}
	else
	{
		// otherwise, we use the expression from sending ArrayCtrl
		int line = what->GetCursor();
		int col = what->GetClickColumn();
		if(line >= 0 && (what != &watches || col != 0))
			expr = what->Get(line, 0);
	}
	// nothing to do on empty expression
	if(expr == "")
		return;

	doExplore(expr, true);
	
	// activate explorer tab
	tab.Set(4);
}

void Gdb_MI2::onExplorerChild()
{
	// click on first line (value line) does nothing
	int line = explorer.GetCursor();
	if(line < 0)
		return;
	if(line < explorerChildExpressions.GetCount())
		doExplore(explorerChildExpressions[line], true);
}

void Gdb_MI2::onExplorerBack()
{
	if(explorerHistoryPos < 1)
		return;
	explorerHistoryPos--;
	String expr = explorerHistoryExpressions[explorerHistoryPos];
	doExplore(expr, false);
}

void Gdb_MI2::onExplorerForward()
{
	if(explorerHistoryPos >= explorerHistoryExpressions.GetCount() - 1)
		return;
	explorerHistoryPos++;
	String expr = explorerHistoryExpressions[explorerHistoryPos];
	doExplore(expr, false);
}

void Gdb_MI2::ExplorerMenu(Bar& bar)
{
}
