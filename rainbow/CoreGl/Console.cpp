#if defined(flagWINGL) || defined(flagLINUXGL)

#include "Console.h"

NAMESPACE_UPP

void Console::Paint(Draw& w)
{
	Size sz = GetSize();

	#ifndef COLOR_ALPHA
	SystemDraw& sw = (SystemDraw&) w;
	sw.alpha = 100.f;
	Color frame = Yellow;
	Color body = Black;
	#else		
	Color frame = Yellow().Alpha(100);
	Color body = Black().Alpha(100);
	#endif
	
	w.DrawRect(sz, body);
	w.DrawRect(0, 0, sz.cx, 1, frame);
	w.DrawRect(sz.cx - 1, 1, 1, sz.cy - 1, frame);
	w.DrawRect(0, 1, 1, sz.cy, frame);
	w.DrawRect(1, sz.cy - 1, sz.cx - 2, 1, frame);
	//DrawFrame(w, sz, Yellow);
	//w.DrawImage(sz.cx - 18, sz.cy - 21, LinuxGlImg::ResizeMarker());
	w.DrawRect(1, sz.cy - 22, sz.cx - 2, 1, frame);
	w.Clip(5, 5, sz.cx - 10, sz.cy - 5 - 21);
	#ifndef COLOR_ALPHA
	sw.alpha = 255.f;
	#endif
	int y = 5;
	//int ty = Draw::GetStdFontCy() + 2;
	int ty = 12 + 2;
	for(int i = 0; i < fixedText.GetCount(); i++)
	{
		w.DrawText(5, y, fixedText[i].text, StdFont(), fixedText[i].ink);
		y += ty;
	}

	if(fixedText.GetCount() > 0 && floatText.GetCount() > 0)
	{
		#ifndef COLOR_ALPHA
		sw.alpha = 150.f;
		#endif
		w.DrawRect(5, y, sz.cx - 10, 1, frame);
		#ifndef COLOR_ALPHA
		sw.alpha = 255.f;
		#endif
		y += 2;
	}

	int lines = (sz.cy - y - 22) / ty;
	int n = fceil(sb.GetPos() / double(ty));
	int maxn = min(n + lines, currLine); 

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
	
	if(++currLine >= maxLine)
	{
		currLine = maxLine;
		for(int i = 0; i < maxLine - 1; i++)
			floatText[i] = floatText[i + 1];
	}

	int ty = Draw::GetStdFontCy() + 2;
	int total = currLine * ty;
	int fcnt = fixedText.GetCount();
	sb.SetTotal(total + 5 + 22);

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
	resize = p.x > sz.cx - 18 && p.y > sz.cy - 18;
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

void Console::Clear()
{
	currLine = 0;
	sb.Clear();
}

Console::Console() : init(true), resize(false)
{
	Add(sb.RightPos(2, 7).VSizePos(2, 22));
	Add(clear.LeftPos(2, 50).BottomPos(2, 18));
	clear.SetLabel("Clear");
	//clear.bg = Yellow;
	//clear.alpha = 150.f;
	clear <<= THISBACK(Clear);
	SetLines(3000);
	currLine = 0;
	scrollLine = 0;
}

END_UPP_NAMESPACE

#endif
