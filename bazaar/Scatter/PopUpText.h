#ifndef _PopUpText_PopUpText_h
#define _PopUpText_PopUpText_h


#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class PopUpInfo : public StaticText {
	Color color;
	bool opened;
	
public:
	void   Paint(Draw& w);

	PopUpInfo& SetFont(Font font)                      { LabelBase::SetFont(font); return *this; }
	PopUpInfo& SetInk(Color color)                     { LabelBase::SetInk(color); return *this; }
	PopUpInfo& SetAlign(int align)                     { LabelBase::SetAlign(align); return *this; }
	PopUpInfo& SetImage(const Image& img, int spc = 0) { LabelBase::SetImage(img, spc); return *this; }
	PopUpInfo& SetText(const char *text)               { LabelBase::SetText(text); return *this; }

	PopUpInfo& operator=(const char *s)                { SetText(s); return *this; }
	PopUpInfo& SetColor(const Color& c)                { color=c; return *this; }

	PopUpInfo();
	
	inline void Appear(Ctrl *owner, const int& x, const int& y, const int& gx=0, const int& gy=0) 
	{
		AppearOnly(owner);
		Move(owner, x, y, gx, gy);	 
	}
	inline void AppearOnly(Ctrl *owner)
	{
		if(IsOpen()) Close();
		Ctrl::PopUp(owner);
	}
	inline void AppearOnlyOpen(Ctrl *owner)
	{
		if (opened)
			return;
		AppearOnly(owner);
		opened = true;
	}
	inline void Move(Ctrl *owner, const int& x, const int& y, const int& gx=0, const int& gy=0)
	{
		SetRect(owner->GetScreenRect().left+x,owner->GetScreenRect().top+y,GetMinSize().cx+8+gx,GetMinSize().cy+4+gy);
	}	
};


#endif

