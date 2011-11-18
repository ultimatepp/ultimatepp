#include "MapView.h"

#define IMAGECLASS MapViewImg
#define IMAGEFILE <Map/MapView.iml>
#include <Draw/iml_source.h>

#define SetButtonStyle(but,im1,im2) 							\
	static Button::Style but##_style = Button::StyleNormal();	\
	but##_style.look[0] = MapViewImg::im1();					\
	but##_style.look[1] = MapViewImg::im1();					\
	but##_style.look[2] = MapViewImg::im2();					\
	but##_style.look[3] = MapViewImg::im2();					\
	but.SetStyle(but##_style);

MapView::MapView()
{
	CtrlLayout(*this, t_("MapCtrl View"));
	MinimizeBox().MinimizeBox().Sizeable();

	// init values
	_mousePos = Point(0, 0);
	_drag     = false;
	_move     = false;
	_level    = -1;

	HideZoomSlider();
	HideSearch();

	// init layout
	ZoomInBtn   .NoWantFocus();
	ZoomOutBtn  .NoWantFocus();

	MoveUpBtn   .NoWantFocus();
	MoveDownBtn .NoWantFocus();
	MoveLeftBtn .NoWantFocus();
	MoveRightBtn.NoWantFocus();

	LevelUpBtn  .NoWantFocus();
	LevelDownBtn.NoWantFocus();

	MoveUpBtn   .WhenPush = MoveUpBtn   .WhenRepeat = THISBACK(OnMoveUp);
	MoveDownBtn .WhenPush = MoveDownBtn .WhenRepeat = THISBACK(OnMoveDown);
	MoveLeftBtn .WhenPush = MoveLeftBtn .WhenRepeat = THISBACK(OnMoveLeft);
	MoveRightBtn.WhenPush = MoveRightBtn.WhenRepeat = THISBACK(OnMoveRight);

	ZoomInBtn .WhenPush = ZoomInBtn .WhenRepeat = THISBACK(OnZoomIn);
	ZoomOutBtn.WhenPush = ZoomOutBtn.WhenRepeat = THISBACK(OnZoomOut);
	ZoomSlider.WhenAction = THISBACK(OnZoomSlide);

	LevelUpBtn  .WhenPush = THISBACK(OnLevelUp);
	LevelDownBtn.WhenPush = THISBACK(OnLevelDown);

	Search.WhenAction = THISBACK(OnSearch);
	Search.WhenEnter  = THISBACK(OnSearch);
	ClearSearchBtn  <<= THISBACK(OnSearchClear);
	
	SetButtonStyle(MoveUpBtn,    map_button_up_1,    map_button_up_2);
	SetButtonStyle(MoveDownBtn,  map_button_down_1,  map_button_down_2);	
	SetButtonStyle(MoveLeftBtn,  map_button_left_1,  map_button_left_2);	
	SetButtonStyle(MoveRightBtn, map_button_right_1, map_button_right_2);

	SetButtonStyle(ZoomInBtn,    map_button_zoom_in_1,  map_button_zoom_in_2);
	SetButtonStyle(ZoomOutBtn,   map_button_zoom_out_1, map_button_zoom_out_2);

	SetButtonStyle(LevelUpBtn,   map_button_zoom_in_1,  map_button_zoom_in_2);
	SetButtonStyle(LevelDownBtn, map_button_zoom_out_1, map_button_zoom_out_2);
}

void MapView::OnLevelUp()
{
	_level++;
	SetCurrentLevel(_level);
	UpdateButtons();
}

void MapView::OnLevelDown()
{
	_level--;
	SetCurrentLevel(_level);
	UpdateButtons();
}

void MapView::OnMoveUp()
{
	if (GetCurrentLevel())
		GetCurrentLevel()->MoveBy(Point(0, -100));
}

void MapView::OnMoveDown()
{
	if (GetCurrentLevel())
		GetCurrentLevel()->MoveBy(Point(0,  100));
}

void MapView::OnMoveLeft()
{
	if (GetCurrentLevel())
		GetCurrentLevel()->MoveBy(Point(-100, 0));
}

void MapView::OnMoveRight()
{
	if (GetCurrentLevel())
		GetCurrentLevel()->MoveBy(Point( 100, 0));
}

void MapView::OnZoomIn()
{
	MapLevel* level = GetCurrentLevel();
	if (!level) return;

	level->ZoomIn();
	UpdateButtons();
	ZoomSlider <<= level->GetCurrentZoom();
}

void MapView::OnZoomOut()
{
	MapLevel* level = GetCurrentLevel();
	if (!level) return;

	level->ZoomOut();
	UpdateButtons();
	ZoomSlider <<= level->GetCurrentZoom();
}

void MapView::OnZoomSlide()
{
	MapLevel* level = GetCurrentLevel();
	if (!level) return;

	int zoom = ~ZoomSlider;
	level->ZoomTo(zoom);
	UpdateButtons();
}

void MapView::UpdateButtons()
{
	MapLevel* level = GetCurrentLevel();
	if (!level)
	{
		ZoomInBtn   .Disable();
		ZoomOutBtn  .Disable();
		LevelUpBtn  .Disable();
		LevelDownBtn.Disable();

		MoveUpBtn   .Disable();
		MoveDownBtn .Disable();
		MoveLeftBtn .Disable();
		MoveRightBtn.Disable();
		return;
	}

	int total = level->GetZoomCount();
	int zoom  = level->GetCurrentZoom();

	ZoomInBtn .Enable(zoom >= 0 && zoom < total - 1);
	ZoomOutBtn.Enable(zoom >= 1 && zoom < total);

	LevelUpBtn  .Enable(_level < _levels.GetCount() - 1);
	LevelDownBtn.Enable(_level > 0);
}

void MapView::ShowZoomSlider(bool flag)
{
	ZoomSlider.Show(flag);
}

void MapView::ShowNavButtons(bool flag)
{
	MoveUpBtn   .Show(flag);
	MoveDownBtn .Show(flag);
	MoveLeftBtn .Show(flag);
	MoveRightBtn.Show(flag);
}

void MapView::ShowLevelsCtrl(bool flag)
{
	LevelName   .Show(flag);
	LevelUpBtn  .Show(flag);
	LevelDownBtn.Show(flag);
}

void MapView::ShowSearch(bool flag)
{
	Search        .Show(flag);
	ClearSearchBtn.Show(flag);
}

void MapView::SetCurrentLevel(int level)
{
	MapLevel* p = GetLevel(level);
	if (!p)
	{
		_level = -1;
		return;
	}

	_level = level;

	for (int i = 0; i < _levels.GetCount(); ++i)
		_levels[i].Hide();

	p->Show();
	ZoomSlider.Range(p->GetZoomCount() - 1).Step(1);
	ZoomSlider <<= p->GetCurrentZoom();
	LevelName.SetLabel(p->GetLevelName());
	UpdateButtons();
}

bool MapView::LoadMap(Map& map, bool editMode)
{
	for (int i = 0; i < _levels.GetCount(); ++i)
		RemoveChild(&_levels[i]);
	_levels.Clear();

	for (int i = 0; i < map.GetLevels().GetCount(); ++i)
	{
		MapLevel& level = _levels.Add();
		AddChildBefore(&level.SizePos(), GetFirstChild());
		level.IgnoreMouse();
		level.Hide();

		String fp = AppendFileName(
			AppendFileName( GetFileDirectory(GetExeFilePath()), "Mipmaps"),
			NFormat("%s-%d.xml", map.GetName(), i + 1)
		);

		level.SetLevelName(map.GetLevels()[i].GetName());
		level.SetLevelDesc(map.GetLevels()[i].GetDesc());
		level.LoadMap(fp);
		level.EditMode(editMode);

		for (int j = 0; j < map.GetLevels()[i].GetLayers().GetCount(); ++j)
		{
			Ptr<IMapRender> layer = level.AddItem<IMapRender>();

/*			Array<Room>& rooms = map.GetLevels()[i].GetLayers()[j].GetRooms();
			for (int r = 0; r < rooms.GetCount(); ++r)
			{
				Ptr<PolygonItem> room = layer->AddItem<PolygonItem>();
				for (int v = 0; v < rooms[r].GetVertices().GetCount(); ++v)
					room->AddVertice(rooms[r].GetVertices()[v]);
				room->SetRoom(rooms[r]);
				if (editMode)
					room->StateOn(STATE_EDIT);
			}*/

			for (int rx = 0; rx < 10; ++rx)
				for (int ry = 0; ry < 10; ++ry)
			{
				Ptr<PolygonItem> room = layer->AddItem<PolygonItem>();
				room->AddVertice( 0 * rx,  0 * ry);
				room->AddVertice(40 * rx,  0 * ry);
				room->AddVertice(40 * rx, 40 * ry);
				room->AddVertice( 0 * rx, 40 * ry);
				room->SetRoom(rooms[r]);
				if (editMode)
					room->StateOn(STATE_EDIT);
			}
		}
	}

	SetCurrentLevel(_levels.GetCount() ? 0 : -1);
	return true;
}

bool MapView::LoadMap(const char* fp, bool editMode)
{
	Map map;
	if (!LoadFromXMLFile(map, fp))
		return false;

	return LoadMap(map, editMode);
}

bool MapView::UpdateLevel(Level& level, int pos, bool editMode)
{
	if (pos < 0 || pos >= _levels.GetCount())
		return false;

	MapLevel& mapLevel = _levels[pos];
	mapLevel.EditMode(editMode);

	if (mapLevel.GetItems().GetCount() != level.GetLayers().GetCount())
		mapLevel.Clear();

	Vector<Ptr<IMapItem> >& mapLayers = mapLevel.GetItems();

	for (int j = 0; j < level.GetLayers().GetCount(); ++j)
	{
		IMapRender* mapLayer =
			(j < mapLayers.GetCount())
				? dynamic_cast<IMapRender*>(~mapLayers[j])
				: ~mapLevel.AddItem<IMapRender>();

		Vector<Ptr<IMapItem> >& mapRooms = mapLayer->GetItems();
		Array<Room>& rooms = level.GetLayers()[j].GetRooms();
		mapLayer->Clear();

		for (int r = 0; r < rooms.GetCount(); ++r)
		{
			Ptr<PolygonItem> mapRoom =
				mapLayer->AddItem<PolygonItem>();

			mapRoom->GetVertices().Clear();
			for (int v = 0; v < rooms[r].GetVertices().GetCount(); ++v)
				mapRoom->AddVertice(rooms[r].GetVertices()[v]);
			mapRoom->SetRoom(rooms[r]);
			if (editMode)
				mapRoom->StateOn(STATE_EDIT);
		}
	}

	Refresh();
}
