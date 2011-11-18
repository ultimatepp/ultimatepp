#include "LevelEditor.h"

void LevelEditor::OnLayerAdd()
{
	int row = LayerList.GetRowCount();

	Layer layer;
	layer.SetName(NFormat(t_("Layer #%d"), _level->GetLayers().GetCount() + 1));

	if (layer.OpenProperties() != IDOK)
		return;

	_level->GetLayers().Add(layer);

	UpdateLayerList();
}

void LevelEditor::OnLayerEdit()
{
	int row = LayerList.GetCurrentRow();
	if (row < 0) return;

	_level->GetLayers()[row].OpenProperties();

	UpdateLayerList();
}

void LevelEditor::OnLayerRemove()
{
	int row = LayerList.GetCurrentRow();
	if (row < 0) return;

	if (!PromptYesNo(t_("Remove the selected layer from level?")))
		return;

	if (!PromptYesNo(t_("No undo for this action. Are you sure?")))
		return;

	_level->GetLayers().Remove(row);
	UpdateLayerList();
}
