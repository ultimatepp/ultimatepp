#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include "OCETest.h"
#include "MakeBottle.h"

#include <AIS_Shape.hxx>

INITBLOCK {

#ifdef PLATFORM_POSIX
	setenv("MMGT_OPT", "0", 1);
	setenv("MMGT_CLEAR", "0", 1);
#else
	SetEnvironmentVariable("MMGT_OPT", "0");
	SetEnvironmentVariable("MMGT_CLEAR", "0");

#endif
}
GUI_APP_MAIN
{
	TopWindow win;
	win.LeftPos(200, 800);
	win.TopPos(200, 400);

	CascadeDocument Document;

	CascadeView View(&Document);

	win.Add(View.HSizePos(10, 10).VSizePos(10, 10));
	win.Sizeable().Zoomable();
	
	win.Open();

    TopoDS_Shape aBottle = ::MakeBottle(50,70,30);
    Handle(AIS_Shape) AISBottle=new AIS_Shape(aBottle);
    Document.GetContext()->SetMaterial(AISBottle,Graphic3d_NOM_GOLD);
    Document.GetContext()->SetDisplayMode(AISBottle,1,Standard_False);
    Document.GetContext()->Display(AISBottle, Standard_False);	
    Document.GetContext()->SetCurrentObject(AISBottle,Standard_False);
    View.FitAll();
	
    win.Run();
}

