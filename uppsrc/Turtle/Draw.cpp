#include <CtrlCore/CtrlCore.h>

#ifdef GUI_TURTLE

#define LLOG(x) // LOG(x)

NAMESPACE_UPP

void SystemDraw::PutRect(const Rect& r, Color color)
{
	LLOG("Ctrl::PutRect " << r << ", color " << color);
	Ctrl::stat_putrect++;
	Point p = r.TopLeft();
	if(color == InvertColor()) {
		Ctrl::Put8(Ctrl::INVERTRECT);
		Ctrl::Put(r);
	}
	else {
		Size sz = r.GetSize();
		Point dp = p - pos;
		if(abs(dp.x) < 256 && abs(dp.y) < 256 && sz.cx < 256 && sz.cy < 256 && 0) {
			Ctrl::Put8(dp.x < 0 ? dp.y < 0 ? Ctrl::RECTNN : Ctrl::RECTNP
			                    : dp.y < 0 ? Ctrl::RECTPN : Ctrl::RECTPP);
			Ctrl::Put8(abs(dp.x));
			Ctrl::Put8(abs(dp.y));
			Ctrl::Put8(sz.cx);
			Ctrl::Put8(sz.cy);
		}
		else {
			Ctrl::Put8(Ctrl::RECT);
			Ctrl::Put(r);
		}
		Ctrl::Put8(color.GetR());
		Ctrl::Put8(color.GetG());
		Ctrl::Put8(color.GetB());
	}
	pos = p;
}

END_UPP_NAMESPACE

#endif
