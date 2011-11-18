#include "LevelEditor.h"

LevelEditor::LevelEditor(Map& map, Level& level, int pos)
{
	CtrlLayout(*this, "Редактор этажа");
	MaximizeBox().Sizeable();

	_item  = -1;
	_pos   = pos;
	_map   = &map;
	_level = &level;

	EditView.HideLevelsCtrl();
	EditView.HideNavButtons();

	LayerList.WhenToolBar = THISBACK(CreateLayerToolBar);
	LayerList.WhenChangeRow = THISBACK(UpdateItemList);
	LayerList.AddColumn(t_("#")).Min(HorzLayoutZoom(40)).Max(VertLayoutZoom(40))
		.HeaderAlignCenter().AlignCenter();
	LayerList.SetToolBar();

	LayerList.AddColumn(t_("Layer Name"))
		.HeaderAlignCenter().AlignCenter();

	ItemList.WhenToolBar = THISBACK(CreateItemToolBar);
	ItemList.WhenChangeRow = THISBACK(OnItemSelect);
	ItemList.WhenLeftDouble = THISBACK(OnItemProperties);
	ItemList.AddColumn(t_("#")).Min(HorzLayoutZoom(40)).Max(VertLayoutZoom(40))
		.HeaderAlignCenter().AlignCenter();
	ItemList.SetToolBar();

	ItemList.AddColumn(t_("Item Name"))
		.HeaderAlignCenter().AlignCenter();

	ItemTypeList.Add(t_("Rooms"));
	ItemTypeList.SetIndex(0);

	LoadMap();
	UpdateLayerList();
}

void LevelEditor::UpdateLayerList()
{
	if (!_level)
		return;

	int row = LayerList.GetCurrentRow();

	LayerList.Clear();

	for (int i = 0; i < _level->GetLayers().GetCount(); ++i)
	{
		LayerList.Add(i, _level->GetLayers()[i].GetName());
	}

	if (row >= 0 && row < LayerList.GetRowCount())
		LayerList.SetCursor(row);

	if (row < 0 && LayerList.GetRowCount())
		LayerList.SetCursor(0);

	MapLevel* level = EditView.GetCurrentLevel();
	if (!level) return;
	else level->ClearEditPoints();

	_item = -1;
	UpdateItemList();
}

void LevelEditor::UpdateItemList()
{
	ItemList.Clear();

	int row = LayerList.GetCurrentRow();
	if (row < 0) return;

	Layer& lay = _level->GetLayers()[row];

	for (int i = 0; i < lay.GetRooms().GetCount(); ++i)
	{
		ItemList.Add(i, lay.GetRooms()[i].GetName());
	}

	if (ItemList.GetRowCount())
		ItemList.SetCursor(0);

	OnItemSelect();
	UpdateMap(ItemList.GetRowCount() > 0);
}

void LevelEditor::LoadMap()
{
	Map map;

	for (int i = 0; i < _pos; ++i)
		map.GetLevels().Add(Level());

	map.GetLevels().Add(Level(*_level));
	map.SetName(_map->GetName());

	EditView.LoadMap(map, true);
	EditView.SetCurrentLevel(_pos);
	EditView.GetCurrentLevel()->WhenChangePoint = THISBACK(OnChangePoint);
	EditView.HighQuality(false);
}

void LevelEditor::UpdateMap(bool editMode)
{
	EditView.UpdateLevel(*_level, _pos, editMode);
}

int LevelEditor::Execute()
{
	int res = TopWindow::Execute();

	int layer = LayerList.GetCurrentRow();
	if (layer < 0) return res;

	Layer& lay = _level->GetLayers()[layer];

	int row = ItemList.GetCurrentRow();
	if (row < 0) return res;

	MapLevel* level = EditView.GetCurrentLevel();
	if (!level) return res;

	if (_item != -1)
	{
		if (_item >= 0 && _item < lay.GetRooms().GetCount())
		{
			Room& room = lay.GetRooms()[_item];
			room.GetVertices().Clear();
			for (int i = 0; i < level->GetEditPoints().GetCount(); ++i)
			{
				room.GetVertices() << level->GetEditPoints()[i];
			}
		}
		_item = -1;
	}

	return res;
}
