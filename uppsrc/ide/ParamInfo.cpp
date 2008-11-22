#include "ide.h"

void AssistEditor::SyncParamInfo()
{
	String qtf;
	int i = GetCursorLine();
	if(param_line >= 0 && param_line < GetLineCount() && i >= param_line && i < param_line + 10
	   && param_editfile == theide->editfile && GetWLine(param_line).StartsWith(param_test)) {
		int c = GetCursor();
		i = GetPos(param_line) + param_test.GetCount();
		if(c >= i) {
			int par = 0;
			int pari = 0;
			for(;;) {
				int ch = Ch(i++);
				if(i > c) {
					qtf = "[A1  " + DecoratedItem(param_item.name, param_item, param_item.natural, pari);
					break;
				}
				if(ch == ')') {
					if(par <= 0)
						break;
					par--;
				}
				if(ch == '(')
					par++;
				if(ch == ',' && par == 0)
					pari++;
			}
		}
	}
	if(param_qtf != qtf)
		param_info.SetQTF(qtf);
	Rect r = GetLineScreenRect(GetCursorLine());
	int cx = max(GetSize().cx - 30, 300);
	int h = param_info.GetHeight(cx);
	h = min(h, 550);
	int y = r.top - h - 6;
	if(y < GetWorkArea().top)
		y = r.bottom;
	r = RectC(r.left, y, min(param_info.GetWidth(), cx) + 32, h + 6);
	if(param_qtf != qtf || r != param_info.GetRect()) {
		param_qtf = qtf;
		if(IsNull(qtf)) {
			if(param_info.IsOpen())
				param_info.Close();
		}
		else {
			param_info.SetRect(r);
			if(!param_info.IsOpen())
				param_info.Ctrl::PopUp(this, false, false, false);
		}
	}
}

void AssistEditor::StartParamInfo()
{
	int x = GetCursor();
	param_line = GetLinePos(x);
	param_test = GetWLine(param_line).Mid(0, x);
	param_editfile = theide->editfile;
	SyncParamInfo();
}
