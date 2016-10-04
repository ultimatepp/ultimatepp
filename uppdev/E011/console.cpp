#include "e011.h"

Console::Console() {
	wrap_text = true;
	SetReadOnly();
	NoHorzScrollbar();
	SetColor(PAPER_READONLY, SColorPaper);
	SetFont(Font(Font::COURIER, 12));
	//TabSize(0);
}

void Console::LeftDouble(Point p, dword) {
	WhenSelect();
}

void Console::RightDown(Point p, dword)
{
	MenuBar::Execute(WhenBar);
}

static int sAppf(int c) {
	return c >= ' ' || c == '\n' ? c : c == '\t' ? ' ' : 0;
}

void Console::Append(const String& s) {
	if(s.IsEmpty()) return;
	int l, h;
	GetSelection(l, h);
	if(GetCursor() == GetLength()) l = -1;
	EditPos p = GetEditPos();
	SetEditable();
	MoveTextEnd();
	String t = Filter(s, sAppf);
	if(wrap_text) {
		int mg = sb.GetReducedViewSize().cx / ScreenInfo().GetFontInfo(GetFont()).GetAveWidth();
		for(const char *q = t; *q; q++) {
			if(mg > 2 && GetColumnLine(GetCursor()).x >= mg - 1)
				Paste(ToUnicode("\n", CHARSET_WIN1252));
			Paste(String(*q, 1).ToWString());
		}
	}
	else
		Paste(t.ToWString());
	SetReadOnly();
	if(l >= 0) {
		SetEditPos(p);
		SetSelection(l, h);
	}
}

bool Console::Key(dword key, int count) {
	if(key == K_ENTER) {
		WhenSelect();
		return true;
	}
	return LineEdit::Key(key, count);
}
