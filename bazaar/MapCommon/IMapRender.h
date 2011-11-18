#ifndef MAP_COMMON__I_MAP_RENDER_H
#define MAP_COMMON__I_MAP_RENDER_H

#include "IMapItem.h"
#include "IMapRenderInfo.h"

class IMapRender : public IMapItem, public IMapRenderInfo
{
public:
	IMapRender(IMapRender* render);
	virtual ~IMapRender() { Clear(); }

	virtual void  RenderItems(Draw* w);
	virtual void  Render(Draw* w);

	virtual Rect  GetPageRect()   { return GetTopRender() ? GetTopRender()->GetPageRect()   : Null; }
	virtual Point GetPageOffset() { return GetTopRender() ? GetTopRender()->GetPageOffset() : Null; }
	virtual Rect  GetRenderRect() { return GetTopRender() ? GetTopRender()->GetRenderRect() : Null; }

	virtual void  RefreshView() { if (GetTopRender()) GetTopRender()->RefreshView(); }

	virtual bool  Load(const char* fp) { return false; }
	virtual bool  Zoom(int level)      { return false; }
	virtual Rect  GetRect();

	virtual double GetZoom(int i) { return GetTopRender() ? GetTopRender()->GetZoom(i) : 1.0; }
	virtual int    GetZoomCount() { return GetTopRender() ? GetTopRender()->GetZoomCount() : 0; }
	virtual int    GetCurrentZoom() { return GetTopRender() ? GetTopRender()->GetCurrentZoom() : 0; }

	virtual String GetItemType() { return "IMapRender"; }

	IMapRender* Antialiased(bool use = true, bool subpixel = false);
	IMapRender* Subpixel();
	IMapRender* NoAA();

	IMapRender* ClearColor(const Color& c) { _clearColor  = c; return this; }
	IMapRender* Opacity(double value)      { _opacity = value; return this; }
	virtual void Clear();

	virtual double GetOpacity() { return _opacity; }
	virtual dword  GetQuality() { return _quality; }
	virtual Color  GetClearColor() { return _clearColor; }
	virtual dword  GetRenderType() { return _renderType; }

	virtual Point SceneToView(const Point& p) { return GetTopRender() ? GetTopRender()->SceneToView(p) : p; }
	virtual Size  SceneToView(const Size& sz) { return GetTopRender() ? GetTopRender()->SceneToView(sz) : sz; }
	virtual Rect  SceneToView(const Rect& rt) { return GetTopRender() ? GetTopRender()->SceneToView(rt) : rt; }
	virtual Array<Point> SceneToView(const Array<Point>& p) { return GetTopRender() ? GetTopRender()->SceneToView(p) : p; }
	virtual Vector<Point> SceneToView(const Vector<Point>& p) { return GetTopRender() ? GetTopRender()->SceneToView(p) : p; }

	virtual Point ViewToScene(const Point& p) { return GetTopRender() ? GetTopRender()->ViewToScene(p) : p; }
	virtual Size  ViewToScene(const Size& sz) { return GetTopRender() ? GetTopRender()->ViewToScene(sz) : sz; }
	virtual Rect  ViewToScene(const Rect& rt) { return GetTopRender() ? GetTopRender()->ViewToScene(rt) : rt; }
	virtual Array<Point> ViewToScene(const Array<Point>& p) { return GetTopRender() ? GetTopRender()->ViewToScene(p) : p; }
	virtual Vector<Point> ViewToScene(const Vector<Point>& p) { return GetTopRender() ? GetTopRender()->ViewToScene(p) : p; }

	virtual Rect  CurrentViewRect() { return GetTopRender() ? GetTopRender()->CurrentViewRect() : GetRect(); }
	virtual Rect  CurrentViewSize() { return CurrentViewRect().GetSize(); }
	virtual Rect  CurrentSceneRect() { return GetTopRender() ? GetTopRender()->CurrentSceneRect() : GetRect(); }
	virtual Size  CurrentSceneSize() { return CurrentSceneRect().GetSize(); }

	template <class T>
	Ptr<T> AddItem() { T* t = new T(this); _items.Add(t); return t; }
	virtual Vector<Ptr<IMapItem> >& GetItems() { return _items; }
	const virtual Vector<Ptr<IMapItem> >& GetItems() const { return _items; }

public:
	virtual void  StateOff(dword state);
	virtual void  StateOn (dword state);
	virtual bool  IsState (dword state);
	virtual void  PerformState(dword state, const String& param = String());

public:
	virtual bool ProcessLeftDouble (Point p, dword keyflags);
	virtual bool ProcessLeftDown   (Point p, dword keyflags);
	virtual bool ProcessLeftDrag   (Point p, dword keyflags);
	virtual bool ProcessLeftUp     (Point p, dword keyflags);

	virtual bool ProcessRightDouble(Point p, dword keyflags);
	virtual bool ProcessRightDown  (Point p, dword keyflags);
	virtual bool ProcessRightDrag  (Point p, dword keyflags);
	virtual bool ProcessRightUp    (Point p, dword keyflags);

	virtual bool ProcessMouseMove  (Point p, dword keyflags);

	virtual bool ProcessMouseEnter();
	virtual bool ProcessMouseLeave();

protected:
	dword  _renderType;
	dword  _quality;
	double _opacity;
	Color  _clearColor;
	Vector<Ptr<IMapItem> > _items;
	enum { RENDER_DRAW, RENDER_PAINTER };
};

#define ZOOM_FUNCTIONS_DECLARE(Type) \
inline Type ViewToScene(Ptr<IMapItem> item, const Type& p) \
{ \
	if (!item) return p; \
	if ( item->GetTopRender() == NULL) return p; \
	return item->GetTopRender()->ViewToScene(p); \
} \
inline Type SceneToView(Ptr<IMapItem> item, const Type& p) \
{ \
	if (!item) return p; \
	if ( item->GetTopRender() == NULL) return p; \
	return item->GetTopRender()->SceneToView(p); \
}

ZOOM_FUNCTIONS_DECLARE(Point);
ZOOM_FUNCTIONS_DECLARE(Size);
ZOOM_FUNCTIONS_DECLARE(Rect);
ZOOM_FUNCTIONS_DECLARE(Array<Point>);
ZOOM_FUNCTIONS_DECLARE(Vector<Point>);

#endif
