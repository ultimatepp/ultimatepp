#include <CtrlLib/CtrlLib.h>
#include "GridCtrl.h"

NAMESPACE_UPP

void GridCtrl::GSort(int scol)
{
	int col;

	int cnt = sortOrder.GetCount();

	if(cnt == 0)
		return;

	if(scol < 0)
		scol = cnt - 1;

	col = sortOrder[scol];

	//int order = 1;
	int order = hitems[col].sortmode;

	/* dla trybu multisort nie mozna w ten sposob
	   sortowac dla pierwszej kolumny */

	if(cnt > 1 && scol != 0)
	{
		int match = 1;
		int is = fixed_rows;

		for(int i = fixed_rows + 1; i < total_rows; i++)
		{
			int n = vitems[i].id;
			int m = vitems[is].id;

			bool found = true;
			for(int j = 0; j < scol; j++)
			{
				int k = sortOrder[j];
				if(items[n][k].val != items[m][k].val)
				{
					found = false;
					break;
				}
			}

			if(found)
			{
				match++;
				continue;
			}
			else
			{
				if(match > 1)
					GSort(col, order, is, match);

				match = 1;
				is = i;
			}
		}
		if(match > 1)
			GSort(col, order, is, match);
	}
	else
		GSort(col, order, fixed_rows);

}

void GridCtrl::GSort(int col, int order, int from, int count)
{
	if(count == 0)
		return;

	ItemRect::sortCol = col;
	ItemRect::sortMode = (order != 0);

	VItems::Iterator its, ite;

	its = vitems.Begin() + from;

	if(count < 0)
		ite = vitems.End();
	else
		ite = its + count;

	if(order < 2)
		Sort(its, ite, StdLess<ItemRect>());
	else
		Sort(its, ite, StdGreater<ItemRect>());
}


void GridCtrl::Multisort()
{
	GSort(-1, 0, fixed_rows);
	for(int i = 0; i < sortOrder.GetCount(); i++)
		GSort(i);
}

int GridCtrl::InMultisort(int col)
{
	for(int i = 0; i < sortOrder.GetCount(); i++)
		if(col == sortOrder[i])
			return i;

	return -1;
}

bool GridCtrl::ClearMultisort()
{
	if(sortOrder.IsEmpty())
		return false;

/*	for(int i = 0; i < sortOrder.GetCount(); i++)
	{
		int idx = sortOrder[i];
		if(hitems[idx].sortMode > 0)
			sorted = true;
		hitems[idx].sortmode = 0;
		hitems[idx].sortcol = 0;
	}
*/

	return ClearSorted();
}

bool GridCtrl::ClearSorted()
{
	bool sorted = false;

	for(int i = 0; i < total_cols; i++)
	{
		if(hitems[i].sortMode > 0)
			sorted = true;
		hitems[i].sortmode = 0;
		hitems[i].sortcol = 0;
	}

	return sorted;
}

bool GridCtrl::IsSorted()
{
	/*
	bool sorted = false;
	for(int i = 0; i < total_cols; i++)
		if(hitems[i].sortmode > 0)
		{
			sorted = true;
			break;
		}

	return sortOrder.GetCount() > 0 || sorted;
	*/
	return sortOrder.GetCount() > 0 || sortCol >= 0;
}

END_UPP_NAMESPACE
