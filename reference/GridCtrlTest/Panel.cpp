#include "Panel.h"

Panel::Panel()
{
	CtrlLayout(*this);
	CtrlLayout(editopt);
	CtrlLayout(lookopt);
	CtrlLayout(colsopt);
	CtrlLayout(numsopt);
	
	opts.Add(lookopt, "Look");
	opts.Add(editopt, "Edit");
	opts.Add(colsopt, "Colors");
	opts.Add(numsopt, "Numbers");
	
	lookopt.indicator         <<= THISBACK1(LookOptions, 0);
	lookopt.hgrid             <<= THISBACK1(LookOptions, 1);
	lookopt.vgrid             <<= THISBACK1(LookOptions, 2);
	lookopt.resizing_cols     <<= THISBACK1(LookOptions, 3);
	lookopt.resizing_rows     <<= THISBACK1(LookOptions, 4);
	lookopt.live_cursor       <<= THISBACK1(LookOptions, 5);
	lookopt.resize_paint_mode <<= THISBACK1(LookOptions, 6);
	lookopt.resize_col_mode   <<= THISBACK1(LookOptions, 7);
	lookopt.resize_row_mode   <<= THISBACK1(LookOptions, 8);
	lookopt.moving_cols       <<= THISBACK1(LookOptions, 9);
	lookopt.moving_rows       <<= THISBACK1(LookOptions, 10);
	lookopt.full_col_resizing <<= THISBACK1(LookOptions, 11);
	lookopt.full_row_resizing <<= THISBACK1(LookOptions, 12);
	lookopt.repaint           <<= THISBACK1(LookOptions, 13);		
	lookopt.disable           <<= THISBACK1(LookOptions, 14);
	lookopt.chameleon         <<= THISBACK1(LookOptions, 15);
	lookopt.draw_focus        <<= THISBACK1(LookOptions, 16);

	editopt.multiline         <<= THISBACK1(EditOptions, 0);
	editopt.select_row        <<= THISBACK1(EditOptions, 1);
	editopt.sorting           <<= THISBACK1(EditOptions, 2);
	editopt.multisort         <<= THISBACK1(EditOptions, 3);
	editopt.tab_changes_row   <<= THISBACK1(EditOptions, 4);
	editopt.tab_adds_row      <<= THISBACK1(EditOptions, 5);
	editopt.edit_mode         <<= THISBACK1(EditOptions, 6);
	editopt.one_click_edit    <<= THISBACK1(EditOptions, 7);
	editopt.enter_like_tab    <<= THISBACK1(EditOptions, 8);
		
	colsopt.theme.Add(-1, "Flat");
	colsopt.theme.Add(0, "Cairo");		
	colsopt.theme.Add(1, "Gray");
	colsopt.theme.Add(2, "Plastik");		
	colsopt.theme.Add(3, "Lipstik");		
	colsopt.theme.Add(4, "Alloy");		
	colsopt.theme.Add(5, "Unknown 01");
	colsopt.theme.Add(6, "Default");

	lookopt.resize_paint_mode.Add(0, "Line");
	lookopt.resize_paint_mode.Add(1, "Line + Fixed");
	lookopt.resize_paint_mode.Add(2, "Fixed + NonFixed");
	
	lookopt.resize_col_mode.Add(0, "Absolute");
	lookopt.resize_col_mode.Add(1, "Proportional");

	lookopt.resize_row_mode.Add(0, "Absolute");
	lookopt.resize_row_mode.Add(1, "Proportional");

	colsopt.theme             <<= THISBACK1(ColorsOptions, 0);
	colsopt.odd_color         <<= THISBACK1(ColorsOptions, 1);
	colsopt.even_color        <<= THISBACK1(ColorsOptions, 2);
	colsopt.odd_even_mode     <<= THISBACK1(ColorsOptions, 3);
	colsopt.grid_color        <<= THISBACK1(ColorsOptions, 4);
	colsopt.focus_color       <<= THISBACK1(ColorsOptions, 5);
	colsopt.live_color        <<= THISBACK1(ColorsOptions, 6);

	colsopt.odd_color.NoRampWheel();
	colsopt.even_color.NoRampWheel();
	colsopt.grid_color.NoRampWheel().NotNull();
	colsopt.focus_color.NoRampWheel();
	colsopt.live_color.NoRampWheel();
	
	numsopt.total_cols        <<= THISBACK1(Actions, 0);
	numsopt.total_rows        <<= THISBACK1(Actions, 1);
	numsopt.fixed_cols        <<= THISBACK1(Actions, 2);
	numsopt.fixed_rows        <<= THISBACK1(Actions, 3);
	
	numsopt.total_cols.Disable();
	
	clear_grid                <<= THISBACK1(Actions, 4);
	reset_grid                <<= THISBACK1(Actions, 5);

	#ifndef flagDEBUG
	debug.Hide();
	level.Hide();
	#else
	clear                     <<= THISBACK1(Actions, 400);
	debug                     <<= THISBACK1(Actions, 401);
	level                     <<= THISBACK1(Actions, 402);
	#endif
		
	level.Add(0, "All")
	     .Add(1, "1")
	     .Add(2, "2")
	     .Add(3, "3");
	     
	level <<= 0;
	
	dlog = &log0;
}

void Panel::Init(GridCtrl& g)
{
	grid = &g;
	
	numsopt.total_cols        <<= grid->total_cols;
	numsopt.total_rows        <<= grid->total_rows;
	numsopt.fixed_cols        <<= grid->fixed_cols;
	numsopt.fixed_rows        <<= grid->fixed_rows;
		
	lookopt.indicator         <<= grid->indicator;
	lookopt.hgrid             <<= grid->horz_grid;
	lookopt.vgrid             <<= grid->vert_grid;
	lookopt.resizing_cols     <<= grid->resizing_cols;
	lookopt.resizing_rows     <<= grid->resizing_rows;
	lookopt.live_cursor       <<= grid->live_cursor;
	lookopt.moving_cols       <<= grid->moving_cols;
	lookopt.moving_rows       <<= grid->moving_rows;
	lookopt.full_col_resizing <<= grid->full_col_resizing;
	lookopt.full_row_resizing <<= grid->full_row_resizing;
	lookopt.disable           <<= !grid->IsEnabled();
	lookopt.chameleon         <<= grid->chameleon;
	lookopt.draw_focus        <<= grid->draw_focus;
	
	editopt.multiline         <<= grid->multi_select;
	editopt.select_row        <<= grid->select_row;
	editopt.sorting           <<= grid->sorting;
	editopt.multisort         <<= grid->sorting_multicol;
	editopt.tab_changes_row   <<= grid->tab_changes_row;
	editopt.tab_adds_row      <<= grid->tab_adds_row;
	editopt.edit_mode           = grid->edit_mode;
	editopt.one_click_edit    <<= grid->one_click_edit;
	
	lookopt.resize_paint_mode <<= grid->resize_paint_mode;
	lookopt.resize_col_mode   <<= grid->resize_col_mode;
	lookopt.resize_row_mode   <<= grid->resize_row_mode;
	
	colsopt.theme             <<= grid->display->theme;

	colsopt.odd_even_mode     <<= grid->coloring_mode;
	colsopt.odd_color         <<= grid->bg_odd;
	colsopt.even_color        <<= grid->bg_even;
	colsopt.grid_color        <<= grid->fg_grid;
	colsopt.focus_color       <<= grid->bg_focus;
	colsopt.live_color        <<= grid->bg_live;
}

void Panel::LookOptions(int n)
{
	switch(n)
	{
		case 0:	 grid->Indicator(lookopt.indicator);                          break;
		case 1:	 grid->HorzGrid(lookopt.hgrid);                               break;
		case 2:	 grid->VertGrid(lookopt.vgrid);                               break;
		case 3:	 grid->ResizingCols(lookopt.resizing_cols);			          break;
		case 4:	 grid->ResizingRows(lookopt.resizing_rows);			          break;
		case 5:  grid->LiveCursor(lookopt.live_cursor);				          break;
		case 6:	 grid->ResizePaintMode(lookopt.resize_paint_mode.GetIndex()); break;
		case 7:	 grid->ResizeColMode(lookopt.resize_col_mode.GetIndex());     break;
		case 8:  grid->ResizeRowMode(lookopt.resize_row_mode.GetIndex());     break;
		case 9:  grid->MovingCols(lookopt.moving_cols);                       break;
		case 10: grid->MovingRows(lookopt.moving_rows);                       break;
		case 11: grid->FullColResizing(lookopt.full_col_resizing);            break;
		case 12: grid->FullRowResizing(lookopt.full_row_resizing);            break;
		case 13:
			#ifdef flagDEBUG
			Ctrl::ShowRepaint((int) lookopt.repaint ? 20 : 0);
			#endif
			break;
			
		case 14: grid->Enable(!lookopt.disable);	                          break;
		case 15: grid->Chameleon(lookopt.chameleon);                          break;
		case 16: grid->DrawFocus(lookopt.draw_focus);                         break;
	}
	grid->Repaint();
}

void Panel::ColorsOptions(int n)
{
	switch(n)
	{
		case 0: grid->GetDisplay().SetTheme(~colsopt.theme);                  break;
		case 1: grid->OddColor(SColorText, ~colsopt.odd_color);               break;
		case 2: grid->EvenColor(SColorText, ~colsopt.even_color);             break;
		case 3: grid->ColoringMode(colsopt.odd_even_mode);                    break;
		case 4: grid->GridColor(~colsopt.grid_color);                         break;
		case 5: grid->FocusColor(SColorText, ~colsopt.focus_color);           break;
		case 6: grid->LiveColor(SColorText, ~colsopt.live_color);             break;
	}
	grid->Repaint();
}

void Panel::EditOptions(int n)
{
	switch(n)
	{
		case 0:	grid->MultiSelect(editopt.multiline);                         break;
		case 1:	grid->SelectRow(editopt.select_row);                          break;
		case 2:	grid->Sorting(editopt.sorting);                               break;
		case 3:	grid->MultiSorting(editopt.multisort);                        break;
		case 4:	grid->TabChangesRow(editopt.tab_changes_row);                 break;
		case 5:	grid->TabAddsRow(editopt.tab_adds_row);		                  break;
		case 6: grid->EditMode(editopt.edit_mode);                            break;
		case 7: grid->OneClickEdit(editopt.one_click_edit);                   break;
		case 8: grid->EnterLikeTab(editopt.enter_like_tab);                   break;
	}
	grid->Repaint();
}

void Panel::Actions(int n)
{
	switch(n)
	{
		case 0:
		{
			/*
			int cnt = grid->GetColumnCount();
			if(cnt < numsopt.total_cols)
				grid->AddColumn(numsopt.total_cols - cnt);
			else
				grid->RemoveColumn(cnt - numsopt.total_cols);
			*/
			break;
		}
		case 1:
		{
			int cnt = grid->total_rows;
			if(cnt < numsopt.total_rows)
				grid->Append(numsopt.total_rows - cnt);
			else
				grid->RemoveLast(cnt - numsopt.total_rows);
			break;
		}
		case 2: grid->SetFixedCols(numsopt.fixed_cols); break;
		case 3:	grid->SetFixedRows(numsopt.fixed_rows); break;

		case 4:
			grid->Clear();
			grid->RefreshTop();
			break;
		case 5:
			grid->Reset();
			break;
		case 400:
			log0.Clear();
			break;
		case 401:
			grid->Debug(0);
			break;
		case 402:
			dlev = ~level;
			break;
		case 403:
			grid->ReSort();
			break;
		case 666:
			Close();
	}
	grid->Repaint();
}
