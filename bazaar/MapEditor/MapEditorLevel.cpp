#include "MapEditor.h"
#include "LevelEditor.h"

void MapEditor::OnAddLevel()
{
	int row = LevelList.GetRowCount();

	Level level;
	level.SetName(NFormat(t_("Level #%d"), _map.GetLevels().GetCount() + 1));

	level.WhenMapList = THISBACK(OnLevelMapList);
	level.WhenCalculate = THISBACK1(OnLevelMapCalc, row);
	if (level.OpenProperties() != IDOK)
		return;

	_map.GetLevels().Add(level);

	UpdateLevelList();
}

void MapEditor::OnEditLevel()
{
	int row = LevelList.GetCurrentRow();
	if (row < 0) return;

	LevelEditor edit(_map, _map.GetLevels()[row], row);
	edit.Execute();
}

void MapEditor::OnChangeLevel()
{
	int row = LevelList.GetCurrentRow();
	if (row < 0) return;

	_map.GetLevels()[row].WhenCalculate = THISBACK1(OnLevelMapCalc, row);
	_map.GetLevels()[row].WhenMapList = THISBACK(OnLevelMapList);
	_map.GetLevels()[row].OpenProperties();

	UpdateLevelList();
}

void MapEditor::OnRemoveLevel()
{
	int row = LevelList.GetCurrentRow();
	if (row < 0) return;

	if (!PromptYesNo(t_("Remove the selected level from map?")))
		return;

	if (!PromptYesNo(t_("No undo for this action. Are you sure?")))
		return;

	_map.GetLevels().Remove(row);
	UpdateLevelList();
}

void MapEditor::OnMoveLevel(int s, int e)
{
	_map.GetLevels().Swap(s - 1, e - 1);

	if (PromptOKCancel(t_("Recalculate mipmaps for moved levels now?")))
	{
		OnLevelMapCalc( _map.GetLevels()[s - 1], s - 1 );
		OnLevelMapCalc( _map.GetLevels()[e - 1], e - 1 );
	}

	SetTimeCallback(1, THISBACK(UpdateLevelList));
}

void MapEditor::OnLevelMapCalc(Level& level, int number)
{
	if (level.GetMapBG().IsEmpty())
	{
		Exclamation(NFormat(t_("Please, select the image for level: %s"), level.GetName()));
		return;
	}

	String fp = AppendFileName( AppendFileName( GetFileDirectory(GetExeFilePath()), "Maps"),
		level.GetMapBG());

	if (!FileExists(fp))
	{
		Exclamation(NFormat(t_("Image file not exist: %s"), fp));
		return;
	}

	String name = level.GetName();
	double zx   = level.GetZoomDX();
	Size   pz   = level.GetPageSize();
	Size   sz   = level.GetCellSize();

	FileIn in(fp);
	One<StreamRaster> r = StreamRaster::OpenAny(in);
	if (!r)
	{
		Exclamation(NFormat(t_("Error while loading image file: %s"), fp));
		return;
	}

	Image img = r->GetImage();
	Calculate(sz.cx, sz.cy, pz.cx, pz.cy, zx,
		NFormat("%s-%d", _map.GetName(), number + 1), img);
}

void MapEditor::OnLevelMapList(VectorMapEx<String, Value>& mapList)
{
	Vector<String> files = GetDirectoryFiles(
		AppendFileName( GetFileDirectory(GetExeFilePath()), "Maps"));
	Sort(files);
	for (int i = 0; i < files.GetCount(); ++i)
		mapList.Add(files[i], files[i]);
}

void MapEditor::CalculateAllPrompt()
{
	if (PromptYesNo(t_("Recalculate all mipmaps for levels?")))
		for (int i = 0; i < _map.GetLevels().GetCount(); ++i)
			OnLevelMapCalc( _map.GetLevels()[i], i );
}

void MapEditor::UpdateLevelList()
{
	int row = LevelList.GetCurrentRow();

	LevelList.Clear();
	for (int i = 0; i < _map.GetLevels().GetCount(); ++i)
	{
		LevelList.Add(i + 1, _map.GetLevels()[i].GetName());
	}

	if (row >= 0 && row < LevelList.GetRowCount())
		LevelList.SetCursor(row);

	if (row < 0 && LevelList.GetRowCount())
		LevelList.SetCursor(0);

	UpdateEditorCtrls();
}
