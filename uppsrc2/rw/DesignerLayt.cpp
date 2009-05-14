#include "Designer.h"

void LayoutDesigner::SetLayout(int i, int li) {
	ClearSelection();
	layouti = i;
	layoutlist = i;
	langi = li;
	if(langlist.GetCount())
		langlist = li;
	itab.Clear();
	if(layouti >= 0) {
		Form& l = layout[layouti];
		SetFormSize(l.size);
		SynchronizeCtrls();
	}
	else {
		ctrl.Clear();
		ref_ctrl.Clear();
		mnem_ctrl.Clear();
		form.Hide();
		itab.Clear();
	}
	sb = Point(0, 0);
	form.Show(layouti >= 0);
	MakeCaption();
	ShowStatus();
	langlist.Enable(langlist.GetCount());
	SynchronizeMnemonic();
}

void LayoutDesigner::EnterLayout() {
	SetLayout(~layoutlist, langlist.GetCount() ? langlist.GetIndex() : 0);
}

void LayoutDesigner::EnterLanguage() {
	int i = -1;
	if(itemi.GetCount())
		i = itemi.Top();
	EnterLayout();
	SelectOneItem(i);
}

void LayoutDesigner::SetupLayoutList() {
	layoutlist.Clear();
	int i;
	for(i = 0; i < layout.GetCount(); i++)
		layoutlist.Add(i, layout[i].name);
	layoutlist.Enable(i);
}

int FilterCid(int c) {
	return IsAlNum(c) || c == '_' ? c : 0;
}

void LayoutDesigner::NewLayout() {
	WithLayoutLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "New Layout");
	dlg.layout.SetFilter(FilterCid);
	dlg.ActiveFocus(dlg.layout);
	if(dlg.Run() == IDOK) {
		int i = layout.GetCount();
		Form& l = layout.Add();
		l.name = dlg.layout;
		l.size = Size(400, 200);
		SetupLayoutList();
		SetLayout(i);
	}
}

void LayoutDesigner::RemoveLayout() {
	if(layouti < 0) return;
	if(PromptYesNo("Remove current layout ?")) {
		int i = layouti;
		SetLayout(-1);
		layout.Remove(i);
		SetupLayoutList();
		SetLayout(min(i, layout.GetCount() - 1));
		Refresh();
	}
}

void LayoutDesigner::RenameLayout() {
	if(layouti < 0) return;
	WithLayoutLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Rename Layout");
	dlg.ActiveFocus(dlg.layout);
	dlg.layout.SetFilter(FilterCid);
	dlg.layout = layout[layouti].name;
	if(dlg.Run() == IDOK) {
		layout[layouti].name = dlg.layout;
		SetupLayoutList();
		SetLayout(layouti);
	}
}

void  LayoutDesigner::NextLayout() {
	if(layoutlist.GetCount() == 0) return;
	layoutlist.SetIndex((layoutlist.GetIndex() + 1) % layoutlist.GetCount());
	EnterLayout();
}

void  LayoutDesigner::PrevLayout() {
	if(layoutlist.GetCount() == 0) return;
	layoutlist.SetIndex((layoutlist.GetIndex() + layoutlist.GetCount() - 1) % layoutlist.GetCount());
	EnterLayout();
}
