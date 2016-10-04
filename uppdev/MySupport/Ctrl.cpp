#include <MySupport/support.h>
#ifdef flagGUI

#define IMAGECLASS commonImg
#define IMAGEFILE <MySupport/common.iml>
#include <Draw/iml_source.h>

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

Zoom ZoomP(Ctrl &ctrl, int percent) {
	Zoom z;
	z.d = ctrl.GetSize().cx;
	z.m = (int)(((double)percent/1000)*z.d);
	return z;
}

void DirSel(EditField& f, FrameRight<Button>& b) {
	f.AddFrame(b);
	b <<= callback1(&sSetFolder, &f);
	b.SetImage(CtrlImg::smallright()).NoWantFocus();
}

static void sSetFolder(EditField *f) {
	FileSel sSD;
	sSD.ActiveDir((const char*)~AsString(f->GetText()));
	if(!sSD.ExecuteSelectDir()) return;
	*f <<= ~sSD;
}

#endif
