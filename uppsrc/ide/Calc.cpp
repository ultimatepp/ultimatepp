#include "ide.h"

IdeCalc::IdeCalc()
{
	Highlight("calc");
	NoHorzScrollbar();
	HideBar();
}

int LfToSpaceFilter(int c)
{
	return c == '\n' ? ' ' : c;
}

void IdeCalc::Execute()
{
	int li = GetLineCount() - 1;
	if(IsSelection()) {
		String s = GetSelection();
		if(s.GetLength() < 80) {
			SetCursor(GetLength64());
			Paste(Filter(s, LfToSpaceFilter).ToWString());
		}
		return;
	}
	if(GetLine(GetCursor64()) != li) {
		WString s = GetWLine(GetLine(GetCursor64()));
		if(s[0] == '$') s = s.Mid(1);
		SetCursor(GetLength64());
		Paste(s);
		return;
	}
	String txt;
	try {
		ArrayMap<String, EscValue>& g = UscGlobal();
		for(int i = 0; i < g.GetCount(); i++)
			vars.GetAdd(g.GetKey(i)) = g[i];
		String s = GetUtf8Line(li);
		if(IsNull(s))
			return;
		EscValue v = Evaluatex(s, vars);
		txt = v.ToString(GetSize().cx / max(1, GetFont().Info()['x']), 4, true);
		vars.GetAdd("_") = v;
	}
	catch(CParser::Error e) {
		const char *x = strchr(e, ':');
		txt << "ERROR: " << (x ? x + 1 : ~e);
	}
	SetCursor(GetPos64(li));
	Paste("$");
	SetCursor(GetLength64());
	Paste("\n");
	Paste(ToUtf32(txt));
	Paste("\n");
}

void IdeCalc::LeftDouble(Point p, dword flags)
{
	CodeEditor::LeftDouble(p, flags);
	if(IsSelection())
		Execute();
}

bool IdeCalc::Key(dword key, int count)
{
	switch(key) {
	case K_ENTER:
		Execute();
		break;
	default:
		return CodeEditor::Key(key, count);
	}
	return true;
}
