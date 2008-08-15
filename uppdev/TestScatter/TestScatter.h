#ifndef _TestScatter_TestScatter_h
#define _TestScatter_TestScatter_h

#include <CtrlLib/CtrlLib.h>
#include <Scatter/Scatter.h>

#define LAYOUTFILE <TestScatter/TestScatter.lay>
#include <CtrlCore/lay.h>




class TestScatter : public WithTestScatterLayout<TopWindow> {

public:
	typedef TestScatter CLASSNAME;
	TestScatter();
	void ModifPoint();
	void AddPoint();
	void RemovePoint();
	void InsertPoint();
	void ChangeColor();
	void ChangeMarkColor();
	void Join();

	void RemoveSeries();
	void RemoveFunct();

	void Zoom();


	void SaveJPG();
	void SavePNG();
	void Preview();
};

#endif
