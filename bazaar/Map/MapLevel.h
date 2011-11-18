#ifndef MAP_RENDER__MAP_RENDER_H
#define MAP_RENDER__MAP_RENDER_H

#include <MapCommon/MapCommon.h>
#include <MapBG/MapBG.h>

#define IMAGECLASS MapLevelImg
#define IMAGEFILE <Map/MapLevel.iml>
#include <Draw/iml_header.h>

class MapLevel : public Ctrl, public IMapRender
{
public:
	MapLevel();
	virtual ~MapLevel() { ClearBG(); }

	// render
public:
	virtual void  Render(Draw* w);
	virtual void  RenderInfo(Draw* w);

	virtual Rect  GetRenderRect();
	virtual Point GetPageOffset();
	virtual Rect  GetPageRect();

	virtual void  Layout() { PagePos(GetPagePos()); UpdateMap(); }
	virtual void  Paint(Draw& w);
	virtual void  ClearBG();
	virtual bool  Zoom(int level);
	virtual Rect  GetRect();

	virtual double GetZoom(int i)   { return _zoomLevels[i].GetZoom(); }
	virtual int    GetZoomCount()   { return _zoomLevels.GetCount(); }
	virtual int    GetCurrentZoom() { return _zoomLevel; }

	virtual void  RefreshView();

	// settings
	bool LoadMap(const char* fp);
	void UpdateMap();

	template <class T>
	Ptr<IMapRender> AddLayer(int pos)
	{
		T* render = new T(this);
		_layers.Add(pos, render);
		Sort(_layers);
		Refresh();
		return render;
	}

	int GetRenderCount() { return _layers.GetCount(); }
	Ptr<IMapRender> GetRender(int i) { return _layers[i]; }

	MapLevel& HighQuality(bool flag = true) { _highQuality = flag; Refresh(); return *this; }
	MapLevel& EditMode(bool flag = true) { _editMode = flag; Refresh(); return *this; }
	MapLevel& NoEditMode() { EditMode(false); return *this; }

	// navigation
public:
	MapLevel& PagePos(const Point& p);

	MapLevel& MoveTo(const Point& p);
	MapLevel& MoveBy(const Point& p);

	MapLevel& ZoomTo(int zoom);
	MapLevel& ZoomIn();
	MapLevel& ZoomOut();

	virtual Point SceneToView(const Point& p);
	virtual Size  SceneToView(const Size& sz);
	virtual Rect  SceneToView(const Rect& rt);
	virtual Point ViewToScene(const Point& p);
	virtual Size  ViewToScene(const Size& sz);
	virtual Rect  ViewToScene(const Rect& rt);

	virtual Array<Point>  SceneToView(const Array<Point>& p);
	virtual Vector<Point> SceneToView(const Vector<Point>& p);

	virtual Array<Point>  ViewToScene(const Array<Point>& p);
	virtual Vector<Point> ViewToScene(const Vector<Point>& p);

	virtual Rect  CurrentViewRect();
	virtual Rect  CurrentSceneRect();

protected:
	Point PointZoomIn();
	Point PointZoomOut();

	// events
public:
	virtual void LeftDouble (Point p, dword keyflags) { ProcessLeftDown(p, keyflags); }
	virtual void LeftDown   (Point p, dword keyflags) { ProcessLeftDown(p, keyflags); }
	virtual void LeftDrag   (Point p, dword keyflags) { ProcessLeftDrag(p, keyflags); }
	virtual void LeftUp     (Point p, dword keyflags) { ProcessLeftUp  (p, keyflags); }

	virtual void RightDouble(Point p, dword keyflags) { ProcessRightDown(p, keyflags); }
	virtual void RightDrag  (Point p, dword keyflags) { ProcessRightDrag(p, keyflags); }
	virtual void RightUp    (Point p, dword keyflags) { ProcessRightUp  (p, keyflags); }

	virtual void MouseMove  (Point p, dword keyflags) { ProcessMouseMove(p, keyflags); }

	virtual void MouseEnter() { ProcessMouseEnter(); Subpixel(); Refresh(); }
	virtual void MouseLeave() { ProcessMouseLeave(); Subpixel(); Refresh(); }

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

public:
	void SetLevelName(const char* name) { _levelName = name; }
	void SetLevelDesc(const char* desc) { _levelDesc = desc; }

	String GetLevelName() const { return _levelName; }
	String GetLevelDesc() const { return _levelDesc; }

	Point GetMousePos() const { return _editModePos; }
	bool  IsEditMode() const { return _editMode; }

	int GetEditPointIndex(const Point& p) const
	{
		Rect r(Point(p.x - 8, p.y - 8), Size(16, 16));
		for (int i = _editModePoints.GetCount() - 1; i >= 0; --i)
		{
			if (Rect(Point(_editModePoints[i].x - 8, _editModePoints[i].y - 8), Size(16, 16))
				.Intersects(r))
					return i;
		}

		return -1;
	}

	void SetEditPoint(int index, const Point& p)
	{
		if (index < 0 || index >= _editModePoints.GetCount())
			return;
		_editModePoints[index] = p;
		WhenChangePoint();
	}

	void AddEditPoint(const Point& p)
	{
		if (p.x < 0 || p.y < 0)
			return;

		Rect r(Point(p.x - 8, p.y - 8), Size(16, 16));
		bool found = false;

		for (int i = _editModePoints.GetCount() - 1; i >= 0; --i)
		{
			Rect er(Point(_editModePoints[i].x - 8, _editModePoints[i].y - 8), Size(16, 16));
			if ( er.Intersects(r))
			{
				_editModePoints.Remove(i);
				found = true;
			}
		}

		if (!found)
			_editModePoints << p;
		else
			_currPoint = -1;

		Refresh();
	}

	Callback WhenChangePoint;
	void ClearEditPoints() { _editModePoints.Clear(); }
	Array<Point>& GetEditPoints() { return _editModePoints; }
	const Array<Point>& GetEditPoints() const { return _editModePoints; }

private:
	String _levelName;
	String _levelDesc;

	int _currPoint;
	bool _editMode;
	Point _editModePos;
	Array<Point> _editModePoints;

	bool _highQuality;
	int _zoomLevel;
	Point _pagePos;
	Vector<MapZoomLevel> _zoomLevels;
	VectorMap<int, Ptr<IMapRender> > _layers;
};

#endif
