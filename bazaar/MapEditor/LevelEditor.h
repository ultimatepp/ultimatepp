#ifndef MAP_EDITOR__LEVEL_EDITOR_H
#define MAP_EDITOR__LEVEL_EDITOR_H

#include "MapEditor.h"

class LevelEditor : public WithLevelEditLayout<TopWindow>
{
	typedef LevelEditor CLASSNAME;

public:
	LevelEditor(Map& map, Level& level, int pos);

	virtual int Execute();

protected:
	void OnLayerAdd();
	void OnLayerEdit();
	void OnLayerRemove();
	void OnChangePoint();

	void OnItemAdd();
	void OnItemEdit();
	void OnItemRemove();
	void OnItemSelect();
	void OnItemProperties();

	void LoadMap();
	void UpdateMap(bool editMode);
	void UpdateItemList();
	void UpdateLayerList();

	void CreateItemToolBar(Bar& bar);
	void CreateLayerToolBar(Bar& bar);

	void OpenItemProperties();

private:
	int _item;
	Level* _level;
	Map* _map;
	int _pos;
};

#endif
