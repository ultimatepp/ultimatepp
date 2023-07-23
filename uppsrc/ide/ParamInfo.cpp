#include "ide.h"

void AssistEditor::SyncParamInfo()
{
	if(!theide)
		return;
	String qtf;
	Ctrl *p = GetTopCtrl();
	int mpar = INT_MAX;
	int pos = 0;
	if(p && p->HasFocusDeep()) {
		for(int q = 0; q < PARAMN; q++) {
			ParamInfo& m = param[q];
			int i = GetCursorLine();
			if(m.line >= 0 && m.line < GetLineCount() && i >= m.line && i < m.line + 10
			   && m.editfile == theide->editfile && GetWLine(m.line).StartsWith(m.test)) {
				int c = GetCursor32();
				i = GetPos32(m.line) + m.test.GetCount();
				if(c >= i) {
					int par = 0;
					int pari = 0;
					int str = 0;
					bool esc = false;
					for(;;) {
						int ch = Ch(i++);
						if(i > c) {
							if(par < mpar) {
								qtf = "[A1  " + SignatureQtf(m.item.name, m.item.pretty, pari);
								mpar = par;
								pos = m.pos;
							}
							break;
						}
						if(findarg(ch, ')', ']', '}') >= 0) {
							if(par <= 0)
								break;
							par--;
						}
						else
						if(findarg(ch, '(', '[', '{') >= 0)
							par++;
						else
						if(findarg(ch, '\"', '\'') >= 0 && !esc)
							str = str ? 0 : ch;
						else
						if(ch == ',' && par == 0 && str == 0)
							pari++;
						esc = ch == '\\';
					}
				}
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
	r = RectC(r.left, y, min(param_info.GetWidth(), cx) + 8, h + 6);
	r.OffsetHorz(GetColumnLine(pos).x * GetFontSize().cx);
	r.OffsetHorz(min(GetWorkArea().right - r.right, 0));
	if(param_qtf != qtf || r != param_info.GetRect()) {
		param_qtf = qtf;
		if(IsNull(qtf)) {
			if(param_info.IsOpen())
				param_info.Close();
		}
		else {
			param_info.SetRect(r);
			if(!param_info.IsOpen() && !IsSelection())
				param_info.Ctrl::PopUp(this, false, false, false);
		}
	}
}

void AssistEditor::StartParamInfo(const AssistItem& m, int pos)
{
	if(!theide)
		return;
	int x = GetCursor32();
	ParamInfo& f = param[parami];
	f.line = GetLinePos32(x);
	f.test = GetWLine(f.line).Mid(0, x);
	f.item = m;
	f.editfile = theide->editfile;
	f.pos = pos;
	SyncParamInfo();
	parami = (parami + 1) % PARAMN;
}

void AssistEditor::State(int reason)
{
	if(reason == FOCUS)
		SyncParamInfo();
	CodeEditor::State(reason);
}

Point AssistEditor::GetCurrentPos() const
{
	int64 pos = GetCursor64();
	Point p;
	p.y = GetLinePos64(pos);
	p.x = (int)pos;
	return p;
}

int AssistEditor::GetCurrentLine()
{
	return GetLine(GetCursor64());
}
