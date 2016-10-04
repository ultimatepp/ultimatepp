#include <CtrlLib/CtrlLib.h>
#include <GridCtrlMC/GridCtrl.h>

using namespace Upp;

#define  LAYOUTFILE <GridTest/main.lay>
#include <CtrlCore/lay.h>
#include <stdlib.h>

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

	//((DropList &) ctrl).
}

void MakeButton(One<Ctrl>& ctrl)
{
	ctrl.Create<Button>();
	ctrl->WantFocus();
}

void MakeCheck(One<Ctrl>& ctrl)
{
	ctrl.Create<Option>();
	ctrl->WantFocus();
}

void MakeEdit(One<Ctrl>& ctrl)
{
	ctrl.Create<EditInt>();
	ctrl->WantFocus();
}


struct Dynamic : public WithDynamicLayout<ParentCtrl>
{
	typedef Dynamic CLASSNAME;

	GridCtrl * grid;
	EditString es;

	Dynamic()
	{
		CtrlLayout(*this);
		create <<= THISBACK(CreateGrid);
		remove <<= THISBACK(RemoveGrid);
		grid = NULL;
	}

	~Dynamic()
	{
		RemoveGrid();
	}

	void CreateGrid()
	{
		if(grid)
			return;

		grid = new GridCtrl;

		grid->AddColumn("Column 0");
		grid->AddColumn("Column 1");

		grid->AddRow(50);

		//grid->HSizePos(0, 0).VSizePos(40, 0);
//		spl.Horz((*grid).SizePos(), es.SizePos());
		spl.Horz((*grid).SizePos(), es.SizePos());
	//	Add(*grid);
	}

	void RemoveGrid()
	{
		if(!grid)
			return;

		RemoveChild(grid);
		delete grid;
		grid = NULL;
	}
};

struct App : public WithGridPanel<TopWindow>
{
	typedef App CLASSNAME;

	Dynamic dynamic;
	EditInt edit0;
	EditInt edit1;
	EditString edit2;
	EditInt e0, e1;
	DropList list, colors, names, numbers;
	Splitter spl;
	DateTimeCtrl dt;

	EditString ee0, ee1;

	WithEditOptionsLayout<ParentCtrl> editopt;
	WithLookOptionsLayout<ParentCtrl> lookopt;
	WithColorsOptionsLayout<ParentCtrl> colsopt;
	WithNumberLayout<ParentCtrl> numsopt;


	GridCtrl grid, grid1, grid2, empty_grid;
	ArrayCtrl arr;

	void InitGrid()
	{
		const int names_cnt = 6;
		const int colors_cnt = 4;
		const int numbers_cnt = 10;

		const char * s_names[] = { "Jack", "Martin", "Smith", "Billy", "Eve", "Pamela" };
		const char * s_colors[] = { "Red", "Blue", "White", "Black" };

		//dlog = &log0;

		grid.Inserting();
		grid.Appending();
		grid.Moving();
		grid.Removing();
		grid.Navigating();
		grid.Searching();
		grid.Editing();
		grid.Hiding();

		//grid.WhenLeftDouble = THISBACK(Double);

		for(int i = 0; i < numsopt.total_cols; i++)
		{
			String name = Format("Column %d", i+1);
			int size = rand()%40 + 50;
			grid.AddColumn(name).Width(size);//.Min(5).Max(25);
			arr.AddColumn(name, size).HeaderTab().Min(5);
			empty_grid.AddColumn(name).Width(size);
		}

		for(int i = 0; i < names_cnt; i++)
			names.Add(i, s_names[i]);
		for(int i = 0; i < colors_cnt; i++)
			colors.Add(i, s_colors[i]);
		for(int i = 0; i < numbers_cnt; i++)
			numbers.Add(i, Format("Number %d", i));

		grid.GetColumn(1).Edit(names).SetConvert(names).Default(0);
		grid.GetColumn(2).Edit(numbers).SetConvert(numbers).Default(1);
		grid.GetColumn(3).Edit(colors).SetConvert(colors).Default(2);
		grid.GetColumn(4).Edit(edit0);
		grid.GetColumn(5).Edit(edit1);
		grid.GetColumn(6).Edit(dt);
		grid.GetColumn(7).Ctrls(MakeCheck).CtrlAlignHorzCenter(15);
		//grid.GetColumn(8).Ctrls(MakeEdit).CtrlAlignHorzPos(5, 5).CtrlAlignTop(5, 19);

		grid.GetColumn(9).Ctrls(MakeCheck).CtrlAlignHorzCenter(15);//CtrlAlignHorzPos(5, 5).CtrlAlignTop(5, 19);
		grid.GetColumn(10).Ctrls(MakeCheck).CtrlAlignHorzCenter(15);//CtrlAlignHorzPos(5, 5).CtrlAlignTop(5, 19);
		grid.GetColumn(11).Ctrls(MakeCheck).CtrlAlignHorzCenter(15);//CtrlAlignHorzPos(5, 5).CtrlAlignTop(5, 19);

		//grid.GetColumn(3).NoEditable();

		//grid.GetColumn(numsopt.total_cols - 1 + 0);
		//grid.GetColumn(numsopt.total_cols - 2 + 0);//.Ctrls(MakeCheck);//Fixed(20);
		//grid.GetColumn(numsopt.total_cols - 3 + 0).Ctrls(MakeEdit);

//		grid.GetColumn(8).Ctrls(MakeDropList);

		grid.GetColumn(numsopt.total_cols - 1).Min(40).Max(40);
		grid.GetColumn(numsopt.total_cols - 2).Fixed(50);
		grid.GetColumn(numsopt.total_cols - 3).Fixed(40);

		numbers.DropFocus();
		names.DropFocus();
		colors.DropFocus();

		grid.SetDefaultRowHeight(26);

		grid.AddRow(numsopt.total_rows);

		grid.FixedPaste();

		Date curdate = GetSysDate();

		for(int i = 0; i < numsopt.total_rows; i++)
			for(int j = 0; j < numsopt.total_cols; j++)
			{
				switch(j)
				{
					case 0: grid.Set(i, j, Value(i + 1)); break;
					case 1: grid.Set(i, j, rand() % (names_cnt - 1)); break;
					case 2: grid.Set(i, j, rand() % (numbers_cnt - 1)); break;
					case 3: grid.Set(i, j, rand() % (colors_cnt - 1)); break;
					case 6: grid.Set(i, j, Date(curdate.year, curdate.month, rand() % 27 + 1)); break;
					case 9:
					case 10:
					case 11:
					case 7: grid.Set(i, j, rand() % 1); break;
					default:
						grid.Set(i, j, Value(rand()%500));

				}
				arr.Set(i, j, Value(rand()%500));
				empty_grid.Set(i, j, i * numsopt.total_cols + j);
			}

		empty_grid.SelectRow(false);

		grid.ResizePaintMode(lookopt.resize_paint_mode.GetIndex());
		grid.ResizeColMode(lookopt.resize_col_mode.GetIndex());
		grid.ResizeRowMode(lookopt.resize_row_mode.GetIndex());
		grid.HorzGrid(lookopt.hgrid).VertGrid(lookopt.vgrid);
		grid.EvenColor();
		grid.ColoringMode(2);
		grid.Indicator(lookopt.indicator);
		grid.EditCell();
		grid.MultiSelect(editopt.multiline);
		grid.SelectRow(editopt.select_row);
		grid.Sorting(editopt.sorting);
		grid.MovingCols(1).MovingRows(1);
		grid.SetToolBar();
		grid.Dragging();
		grid.SetFixedCols(numsopt.fixed_cols);
		grid.SetFixedRows(numsopt.fixed_rows);
		grid.SetCursor(0);
		//grid.Proportional();

		grid.GetRow(0).NoEditable();
		grid.GetRow(1).NoEditable();

		grid.SetRowHeight(3, 30);

		grid.GetColumn(1).AlignCenter();
		grid.JoinFixedCells(1, 0, 5, 0);
		grid.JoinCells(1, 3, 4, 5);
		grid.JoinCells(6, 2, 8, 6);
		grid.JoinCells(6, 8, 7, 9);
		grid.JoinCells(6, 11, 9, 11);
		grid.JoinCells(9, 8, 10, 8);

		//grid.AutoHideSb(false);

		arr.HeaderTab(0).SetRatio(10);
		arr.HeaderObject().Absolute();
		arr.MultiSelect();
		arr.Inserting();
		arr.SetLineCy(20);
		arr.ColumnAt(1).Ctrls(MakeDropList);
		arr.ColumnAt(2).Ctrls(MakeButton);
		arr.ColumnAt(3).Ctrls(MakeEdit);
		arr.ColumnAt(1).Edit(e0);
		arr.ColumnAt(2).Edit(e1);


		grid1.AddColumn("c0").Edit(ee0);
		grid1.AddColumn("c1").Edit(ee1);
		grid1.AddRow(50);
		//grid1.Indicator();
		grid2.AddColumn("c0");
		grid2.AddColumn("c1");
		//grid1.AddRow();
		//grid2.AddRow(3);
		//grid1.SetFixedRows(1);

//		grid.GSort(1, 1, grid.GetFixedCount());

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
				Ctrl::ShowRepaint((int) lookopt.repaint ? 20 : 0);
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
			case 8: grid.EnterLikeTab(editopt.enter_like_tab);   break;
		}
		grid.Repaint();
	}

	void Actions(int n)
	{
		switch(n)
		{
			case 0:
			{
				/*
				int cnt = grid.GetColumnCount();
				if(cnt < numsopt.total_cols)
					grid.AddColumn(numsopt.total_cols - cnt);
				else
					grid.RemoveColumn(cnt - numsopt.total_cols);
				*/
				break;
			}
			case 1:
			{
				int cnt = grid.GetTotalCount();
				if(cnt < numsopt.total_rows)
					grid.Append(numsopt.total_rows - cnt + 1);
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
				grid.Debug(0);
				break;
			case 666:
				Close();
		}
		grid.Repaint();
	}

	void Serialize(Stream &s)
	{
		SerializePlacement(s);
		s % level;
	}

	App()
	{
		dlog = &log0;
		dlev = &level;

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
		//tab.Add(spl.Horz(grid1, grid2), "Splitter");
		//tab.Add(dynamic.SizePos(), "Dynamic");
		tab.Add(arr.SizePos(), "ArrayCtrl");
		tab.Add(empty_grid.SizePos(), "Empty grid");

		lookopt.resize_paint_mode.Add("Line");
		lookopt.resize_paint_mode.Add("Line + Fixed");
		lookopt.resize_paint_mode.Add("Fixed + NonFixed");

		lookopt.resize_col_mode.Add("Absolute");
		lookopt.resize_col_mode.Add("Proportional");

		lookopt.resize_row_mode.Add("Absolute");
		lookopt.resize_row_mode.Add("Proportional");

		colsopt.odd_even_mode = 2;

		numsopt.total_cols <<= 15;
		numsopt.total_rows <<= 15;
		numsopt.fixed_cols <<= 0;
		numsopt.fixed_rows <<= 4;

		lookopt.indicator         <<= !false;
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
		editopt.select_row        <<= !false;
		editopt.sorting           <<= true;
		editopt.multisort         <<= true;
		editopt.tab_changes_row   <<= true;
		editopt.tab_adds_row      <<= true;
		editopt.edit_mode         = 1;
		editopt.one_click_edit    <<= false;

		lookopt.resize_paint_mode.SetIndex(2);
		lookopt.resize_col_mode.SetIndex(1);
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
		editopt.enter_like_tab    <<= THISBACK1(EditOptions, 8);


/*		for(int i = -1; i < grid.GetDisplay().GetThemeCount(); i++)
			colsopt.theme.Add(i, Format("Theme %d", i + 2));
*/
		colsopt.theme.Add(-1, "Flat");
		colsopt.theme.Add(0, "Cairo");
		colsopt.theme.Add(1, "Gray");
		colsopt.theme.Add(2, "Plastik");
		colsopt.theme.Add(3, "Lipstik");
		colsopt.theme.Add(4, "Alloy");
		colsopt.theme.Add(5, "Unknown 01");
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

		numsopt.total_cols.Disable();

		clear_grid                <<= THISBACK1(Actions, 4);
		reset_grid                <<= THISBACK1(Actions, 5);

		exit                      <<= THISBACK1(Actions, 666);

		clear                     <<= THISBACK1(Actions, 400);
		#ifndef flagDEBUG
		debug.Hide();
		level.Hide();
		#else
		debug                     <<= THISBACK1(Actions, 401);
		level                     <<= THISBACK1(Actions, 402);
		#endif

		level.Add(0, "All")
		     .Add(1, "1")
		     .Add(2, "2");

		level <<= 0;

		InitGrid();

		colsopt.odd_color         <<= grid.bg_odd;
		colsopt.even_color        <<= grid.bg_even;
		colsopt.grid_color        <<= grid.fg_grid;
		colsopt.focus_color       <<= grid.bg_focus;
		colsopt.live_color        <<= grid.bg_live;

		Sizeable().Zoomable();//.BackPaint(!false);
	}
};

GUI_APP_MAIN
{
	App app;
	LoadFromFile(app);
	SetLanguage(LNGC_('E','N','U','S', CHARSET_UNICODE));
	app.Run();
	StoreToFile(app);
}
