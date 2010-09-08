#include "GridCtrlTest.h"

void TestExcelGrid::Init()
{
	const int total_cols = 50;
	
	for(int i = 0; i < total_cols; i++)
	{
		String name = Format("Column %d", i + 1);
		grid.AddColumn((const char *)name).Width(80);
	}
	
	grid.SelectRow(false);
	grid.SetRowCount(200);
	grid.ResizePaintMode(1);
	grid.Absolute();
	grid.Clipboard();
}