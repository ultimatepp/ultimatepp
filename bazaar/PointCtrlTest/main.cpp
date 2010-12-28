#include "PointCtrlTest.h"

PointCtrlTest::PointCtrlTest()
{
	CtrlLayout(*this, "Window title");
	Sizeable().Zoomable();

	pc.SetMinMaxXY(0,100,0,100);
	pc <<= THISBACK(PosCB);

	pc.WhenAddPoint = THISBACK(OnAddPoint);
	pc.WhenRemovePoint = THISBACK(OnRemovePoint);
	pc.WhenMovePoint = THISBACK(OnMovePoint);

	slx.MinMax(0,100);
	slx <<= THISBACK(SlXCB);
	sly.MinMax(0,100);
	sly <<= THISBACK(SlYCB);
	
	PointCtrl();
}

void PointCtrlTest::SlXCB()
{
	pc.SetDataX(slx.GetData());
	PosCB(); //show
}

void PointCtrlTest::SlYCB()
{
	pc.SetDataY(sly.GetData());
	PosCB(); //show
}

void PointCtrlTest::PosCB()
{
	double x = pc.GetDataX();
	double y = pc.GetDataY();
	slx.SetData(x);
	sly.SetData(y);
	ToInfo(String().Cat() << "(" << x << "," << y << ")");
}

void PointCtrlTest::OnAddPoint()
{
	ToInfo(String().Cat() << "Added: XY(" << pc.vp.Top() << ")");
}

void PointCtrlTest::OnRemovePoint(int i)
{
	ToInfo(String().Cat() << "Removed Point @: " << i);
}

void PointCtrlTest::OnMovePoint(int i)
{
	ToInfo(String().Cat() << "MovePoint @: " << i << " XY(" << pc.vp[i] << ")");
}

GUI_APP_MAIN
{
	PointCtrlTest().Run();
}

