#include "MapEditor.h"

void MapEditor::OnNewMap()
{
	int r = PromptYesNoCancel(t_("Save current map before creating new map?"));

	if (r == IDYES)
		OnSaveMap();

	if (r == IDCANCEL)
		return;

	_map.Clear();

	UpdateLevelList();
	UpdateEditorCtrls();
}

void MapEditor::OnSaveMap()
{
	CalculateAllPrompt();

	if (!StoreAsXMLFile(_map, _map.GetName(),
		AppendFileName(
			AppendFileName(GetFileDirectory(GetExeFilePath()), "Mipmaps"),
			_map.GetName() + ".map"
		)))
	{
		Exclamation(t_("Error while saving map to file!"));
	}

	UpdateEditorCtrls();
}

void MapEditor::OnLoadMap()
{
	WithMapLoadLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, t_("Load Map..."));

	String dir = AppendFileName( GetFileDirectory(GetExeFilePath()), "Mipmaps");
	Vector<String> files = GetDirectoryFiles(dir, "*.map");
	Sort(files);

	if (files.GetCount() <= 0)
	{
		Exclamation(t_("No any file to load!"));
		return;
	}

	for (int i = 0; i < files.GetCount(); ++i)
		dlg.MapList.Add(files[i], files[i]);
	dlg.MapList.SetIndex(0);

	if (dlg.Execute() != IDOK)
		return;

	String fp = AppendFileName(dir, (~dlg.MapList).ToString());

	if (!FileExists(fp))
	{
		Exclamation(NFormat(t_("File not found: %s"), fp));
		return;
	}

	if (!LoadFromXMLFile(_map, fp))
	{
		Exclamation(NFormat(t_("Error while loading map from file: %s"), fp));
		return;
	}

	UpdateLevelList();
	UpdateEditorCtrls();
}

void MapEditor::OnViewMap()
{
	WithPreviewLayout<TopWindow> dlg;
	CtrlLayout(dlg, t_("Map Preview"));
	dlg.Sizeable().MaximizeBox();
	dlg.View.HighQuality(true);

	if (!dlg.View.LoadMap(_map))
	{
		Exclamation(t_("Error while loading preview!"));
		return;
	}

	dlg.Execute();
}

void MapEditor::OnMapProperties()
{
	_map.OpenProperties();
	UpdateEditorCtrls();
}
