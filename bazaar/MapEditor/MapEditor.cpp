#include "MapEditor.h"

#define IMAGECLASS MapEditorImg
#define IMAGEFILE <MapEditor/MapEditor.iml>
#include <Draw/iml_source.h>

MapEditor::MapEditor()
{
	CtrlLayout(*this, t_("Map Editor"));
	RealizeDirectories();
	UpdateEditorCtrls();

	WhenClose  = THISBACK(OnClose);
	PropsBtn <<= THISBACK(OnMapProperties);

	NewMapBtn  <<= THISBACK(OnNewMap);
	SaveMapBtn <<= THISBACK(OnSaveMap);
	LoadMapBtn <<= THISBACK(OnLoadMap);
	ViewMapBtn <<= THISBACK(OnViewMap);
	CalcMapBtn <<= THISBACK(CalculateAllPrompt);

	AddLevelBtn    <<= THISBACK(OnAddLevel);
	EditLevelBtn   <<= THISBACK(OnEditLevel);
	ChangeLevelBtn <<= THISBACK(OnChangeLevel);
	RemoveLevelBtn <<= THISBACK(OnRemoveLevel);

	LevelList.Moving();
	LevelList.AddColumn(t_("#")).Min(HorzLayoutZoom(40)).Max(VertLayoutZoom(40))
		.HeaderAlignCenter().AlignCenter();

	LevelList.AddColumn(t_("Level Name"))
		.HeaderAlignCenter().AlignCenter();

	LevelList.WhenChangeRow = THISBACK(UpdateEditorCtrls);
	LevelList.WhenLeftDouble = THISBACK(OnChangeLevel);
	LevelList.WhenMoveRow = THISBACK(OnMoveLevel);
}

void MapEditor::OnClose()
{
	if (!_map.GetName().IsEmpty() && _map.GetLevels().GetCount() > 0)
	{
		int r = PromptYesNoCancel(t_("Save current map before exit?"));

		if (r == 1)
			OnSaveMap();

		if (r == -1)
			return;
	}

	Break();
}

void MapEditor::UpdateEditorCtrls()
{
	bool flag = _map.GetLevels().GetCount() > 0 && LevelList.GetCurrentRow() >= 0
		&& !_map.GetName().IsEmpty();

	LevelList     .Enable( _map.GetLevels().GetCount());
	SaveMapBtn    .Enable( _map.GetLevels().GetCount());
	AddLevelBtn   .Enable(!_map.GetName().IsEmpty());
	ChangeLevelBtn.Enable(flag);
	RemoveLevelBtn.Enable(flag);

	EditLevelBtn  .Enable(flag);

	CalcMapBtn.Enable(_map.GetLevels().GetCount() > 0 && !_map.GetName().IsEmpty());
	ViewMapBtn.Enable(_map.GetLevels().GetCount() > 0 && !_map.GetName().IsEmpty());
}

void MapEditor::RealizeDirectories()
{
	RealizeDirectory(AppendFileName( GetFileDirectory(GetExeFilePath()), "Mipmaps"));
	RealizeDirectory(AppendFileName( GetFileDirectory(GetExeFilePath()), "Maps"));
}
