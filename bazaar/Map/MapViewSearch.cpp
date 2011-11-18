#include "MapView.h"

void MapView::DoSearch(const String& data)
{
	if (GetCurrentLevel())
		GetCurrentLevel()->PerformStates(PERFORM_SEARCH, data);
	Refresh();
}

void MapView::OnSearch()
{
	if (GetCurrentLevel())
		GetCurrentLevel()->StatesOff(STATE_SELECTED);
	DoSearch((~Search).ToString());
}

void MapView::OnSearchClear()
{
	MapLevel* level = GetCurrentLevel();
	if (!level) return;

	level->StatesOff(STATE_SELECTED);
	level->StatesOff(STATE_FOUND);
	Search.Clear();

	Refresh();
}
