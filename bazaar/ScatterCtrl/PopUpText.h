#ifndef _PopUpText_PopUpText_h
#define _PopUpText_PopUpText_h

namespace Upp {
	
class PopUpText {
public:
	Point pos;
	Size sz;
	String text;
	Font font = StdFont();
	Color color = SColorText;
	Color background = SColorFace;
	bool show = false;
	int xmargin = StdFont().GetWidth('I');
	
	PopUpText &SetText(String _text);
	PopUpText &SetColor(Color _color)		{color = _color;	return *this;}
	PopUpText &SetBackground(Color _color)	{background = _color;return *this;}
	PopUpText &Show(bool _show = true)		{show = _show;		return *this;}
	PopUpText &Hide()						{
	show = false;		
	return *this;}
	PopUpText &Move(Point& p)				{pos = p;			return *this;}	
	Size GetSize()							{return sz;}
	
	template <class T>
	void DoPaint(T& w) {
		if (!show)
			return;
		Rect rect(pos.x, pos.y, pos.x + sz.cx + 3*xmargin, pos.y + sz.cy);
		FillRectangle(w, rect, background);
		DrawRectangle(w, rect, 1, 1, Black());
		DrawText(w, rect.left + xmargin, rect.top, 0, text, font, color);
	}
};
class PopUpInfo : public StaticText {
	Color color;
	bool opened;
	
public:
	void   Paint(Draw& w);

	PopUpInfo& SetFont(Upp::Font font)                 { LabelBase::SetFont(font); return *this; }
	PopUpInfo& SetInk(Color color)                     { LabelBase::SetInk(color); return *this; }
	PopUpInfo& SetAlign(int align)                     { LabelBase::SetAlign(align); return *this; }
	PopUpInfo& SetImage(const Image& img, int spc = 0) { LabelBase::SetImage(img, spc); return *this; }
	PopUpInfo& SetText(const char *text)               { LabelBase::SetText(text); return *this; }

	PopUpInfo& operator=(const char *s)                { SetText(s); return *this; }
	PopUpInfo& SetColor(const Color& c)                { color=c; return *this; }

	PopUpInfo();
	
	inline void Appear(Ctrl *owner, const Point& p, const int& gx=0, const int& gy=0) 
	{
		AppearOnly(owner);
		Move(owner, p, gx, gy);	 
	}
	inline void AppearOnly(Ctrl *owner)
	{
		if(IsOpen()) 
			Close();
		Ctrl::PopUp(owner);
	}
	inline void AppearOnlyOpen(Ctrl *owner)
	{
		if (opened)
			return;
		AppearOnly(owner);
		opened = true;
	}
	inline void Move(Ctrl *owner, const Point& p, int gx=0, int gy=0)
	{
		SetRect(owner->GetScreenRect().left+p.x,owner->GetScreenRect().top+p.y,GetMinSize().cx+8+gx,GetMinSize().cy+4+gy);
	}	
};

}

#endif

