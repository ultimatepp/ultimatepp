#include <src/support/support.h>
#ifdef flagGUI

bool DropSelect(String& s, const char *title, const char *label, const Vector<String>& lst)
{
	int i;

	WithDropSelectPanel<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, title);
	dlg.lbl = label;
	for (i = 0; i < lst.GetCount(); i++)
	{
		dlg.list.Add(AsString(lst[i]));
	}
	if(dlg.list.GetCount() > 0)
		//dlg.list.SetIndex(0);
		dlg.list.SetData(s);

	if(dlg.Execute() == IDOK) {
		s = ~dlg.list;
		return true;
	}
	return false;
}

bool PromptPanelOKCancel(const String& tag, const String& text) {

	WithpromptPanel<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Info");
	dlg.Sizeable();
	dlg.tag = tag;
	dlg.text.Set(text);
	if(dlg.Execute() == IDOK) {
		return true;
	}
	return false;
}

/*
MyVectorText::MyVectorText() {

	OutlineWidth(2).OutlineColor(Null).FillColor(Null).MarginHorz(0).MarginVert(0).Style(1).Rounding(14);

	return;
}
*/

Zoom ZoomP(Ctrl &ctrl, int percent) {
	Zoom z;
	z.d = ctrl.GetSize().cx;
	z.m = (int)(((double)percent/1000)*z.d);
	return z;
}

#endif
