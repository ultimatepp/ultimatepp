#include "Console.h"

NAMESPACE_UPP

void Console::Paint(Draw& w)
{
	SystemDraw& sw = (SystemDraw&) w;
	Size sz = GetSize();
	sw.alpha = 100.f;
	w.DrawRect(sz, Black);
	w.DrawRect(0, 0, sz.cx, 1, Yellow);
	w.DrawRect(sz.cx - 1, 1, 1, sz.cy - 1, Yellow);
	w.DrawRect(0, 1, 1, sz.cy, Yellow);
	w.DrawRect(1, sz.cy - 1, sz.cx - 2, 1, Yellow);
	//DrawFrame(w, sz, Yellow);
	w.DrawImage(sz.cx - 10, sz.cy - 10, WinGlImg::ResizeMarker());
	w.Clip(5, 5, sz.cx - 10, sz.cy - 10);
	sw.alpha = 255.f;
	int y = 5;
	int ty = Draw::GetStdFontCy() + 2;
	for(int i = 0; i < fixedText.GetCount(); i++)
	{
		w.DrawText(5, y, fixedText[i].text, StdFont(), fixedText[i].ink);
		y += ty;
	}

	if(fixedText.GetCount() > 0 && floatText.GetCount() > 0)
	{
		sw.alpha = 150.f;
		w.DrawRect(5, y, sz.cx - 10, 1, Yellow);
		y += 2;
		sw.alpha = 255.f;
	}

	int lines = (sz.cy - y - 5) / ty;
	int n = sb.GetPos() / ty;
	int maxn = min(n + lines, maxLine); 

	while(n < maxn)
	{
		LineInfo& lf = floatText[n];
		w.DrawText(5, y, lf.text, StdFont(), lf.ink);
		y += ty;
		++n;
	}
	
	w.End();
}

void Console::Log(int line, const char* text, Color ink)
{
	LineInfo& lf = fixedText.GetAdd(line);
	lf.ink = ink;
	lf.text = text;
}

void Console::Log(const char* text, Color ink)
{
	bool isend = sb.IsEnd();
	
	if(++currLine > maxLine)
	{
		currLine = maxLine;
		for(int i = 0; i < maxLine - 1; i++)
			floatText[i] = floatText[i + 1];
	}

	int ty = Draw::GetStdFontCy() + 2;
	sb.SetTotal(currLine * ty);
	if(isend)
		sb.GoEnd();

	LineInfo& lf = floatText[currLine - 1];
	lf.ink = ink;
	lf.text = text;	
}

void Console::LeftDown(Point p, dword keys)
{
	SetCapture();
	pos = p;
	rs = GetSize();
	tl = GetScreenRect().TopLeft();
	Size sz = GetSize();
	resize = p.x > sz.cx - 10 && p.y > sz.cy - 10;
}

void Console::LeftUp(Point p, dword keys)
{
	ReleaseCapture();
}

void Console::MouseMove(Point p, dword keys)
{
	if(HasCapture())
	{
		Rect r = GetRect();
		bool isend = sb.IsEnd();
		if(resize)
		{
			tl = Point(p.x - pos.x, p.y - pos.y);
			SetConsoleRect(r.left, r.top, max(2, rs.cx + tl.x), max(2, rs.cy + tl.y));
		}
		else
		{
			tl += Point(p.x - pos.x, p.y - pos.y);
			SetConsoleRect(tl.x, tl.y, r.Width(), r.Height());
		}
		if(isend)
			sb.GoEnd();
	}
}

void Console::Layout()
{
}

void Console::SetConsoleRect(int x, int y, int cx, int cy)
{
	int width = cx < 0 ? 300 : cx;
	int height = cy < 0 ? 5 + 5 + (Draw::GetStdFontCy() + 2) * 30 + 3 : cy;
	SetRect(x, y, width, height);
}

void Console::SetLines(int lines)
{
	maxLine = lines;
	floatText.SetCount(lines);
}

void Console::Init(Ctrl& parent)
{
	if(!init)
		return;
	init = false;
	this->parent = &parent;
	SetConsoleRect(parent.GetSize().cx - 5 - 300, 5);
	sb.SetFrameSize(5, false).SetAlign(AlignedFrame::RIGHT);
	Show();
}

void Console::Show(bool b)
{
	if(IsOpen())
	{
		if(!b)
			Close();
	}
	else
	{
		if(b)
			PopUp(parent, true, false);
	}
}

Console::Console() : init(true), resize(false)
{
	Add(sb.RightPos(2, 7).VSizePos(2, 10));
	SetLines(1000);
	currLine = 0;
	scrollLine = 0;
}

END_UPP_NAMESPACE