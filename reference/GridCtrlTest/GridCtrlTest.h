#ifndef _GridTest_GridCtrlTest_h_
#define _GridTest_GridCtrlTest_h_

#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>

using namespace Upp;

#include "Panel.h"

#define  LAYOUTFILE <GridCtrlTest/GridCtrlTest.lay>
#include <CtrlCore/lay.h>

struct Test : public Ctrl
{
	virtual void Init() = 0;
};

struct BasicTest : Test
{
	GridCtrl grid;
	BasicTest()
	{
		Add(grid.SizePos());
	}
};

struct TestMain : BasicTest
{
	EditInt edit0;
	EditInt edit1;
	EditString edit2;
	DropTime dt;
	EditInt ei;
	EditString ee0, ee1;
	
	DropList list, colors, names, numbers;
	
	void Init();
};

struct TestExcelGrid : BasicTest
{
	void Init();
};

struct TestPropertyGrid : BasicTest
{
	void Init();
};

struct TestFocusLost : Test
{
	Splitter spl;
	GridCtrl grid0;
	GridCtrl grid1;
	
	void Init();
};

struct TestArrayCtrl : Test
{
	EditInt e0, e1;
	ArrayCtrl arr;
	
	void Init();
};

struct App : public WithGridPanel<TopWindow>
{
	typedef App CLASSNAME;

	Splitter spl;
	
	TestMain testMain;
	TestExcelGrid testExcelGrid;
	TestPropertyGrid testPropertyGrid;
	TestFocusLost testFocusLost;
	TestArrayCtrl testArrayCtrl;
	
	void TabChange();
	void Init();
	void Serialize(Stream &s);
	void Resort();
	void ToXml();
	void FromXml();
	App();
};

#endif
