#include <CtrlLib/CtrlLib.h>

#include "OCETest.h"
#include "MakeBottle.h"

#include <AIS_Shape.hxx>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;
	win.LeftPos(200, 800);
	win.TopPos(200, 400);

	CascadeDocument document;

	CascadeView view;
	view.SetDocument(&document);

	win.Add(view.HSizePos(10, 10).VSizePos(10, 10));
	win.Sizeable().Zoomable();
	
	win.Open();

    TopoDS_Shape aBottle = ::MakeBottle(50,70,30);
    Handle(AIS_Shape) AISBottle=new AIS_Shape(aBottle);
    document.GetContext()->SetMaterial(AISBottle,Graphic3d_NOM_GOLD);
    document.GetContext()->SetDisplayMode(AISBottle,1,Standard_False);
    document.GetContext()->Display(AISBottle, Standard_False);	
    document.GetContext()->SetCurrentObject(AISBottle,Standard_False);
    view.FitAll();
	
    win.Run();
}

