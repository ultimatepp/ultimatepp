#include "CtrlLib/CtrlLib.h"


#define DEBUG3	1 // Debug für Test in den Methoden

#if DEBUG3 != 0 // Debug für Test in den Methoden
	#define D3(x) RLOG("[" << GetSysTime() << "." << int(GetTickCount() % 10000u) << "] " << x)
#else
	#define D3(x)
#endif



using namespace Upp;

struct App : TopWindow {
	TreeCtrl   tree;
	Vector<int> parent, parent2;
	int i;

	typedef App CLASSNAME;

	void DropInsert(int parent, int ii, PasteClip& d)
	{
//		if (0 == parent)
		if (1 < parent)
			return;
		
		
		if(AcceptInternal<TreeCtrl>(d, "mytreedrag")) {
//			if (7 < parent)
//			{
//				if(PromptOKCancel("Dialog"))
//				{
//				tree.CancelMode();
					//return;
//					sort = true;
//				}
//			}


			tree.AdjustAction(parent, d);

			const TreeCtrl &src = GetInternal<TreeCtrl>(d); // JT
			Vector<int> sel = src.GetSel(); // JT
			// Etc.
			for(int i=0;i<sel.GetCount();i++)
			{
				DUMP(parent);
				DUMP(ii);
				DUMP(sel[i]);	
			}			


			tree.InsertDrop(parent, ii, d);
			tree.SetFocus();
			D3(Sprintf("AcceptInternal<TreeCtrl>(d, 'mytreedrag'): parent: %2d, ii %2d", parent, ii));
			return;
		}
		if(AcceptText(d)) {
			tree.AdjustAction(parent, d);
			tree.SetCursor(tree.Insert(parent, ii, Image(), GetString(d)));
			tree.SetFocus();
			D3(Sprintf("AcceptText(d): parent: %2d, ii %2d", parent, ii));
			return;
		}
	}

	void Drag()
	{
		if(tree.DoDragAndDrop(InternalClip(tree, "mytreedrag"),
		                       tree.GetDragSample()) == DND_MOVE)
		{
			tree.RemoveSelection();
			D3(Sprintf("DoDragAndDrop"));
			i++;
		}
		
		/**
		for (int x = 0; x < parent.GetCount(); x++)
			D3(Sprintf("Element Nr.: %3d, mit Wert: %3d ",x,parent[x] ));
			*/
		
		D3(Sprintf("Elements: %d",tree.GetLineCount()));
		if (0 && PromptOKCancel("Sortieren?"))
		{
			Value i = 1;
			tree.Sort(0);
			for (int i = 1; i <= tree.GetChildCount(0); i++)
			{
				int n = tree.GetItemAtLine(i);
				Value a = tree.GetValue(n);
				Value b = tree.GetValue(i);
				D3(" sorted (" << i << ")  get with i: " << b <<  "\t get with n " << n << ": " << a);
			}
		}
		else
		{
			for (int i = 1; i <= tree.GetChildCount(0); i++)
			{
				int n = tree.GetItemAtLine(i);
				Value a = tree.GetValue(n);
				Value b = tree.GetValue(i);
				D3(Sprintf(" unsorted (%2d)  get with i: ", i)  + String(b) + "\t get with n: " +String(a));
			}
		}
	}


	App() {
		int i = 0;
		Add(tree.SizePos());
		parent.Add(0);
		//tree.SetRoot(Image(), "The Tree");
		tree.NoRoot();
		for(int i = 1; i < 10; i++) {
			/**
			parent.Add(tree.Add(parent[rand() % parent.GetCount()], Image(),
			            FormatIntRoman(i, true)));
			if((rand() & 3) == 0)
				tree.Open(parent.Top());
				*/
			parent.Add(tree.Add(0, Image(),FormatIntRoman(i, true)));
//			TreeCtrl::Node tmp(i,FormatIntRoman(i, true));
			
//			parent.Add(tree.Insert(0,i+10,tmp));
			
		}
		tree.Open(0);
		tree.WhenDropInsert = THISBACK(DropInsert);
		tree.WhenDrag = THISBACK(Drag);
		tree.MultiSelect();
		D3(Sprintf("Elements: %d",tree.GetLineCount()));
		for (int i = 1; i <= tree.GetChildCount(0); i++)
		{
			int n = tree.GetItemAtLine(i);
			Value a = tree.GetValue(n);
			Value b = tree.GetValue(i);
			D3(Sprintf(" just created (%2d)  get with i: ", i)  + String(b) + "\t get with n: " +String(a));
		}
		Sizeable();
	}
};

GUI_APP_MAIN
{
	App().Run();
}
