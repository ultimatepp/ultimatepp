#ifndef MAP_EDITOR__MAP_EDITOR_H
#define MAP_EDITOR__MAP_EDITOR_H

#include <GridCtrl/GridCtrl.h>
using namespace Upp;

#include <Map/Map.h>

#define LAYOUTFILE <MapEditor/MapEditor.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS MapEditorImg
#define IMAGEFILE <MapEditor/MapEditor.iml>
#include <Draw/iml_header.h>

class MapEditor : public WithMapEditorLayout<TopWindow>
{
public:
	typedef MapEditor CLASSNAME;
	MapEditor();

	void OnClose();
	void OnNewMap();
	void OnSaveMap();
	void OnLoadMap();
	void OnViewMap();
	void OnEditLevel();
	void OnMapProperties();

	void OnAddLevel();
	void OnMoveLevel(int s, int e);
	void OnChangeLevel();
	void OnRemoveLevel();

	void UpdateLevelList();
	void UpdateEditorCtrls();

protected:
	void RealizeDirectories();
	void CalculateAllPrompt();
	void OnLevelMapCalc(Level& level, int number);
	void OnLevelMapList(VectorMapEx<String, Value>& mapList);

private:
	Map _map;
};

#endif
