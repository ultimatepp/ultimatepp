#include "GridCtrlTest.h"

void TestMain::Init()
{
	const int names_cnt = 6;
	const int colors_cnt = 4;
	const int numbers_cnt = 10;
	
	const char * s_names[] = { "Jack", "This is a very long multiline text\n\n\n1234\n5678\nThis text is absolutely about nothing interesting. This text is absolutely about nothing interesting. This text is absolutely about nothing interesting. This text is absolutely about nothing interesting. This text is absolutely about nothing interesting. This text is absolutely about nothing interesting. This text is absolutely about nothing interesting.", "Smith", "Billy", "Eve", "Pamela" };
	const char * s_colors[] = { "Red", "Blue", "White", "Black" };
		
	grid.Inserting(false);
	grid.Appending(!false);
	grid.Moving();
	grid.Removing();
	grid.Navigating();
	grid.Searching();
	grid.Editing();
	grid.Hiding();
	grid.Duplicating();
	grid.AfterBeforeInserting();
	grid.Accepting().Canceling();
	grid.Clipboard();
	grid.SummaryRow();
	
	const int total_cols = 15;
	const int total_rows = 10;
	
	for(int i = 0; i < total_cols; i++)
	{
		String name = Format("Column %d", i + 1);
		int size = rand() % 40 + 50;
		grid.AddColumn((const char *)name).Width(size);
	}
		
	for(int i = 0; i < names_cnt; i++)
		names.Add(i, s_names[i]);
	for(int i = 0; i < colors_cnt; i++)
		colors.Add(i, s_colors[i]);
	for(int i = 0; i < numbers_cnt; i++)
		numbers.Add(i, Format("Number %d", i));
	
	grid.GetColumn(1).Edit(names).SetConvert(names).Default(0).SetImage(CtrlImg::go_forward());
	grid.GetColumn(2).Edit(numbers).SetConvert(numbers).Default(1);
	grid.GetColumn(3).Edit(colors).SetConvert(colors).Default(2);
	grid.GetColumn(4).Edit(edit0).DoCount();
	grid.GetColumn(5).Edit(edit1).DoSum("Sum: %` $");
	grid.GetColumn(6).Edit(dt).DoMin();

	grid.GetColumn(9).Option();
	grid.GetColumn(10).Option();
	grid.GetColumn(11).Option();
	
	grid.GetColumn(total_cols - 1).Min(40).Max(40);
	grid.GetColumn(total_cols - 2).Fixed(50);
	grid.GetColumn(total_cols - 3).Fixed(40);

	grid.SetDefaultRowHeight(DPI(26));			
	grid.AddRow(total_rows);
	grid.FixedPaste();
	
	Date curdate = GetSysDate();

	for(int i = 0; i < total_rows; i++)
	{
		for(int j = 0; j < total_cols; j++)
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
					grid.Set(i, j, Value(rand() % 500));               
			}
		}
	}
	
	grid.EvenColor();
	grid.ColoringMode(2);
	grid.Indicator();
	grid.EditCell();
	grid.MultiSelect();
	grid.SelectRow();
	grid.Sorting();
	grid.MovingCols(1).MovingRows(1);
	grid.SetToolBar();
	grid.Dragging();
	grid.SetCursor(0);
	grid.DrawFocus();
	grid.AddColumn().WrapText();	
	grid.SetCtrl(0, 0, ei);
}
