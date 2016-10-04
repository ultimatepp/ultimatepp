#include <GridCtrl/GridCtrl.h>

using namespace Upp;

#define  LAYOUTFILE "main.lay"
#include <CtrlCore/lay.h>

#define LOG_PANEL

struct ConvDbId : Convert
{
	virtual Value Format(const Value& q) const
	{
		return ::Format("%.4d", q);
	}
} convdbid;

void MakeDropList(One<Ctrl>& ctrl)
{
	ctrl.Create<DropList>()
	   .Add(Null, "")
	   .Add(0, "None")
	   .Add(1, "Minimal")
	   .Add(2, "Full");
}

void MakeButton(One<Ctrl>& ctrl)
{
	ctrl.Create<Button>();
}

struct App : public WithGridPanel<TopWindow>
{
	typedef App CLASSNAME;

	EditInt edit0;
	EditInt edit1;
	EditString edit2;
	EditInt e0, e1;
	DropList list, colors, names, numbers;
	Splitter spl;

	WithEditOptionsLayout<ParentCtrl> editopt;
	WithLookOptionsLayout<ParentCtrl> lookopt;
	WithColorsOptionsLayout<ParentCtrl> colsopt;
	WithNumberLayout<ParentCtrl> numsopt;


	GridCtrl grid, grid1, grid2;
	ArrayCtrl arr;


	void InitGrid()
	{
		const int names_cnt = 6;
		const int colors_cnt = 4;
		const int numbers_cnt = 10;

		const char * s_names[] = { "Jack", "Martin", "Smith", "Billy", "Eve", "Pamela" };
		const char * s_colors[] = { "Red", "Blue", "White", "Black" };

		#ifdef LOG_PANEL
		dlog = &log0;
		#endif

		grid.Inserting();
		grid.Appending();
		grid.Moving();
		grid.Removing();
		grid.Navigating();
		grid.Searching();
		grid.Editing();

		grid.WhenLeftDouble = THISBACK(Double);

		for(int i = 0; i < numsopt.total_cols; i++)
		{
			String name = Format("Column %d", i+1);
			int size = rand()%40 + 50;
			grid.AddColumn(name, size).Min(5);
			arr.AddColumn(name, size).HeaderTab().Min(5);
		}

		for(int i = 0; i < names_cnt; i++)
			names.Add(i, s_names[i]);
		for(int i = 0; i < colors_cnt; i++)
			colors.Add(i, s_colors[i]);
		for(int i = 0; i < numbers_cnt; i++)
			numbers.Add(i, Format("Number %d", i));

		grid.GetColumn(1).Edit(names).SetConvert(names);
		grid.GetColumn(2).Edit(numbers).SetConvert(numbers);
		grid.GetColumn(3).Edit(colors).SetConvert(colors);
		grid.GetColumn(4).Edit(edit0);
		grid.GetColumn(5).Edit(edit1);

		numbers.DropFocus();
		names.DropFocus();
		colors.DropFocus();


//		grid.Add(numsopt.total_rows);

		for(int i = 0; i < numsopt.total_rows; i++)
			for(int j = 0; j < numsopt.total_cols; j++)
			{
				switch(j)
				{
					case 0: grid.Set(i, j, Value(i + 1)); break;
					case 1: grid.Set(i, j, rand() % (names_cnt - 1)); break;
					case 2: grid.Set(i, j, rand() % (numbers_cnt - 1)); break;
					case 3: grid.Set(i, j, rand() % (colors_cnt - 1)); break;
					default:
						grid.Set(i, j, Value(rand()%500));

				}
				arr.Set(i, j, Value(rand()%500));
			}

		grid.ResizePaintMode(2);
		grid.ResizeColMode(0);
		grid.ResizeRowMode(0);
		grid.HorzGrid(lookopt.hgrid).VertGrid(lookopt.vgrid);
		grid.EvenColor();
		grid.ColoringMode(2);
		grid.EditCell();
		grid.MultiSelect(editopt.multiline);
		grid.SelectRow(editopt.select_row);
		grid.Sorting(editopt.sorting);
		grid.MovingCols(1).MovingRows(1);
		grid.SetToolBar();
		grid.Dragging();
		grid.SetCursor(0);

		//grid.AutoHideSb();

		arr.HeaderTab(0).SetRatio(10);
		arr.HeaderObject().Absolute();//ReduceNext();
		arr.MultiSelect();
		arr.Inserting();
		arr.SetLineCy(20);
		//arr.ColumnAt(1).Ctrls(MakeDropList);
		//arr.ColumnAt(2).Ctrls(MakeButton);
		arr.ColumnAt(1).Edit(e0);
		arr.ColumnAt(2).Edit(e1);


		grid1.AddColumn("c0");
		grid1.AddColumn("c1");
		grid2.AddColumn("c0");
		grid2.AddColumn("c1");

	}

	void Double()
	{
		if(grid.IsCursor())
			PromptOK("Hello");
	}

	void LookOptions(int n)
	{
		switch(n)
		{
			case 0:	 grid.Indicator(lookopt.indicator);                          break;
			case 1:	 grid.HorzGrid(lookopt.hgrid);                               break;
			case 2:	 grid.VertGrid(lookopt.vgrid);                               break;
			case 3:	 grid.ResizingCols(lookopt.resizing_cols);			         break;
			case 4:	 grid.ResizingRows(lookopt.resizing_rows);			         break;
			case 5:  grid.LiveCursor(lookopt.live_cursor);				         break;
			case 6:	 grid.ResizePaintMode(lookopt.resize_paint_mode.GetIndex()); break;
			case 7:	 grid.ResizeColMode(lookopt.resize_col_mode.GetIndex());     break;
			case 8:  grid.ResizeRowMode(lookopt.resize_row_mode.GetIndex());     break;
			case 9:  grid.MovingCols(lookopt.moving_cols);                       break;
			case 10: grid.MovingRows(lookopt.moving_rows);                       break;
			case 11: grid.FullColResizing(lookopt.full_col_resizing);            break;
			case 12: grid.FullRowResizing(lookopt.full_row_resizing);            break;
			case 13:
				#ifdef flagDEBUG
				Ctrl::ShowRepaint((int) lookopt.repaint ? 30 : 0);
				#endif
				break;

			case 14: grid.Enable(!lookopt.disable);	     break;
			case 15: grid.Chameleon(lookopt.chameleon);  break;
		}
		grid.Repaint();
	}

	void ColorsOptions(int n)
	{
		switch(n)
		{
			case 0: grid.GetDisplay().SetTheme(~colsopt.theme);  break;
			case 1: grid.OddColor(SColorText, ~colsopt.odd_color); break;
			case 2: grid.EvenColor(SColorText, ~colsopt.even_color); break;
			case 3: grid.ColoringMode(colsopt.odd_even_mode); break;
			case 4: grid.GridColor(~colsopt.grid_color); break;
			case 5: grid.FocusColor(SColorText, ~colsopt.focus_color); break;
			case 6: grid.LiveColor(SColorText, ~colsopt.live_color); break;
		}
		grid.Repaint();
	}

	void EditOptions(int n)
	{
		switch(n)
		{
			case 0:	grid.MultiSelect(editopt.multiline);         break;
			case 1:	grid.SelectRow(editopt.select_row);          break;
			case 2:	grid.Sorting(editopt.sorting);               break;
			case 3:	grid.MultiSorting(editopt.multisort);        break;
			case 4:	grid.TabChangesRow(editopt.tab_changes_row); break;
			case 5:	grid.TabAddsRow(editopt.tab_adds_row);		 break;
			case 6:
				if(editopt.edit_mode == 0)
					grid.EditRow();
				else
					grid.EditCell();
				break;
			case 7: grid.OneClickEdit(editopt.one_click_edit);   break;
		}
		grid.Repaint();
	}

	void Actions(int n)
	{
		switch(n)
		{
			case 0:
			/*
				int cnt = grid.GetColumnCount();
				if(cnt < totalCols)
					grid.AddColumn(totalRows - cnt);
				else
					grid.RemoveColumn(cnt - totalRows);
				break;*/
				break;
			case 1:
			{
				int cnt = grid.GetCount();
				if(cnt < numsopt.total_rows)
					grid.Append(numsopt.total_rows - cnt);
				else
					grid.RemoveLast(cnt - numsopt.total_rows);
				break;
			}
			case 2: grid.SetFixedCols(numsopt.fixed_cols); break;
			case 3:	grid.SetFixedRows(numsopt.fixed_rows); break;

			case 4:
				grid.Clear();
				break;
			case 5:
				grid.Reset();
				break;
			case 400:
				log0 <<= "";
				break;
			case 401:
				grid.Debug();
				break;
			case 666:
				Close();
		}
		grid.Repaint();
	}

	App()
	{
		CtrlLayout(*this, "Grid Test Panel");
		CtrlLayout(editopt);
		CtrlLayout(lookopt);
		CtrlLayout(colsopt);
		CtrlLayout(numsopt);

		colsopt.odd_color.NoRampWheel();
		colsopt.even_color.NoRampWheel();
		colsopt.grid_color.NoRampWheel().NotNull();
		colsopt.focus_color.NoRampWheel();
		colsopt.live_color.NoRampWheel();

		opts.Add(lookopt, "Look");
		opts.Add(editopt, "Edit");
		opts.Add(colsopt, "Colors");
		opts.Add(numsopt, "Numbers");

		tab.Add(grid.SizePos(), "GridCtrl");
		tab.Add(arr.SizePos(), "ArrayCtrl");
		tab.Add(spl.Horz(grid1, grid2), "Splitter");

		lookopt.resize_paint_mode.Add("Line");
		lookopt.resize_paint_mode.Add("Line + Fixed");
		lookopt.resize_paint_mode.Add("Fixed + NonFixed");

		lookopt.resize_col_mode.Add("Absolute");
		lookopt.resize_col_mode.Add("Proportional");

		lookopt.resize_row_mode.Add("Absolute");
		lookopt.resize_row_mode.Add("Proportional");

		colsopt.odd_even_mode = 2;

		numsopt.total_cols <<= 8;
		numsopt.total_rows <<= 105;
		numsopt.fixed_cols <<= 0;
		numsopt.fixed_rows <<= 1;

		lookopt.indicator         <<= false;
		lookopt.hgrid             <<= true;
		lookopt.vgrid             <<= true;
		lookopt.resizing_cols     <<= true;
		lookopt.resizing_rows     <<= true;
		lookopt.moving_cols       <<= true;
		lookopt.moving_rows       <<= true;
		lookopt.full_col_resizing <<= true;
		lookopt.full_row_resizing <<= false;
		lookopt.chameleon         <<= false;

		editopt.multiline         <<= true;
		editopt.select_row        <<= false;
		editopt.sorting           <<= true;
		editopt.multisort         <<= true;
		editopt.tab_changes_row   <<= true;
		editopt.tab_adds_row      <<= true;
		editopt.edit_mode         = 1;
		editopt.one_click_edit    <<= false;

		lookopt.resize_paint_mode.SetIndex(2);
		lookopt.resize_col_mode.SetIndex(0);
		lookopt.resize_row_mode.SetIndex(0);

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

		editopt.multiline         <<= THISBACK1(EditOptions, 0);
		editopt.select_row        <<= THISBACK1(EditOptions, 1);
		editopt.sorting           <<= THISBACK1(EditOptions, 2);
		editopt.multisort         <<= THISBACK1(EditOptions, 3);
		editopt.tab_changes_row   <<= THISBACK1(EditOptions, 4);
		editopt.tab_adds_row      <<= THISBACK1(EditOptions, 5);
		editopt.edit_mode         <<= THISBACK1(EditOptions, 6);
		editopt.one_click_edit    <<= THISBACK1(EditOptions, 7);

		for(int i = 0; i < grid.GetDisplay().GetThemeCount(); i++)
			colsopt.theme.Add(i, Format("Theme %d", i + 1));

		colsopt.theme <<= 5;

		colsopt.theme             <<= THISBACK1(ColorsOptions, 0);
		colsopt.odd_color         <<= THISBACK1(ColorsOptions, 1);
		colsopt.even_color        <<= THISBACK1(ColorsOptions, 2);
		colsopt.odd_even_mode     <<= THISBACK1(ColorsOptions, 3);
		colsopt.grid_color        <<= THISBACK1(ColorsOptions, 4);
		colsopt.focus_color       <<= THISBACK1(ColorsOptions, 5);
		colsopt.live_color        <<= THISBACK1(ColorsOptions, 6);

		numsopt.total_cols        <<= THISBACK1(Actions, 0);
		numsopt.total_rows        <<= THISBACK1(Actions, 1);
		numsopt.fixed_cols        <<= THISBACK1(Actions, 2);
		numsopt.fixed_rows        <<= THISBACK1(Actions, 3);

		clear_grid                <<= THISBACK1(Actions, 4);
		reset_grid                <<= THISBACK1(Actions, 5);

		exit                      <<= THISBACK1(Actions, 666);

		clear                     <<= THISBACK1(Actions, 400);
		#ifndef flagDEBUG
		debug.Hide();
		#else
		debug                     <<= THISBACK1(Actions, 401);
		#endif


		InitGrid();

		colsopt.odd_color         <<= grid.bg_odd;
		colsopt.even_color        <<= grid.bg_even;
		colsopt.grid_color        <<= grid.fg_grid;
		colsopt.focus_color       <<= grid.bg_focus;
		colsopt.live_color        <<= grid.bg_live;

		Sizeable().Zoomable()/*.BackPaint(false)*/;
	}
};

GUI_APP_MAIN
{
//	Ctrl::ShowRepaint(50);
	SetLanguage(LNGC_('E','N','U','S', CHARSET_UNICODE));
	App().Run();
}
