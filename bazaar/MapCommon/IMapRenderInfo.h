#ifndef MAP_COMMON__I_MAP_RENDER_INFO_H
#define MAP_COMMON__I_MAP_RENDER_INFO_H

#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>
using namespace Upp;

class IMapRenderInfo
{
public:
	virtual ~IMapRenderInfo() {}

	virtual void  RenderInfo(Painter* w) { RenderInfo((Draw*)w); }
	virtual void  RenderInfo(Draw* w) {}

	virtual Point GetRenderCenter() { return GetRenderRect().CenterPoint(); }
	virtual Size  GetRenderSize()   { return GetRenderRect().GetSize(); }
	virtual Rect  GetRenderRect()   = 0;

	virtual Point GetPageCenter()   { return GetPageRect().CenterPoint(); }
	virtual Point GetPageOffset()   = 0;
	virtual Size  GetPageSize()     { return GetPageRect().GetSize(); }
	virtual Rect  GetPageRect()     = 0;
	virtual Point GetPagePos()      { return GetPageRect().TopLeft(); }
};

#endif
