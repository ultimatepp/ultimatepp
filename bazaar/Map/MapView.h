#ifndef MAP_VIEW__H
#define MAP_VIEW__H

#include "MapLevel.h"
#include "MapSerialize.h"

#define LAYOUTFILE <Map/MapView.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS MapViewImg
#define IMAGEFILE <Map/MapView.iml>
#include <Draw/iml_header.h>

class MapView : public WithMapViewLayout<TopWindow>
{
	typedef MapView CLASSNAME;

public:
	MapView();
	virtual ~MapView() {}
	virtual void Layout();

	virtual void LeftDouble (Point p, dword keyflags);
	virtual void LeftDown   (Point p, dword keyflags);
	virtual void LeftDrag   (Point p, dword keyflags);
	virtual void LeftUp     (Point p, dword keyflags);

	virtual void RightDouble(Point p, dword keyflags);
	virtual void RightDown  (Point p, dword keyflags);
	virtual void RightDrag  (Point p, dword keyflags);
	virtual void RightUp    (Point p, dword keyflags);

	virtual void MouseMove  (Point p, dword keyflags);

	virtual void MouseEnter();
	virtual void MouseLeave();

	void OnLevelUp();
	void OnLevelDown();

	void OnMoveUp();
	void OnMoveDown();
	void OnMoveLeft();
	void OnMoveRight();

	void OnZoomIn();
	void OnZoomOut();
	void OnZoomSlide();
	void UpdateButtons();

	void DoSearch(const String& data);
	void OnSearch();
	void OnSearchClear();

	void ShowNavButtons(bool flag = true);
	void HideNavButtons() { ShowNavButtons(false); }

	void ShowZoomSlider(bool flag = true);
	void HideZoomSlider() { ShowZoomSlider(false); }

	void ShowLevelsCtrl(bool flag = true);
	void HideLevelsCtrl() { ShowLevelsCtrl(false); }

	void ShowSearch(bool flag = true);
	void HideSearch() { ShowSearch(false); }

	bool UpdateLevel(Level& level, int pos, bool editMode = false);
	bool LoadMap(Map& map, bool editMode = false);
	bool LoadMap(const char* fp, bool editMode = false);

	MapView& HighQuality(bool flag = true)
	{
		for (int i = 0; i < GetLevelCount(); ++i)
			GetLevel(i)->HighQuality(flag);
		return *this;
	}

	MapLevel* GetCurrentLevel() { return GetLevel(_level); }
	MapLevel* GetLevel(int i)
	{
		return (i < 0 || i >= GetLevelCount())
			? NULL
			: &_levels[i];
	}

	int  GetLevelCount() const { return _levels.GetCount(); }
	void SetCurrentLevel(int level);

private:
	int   _level;
	bool  _drag;
	bool  _move;
	Point _mousePos;

	Array<MapLevel> _levels;
};

#endif
