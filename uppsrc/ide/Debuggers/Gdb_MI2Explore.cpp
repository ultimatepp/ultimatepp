#include "Debuggers.h"

// sync explorer pane
#ifdef flagMT
void Gdb_MI2::SyncExplorer()
{
	// re-enter if called from main thread
	if(IsMainThread())
	{
		debugThread.Start(THISBACK(SyncExplorer));
		return;
	}
	
	INTERLOCKED {
		IncThreadRunning();
		
		try
		{
	
			VectorMap<String, String> prev = DataMap(explorer);
		
			// get expression from editfield
			String expr;
			{
				GuiLock __;
				expr = explorerExprEdit;
			}
			
			// create a vari object and evaluate '*this' expression
			VarItem vItem(this);
			vItem.Evaluate(expr);
	
			RaiseIfStop();
	
			// get children if complex variable
			Vector<VarItem> children;
			if(vItem.kind == VarItem::COMPLEX)
				children = vItem.GetChildren();
			else
				children.Add(vItem);
	
			RaiseIfStop();
	
			// fill explorer memners expressions, short expressions and values
			explorerExpressions.Clear();
			explorerValues.Clear();
			for(int iVar = 0; iVar < children.GetCount(); iVar++)
			{
				VarItem &v = children[iVar];
				explorerExpressions << v.shortExpression;
				explorerValues << v.value;
			}
		
			RaiseIfStop();
	
			// update 'this' pane
			FillPane(explorer, explorerExpressions, explorerValues);
	
			// simplify batch
			for(int iVar = 0; iVar < children.GetCount(); iVar++)
			{
				RaiseIfStop();
				while(children[iVar].Simplify())
					RaiseIfStop();
	
				VarItem &v = children[iVar];
	
				explorerValues[iVar] = v.value;
				{
					GuiLock __;
					explorer.Set(iVar, 1, v.value);
				}
			}
	
			// when finished, mark changed values
			MarkChanged(prev, explorer);
			
			explorerSynced = true;
		}
		catch(...)
		{
			explorerSynced = false;
		}
	
		DecThreadRunning();
	}
}
#else
void Gdb_MI2::SyncExplorer(const Vector<VarItem>& children_)
{
	static VectorMap<String, String> prev;
	Vector<VarItem> children = clone(children_);
	if(children.IsEmpty())
	{
		prev = DataMap(explorer);

		// get expression from editfield
		String expr = explorerExprEdit;
		if(expr.IsEmpty())
		{
			explorerSynced = true;
			return;
		}
	
		// create a vari object and evaluate the expression
		VarItem vItem(this, expr);

		// get children if complex variable
		if(vItem.kind == VarItem::COMPLEX)
			children = clone(vItem.GetChildren());
		else
			children.Add(vItem);

		// fill explorer memners expressions, short expressions and values
		explorerExpressions.Clear();
		explorerValues.Clear();
		for(int iVar = 0; iVar < children.GetCount(); iVar++)
		{
			VarItem &v = children[iVar];
			explorerExpressions << v.shortExpression;
			explorerValues << v.value;
		}
	
		// update 'this' pane
		FillPane(explorer, explorerExpressions, explorerValues);
	
		exploreCallback.Set(500, THISBACK1(SyncExplorer, DeepClone(children)));
		return;
	}
	
	// simplify batch
	for(int iVar = 0; iVar < children.GetCount(); iVar++)
	{
		if(children[iVar].Simplify())
		{
			VarItem &v = children[iVar];
			explorer.Set(iVar, 1, v.value);
			explorerValues[iVar] = v.value;
			exploreCallback.Set(100, THISBACK1(SyncExplorer, DeepClone(children)));
			return;
		}
	}
	
	for(int iVar = 0; iVar < children.GetCount(); iVar++)
		explorer.Set(iVar, 1, children[iVar].value);

	// when finished, mark changed values
	MarkChanged(prev, explorer);
	explorerSynced = true;
}
#endif

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
	
	explorerSynced = false;
	SyncExplorer();
	
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
	if(line < explorerExpressions.GetCount())
		doExplore(explorerExpressions[line], true);
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
