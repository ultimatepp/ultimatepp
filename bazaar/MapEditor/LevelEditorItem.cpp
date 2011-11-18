#include "LevelEditor.h"

void LevelEditor::OnItemAdd()
{
	int layer = LayerList.GetCurrentRow();
	if (layer < 0) return;

	Layer& lay = _level->GetLayers()[layer];

	Room room;
	room.SetName(NFormat(t_("Room #%d"), lay.GetRooms().GetCount() + 1));

	if (room.OpenProperties() != IDOK)
		return;

	lay.GetRooms().Add(room);

	UpdateItemList();
}

void LevelEditor::OnItemEdit()
{
	int layer = LayerList.GetCurrentRow();
	if (layer < 0) return;

	Layer& lay = _level->GetLayers()[layer];

	int row = ItemList.GetCurrentRow();
	if (row < 0) return;

	lay.GetRooms()[row].OpenProperties();

	UpdateItemList();
}

void LevelEditor::OnItemRemove()
{
	int layer = LayerList.GetCurrentRow();
	if (layer < 0) return;

	Layer& lay = _level->GetLayers()[layer];

	int row = ItemList.GetCurrentRow();
	if (row < 0) return;

	if (!PromptYesNo(t_("Remove the selected item from level?")))
		return;

	lay.GetRooms().Remove(row);
	UpdateItemList();
}

void LevelEditor::OnItemSelect()
{
	int layer = LayerList.GetCurrentRow();
	if (layer < 0) return;

	Layer& lay = _level->GetLayers()[layer];

	int row = ItemList.GetCurrentRow();
	if (row < 0) return;

	MapLevel* level = EditView.GetCurrentLevel();
	if (!level) return;

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

	level->ClearEditPoints();

	Room& room = lay.GetRooms()[row];
	for (int i = 0; i < room.GetVertices().GetCount(); ++i)
	{
		level->AddEditPoint(room.GetVertices()[i]);
	}

	_item = row;
	Refresh();
}

void LevelEditor::OnItemProperties()
{
	int layer = LayerList.GetCurrentRow();
	if (layer < 0) return;

	Layer& lay = _level->GetLayers()[layer];

	int row = ItemList.GetCurrentRow();
	if (row < 0) return;

	lay.GetRooms()[row].OpenProperties();
	UpdateItemList();
}

void LevelEditor::OnChangePoint()
{
	OnItemSelect();
	UpdateMap(true);
}
