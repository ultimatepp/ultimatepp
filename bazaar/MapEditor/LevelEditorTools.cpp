#include "LevelEditor.h"

void LevelEditor::OpenItemProperties()
{
}

void LevelEditor::CreateItemToolBar(Bar& bar)
{
	bar.Add("Add", CtrlImg::Add(), THISBACK(OnItemAdd));
	bar.Add("Remove", CtrlImg::Remove(), THISBACK(OnItemRemove));
	bar.Add("Properties", CtrlImg::smallcheck(), THISBACK(OnItemProperties));
}

void LevelEditor::CreateLayerToolBar(Bar& bar)
{
	bar.Add("Add", CtrlImg::Add(), THISBACK(OnLayerAdd));
	bar.Add("Remove", CtrlImg::Remove(), THISBACK(OnLayerRemove));
}
