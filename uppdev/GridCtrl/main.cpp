#include <CtrlLib/CtrlLib.h>
#include "GridCtrl.h"
#define  LAYOUTFILE "main.lay"
#include <CtrlCore/lay.h>
#include <stdlib.h>

bool logOn = true;
//extern int Ctrl::ShowRepaint;

struct ConvDbId : Convert
{
    virtual Value Format(const Value& q) const
    {
    	return ::Format("%.4d", q);
	}
} convdbid;



struct App : public WithGridPanel<TopWindow>
{

	typedef App CLASSNAME;

	EditInt edit0;
	EditString edit1;
	EditString edit2;
	DropList   list;

	void InitGrid()
	{
		for(int i = 0; i < totalCols; i++)
		{
			grid.AddColumn(Format("Column %d", i+1), rand()%40 + 50).Min(20);
		}
		grid.GetColumn(1).Edit(edit0);//.SetConvert(convdbid);
		grid.GetColumn(2).Edit(edit1);
		grid.GetColumn(3).Edit(list);
		grid.GetColumn(4).Edit(edit2);
		for(int i = 0; i < 10; i++)
			list.Add(i, Format("Value %d", i));


/*		grid.SetColCtrl(2, edit0);
		grid.SetColCtrl(3, list);
		grid.SetColCtrl(4, edit1);*/

		grid.Indicator(indicator);
		grid.SetDefaultRowHeight(19);

		for(int i = 0; i < totalRows - 1; i++)
			grid.Add();


		for(int i = 1; i < totalRows; i++)
			for(int j = 0; j < totalCols; j++)
			{
				//grid.Set(i, j, (Value) Format("Value (%d, %d)", j, i));
//				grid.Set(i, j, (Value) Format("%d", (int) rand()%9));
				grid.Set(i, j, Value(rand()%9));
			}

		grid.SetRowHeight(0, 20);
		//grid.SetColor(colEvenRow, Color(255,255,0));

		grid.HGrid(hgrid).VGrid(vgrid);
		grid.SetFixedRows(fixedRows);
		grid.Multiline(multiline);
		grid.SelectRow(selectRow);
		grid.Sorting(sorting);
		grid.ResizePaintMode(2);
		grid.ResizeColMode(0);
		grid.ResizeRowMode(0);
		grid.ColOddColor(Black, Color(242,242,242));
		grid.ColEvenColor(Black, White);

		grid.Fixed3DMode(0);

		grid.EditRow();

		dlog = &log0;

		StdGridDisplay.SetTextAlign(alcenter, alcenter);
	}

	void OnGridSettingsChange(int n)
	{
		Rect rw; int i;
		switch(n)
		{
			case 0:
				break;
			case 1:
				break;
			case 2: 	grid.SetFixedCols(fixedCols); 				break;
			case 3:		grid.SetFixedRows(fixedRows); 				break;
			case 4:		grid.Indicator(indicator);    				break;
			case 5:		grid.Multiline(multiline);					break;
			case 6:		grid.SelectRow(selectRow);					break;
			case 7:		grid.HGrid(hgrid);							break;
			case 8:		grid.VGrid(vgrid);							break;
			case 9:		grid.ResizingCols(resizingCols);			break;
			case 10:	grid.ResizingRows(resizingRows);			break;
			case 11:	grid.Sorting(sorting);						break;
			case 12:	grid.LiveCursor(liveCursor);				break;
			case 13:	grid.ResizePaintMode(resizePMode.GetIndex()); break;
			case 14:	grid.MultiSorting(multisort);				break;
			case 15:
				i = resizeColMode.GetIndex();
				grid.ResizeColMode(i);
				if(i == 1)
				{
					grid.th.RecalcCols();
					grid.Refresh();
				}
				break;
			case 16:
				i = resizeRowMode.GetIndex();
				grid.ResizeRowMode(i);
				if(i == 1)
				{
					grid.lh.RecalcRows();
					grid.Refresh();
				}
				break;

			case 664:
				rw = GetRect();
				if(logOn)
				{
					rw.right -= 100;
					log.SetLabel("Log >>>");
				}
				else
				{
					rw.right += 100;
					log.SetLabel("Log <<<");
				}
				SetRect(rw);
				logOn = !logOn;
				break;
			case 665:
				log0 <<= "";
				break;
			case 666:
				Close();
		}
		grid.Refresh();
	}

	App()
	{
    	CtrlLayout(*this, "Grid Test Panel");
    	//SetRect(0, 0, 800, 600);

		//grid.dbg0 = &edt0;
		//grid.dbg1 = &edt1;

		resizePMode.Add("Line");
		resizePMode.Add("Line + Fixed");
		resizePMode.Add("Fixed + NonFixed");

		resizeColMode.Add("Absolute");
		resizeColMode.Add("Proportional");
		resizeColMode.Add("Reduce next");

		resizeRowMode.Add("Absolute");
		resizeRowMode.Add("Proportional");
		resizeRowMode.Add("Reduce next");

		totalCols <<= 5;
		totalRows <<= 16;
		fixedCols <<= 0;
		fixedRows <<= 1;

		indicator    <<= false;
		multiline    <<= true;
		selectRow    <<= false;
		hgrid        <<= true;
		vgrid        <<= true;
		resizingCols <<= true;
		resizingRows <<= true;
		sorting      <<= true;
		multisort    <<= true;
		resizePMode.SetIndex(2);
		resizeColMode.SetIndex(0);
		resizeRowMode.SetIndex(0);

		totalCols    <<= THISBACK1(OnGridSettingsChange, 0);
		totalRows    <<= THISBACK1(OnGridSettingsChange, 1);
		fixedCols    <<= THISBACK1(OnGridSettingsChange, 2);
		fixedRows    <<= THISBACK1(OnGridSettingsChange, 3);
		indicator    <<= THISBACK1(OnGridSettingsChange, 4);
		multiline    <<= THISBACK1(OnGridSettingsChange, 5);
		selectRow    <<= THISBACK1(OnGridSettingsChange, 6);
		hgrid        <<= THISBACK1(OnGridSettingsChange, 7);
		vgrid        <<= THISBACK1(OnGridSettingsChange, 8);
		resizingCols <<= THISBACK1(OnGridSettingsChange, 9);
		resizingRows <<= THISBACK1(OnGridSettingsChange, 10);
		sorting      <<= THISBACK1(OnGridSettingsChange, 11);
		liveCursor   <<= THISBACK1(OnGridSettingsChange, 12);
		resizePMode  <<= THISBACK1(OnGridSettingsChange, 13);
		multisort    <<= THISBACK1(OnGridSettingsChange, 14);
		resizeColMode <<= THISBACK1(OnGridSettingsChange, 15);
		resizeRowMode <<= THISBACK1(OnGridSettingsChange, 16);
		log          <<= THISBACK1(OnGridSettingsChange, 664);
		clear        <<= THISBACK1(OnGridSettingsChange, 665);
		exit         <<= THISBACK1(OnGridSettingsChange, 666);


		InitGrid();

		Sizeable().Zoomable();
	}
};

GUI_APP_MAIN
{
	//Ctrl::ShowRepaint = 30;
	App().Run();
}
