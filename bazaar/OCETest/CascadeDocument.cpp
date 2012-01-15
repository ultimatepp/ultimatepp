#include "CascadeDocument.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Static members initialization
#ifdef WIN32
		Handle(Graphic3d_WNTGraphicDevice) CascadeDocument::GraphicDevice = 0;
#else
		Handle(Graphic3d_GraphicDevice) CascadeDocument::GraphicDevice = 0;
#endif
int CascadeDocument::Instances = 0;


/////////////////////////////////////////////////////////////////////////////////////////
// Constructor
CascadeDocument::CascadeDocument()
{
	// CREATES THE GRAPHIC DEVICE
    if( GraphicDevice.IsNull() )
#ifdef WIN32
        GraphicDevice = new Graphic3d_WNTGraphicDevice();
#else
//        GraphicDevice = new Graphic3d_GraphicDevice(getenv("DISPLAY"));
        GraphicDevice = new Graphic3d_GraphicDevice(getenv(""));
#endif

	// CREATES THE VIEWER
    TCollection_ExtendedString a3DName("Visu3D");
	Viewer = new V3d_Viewer(
	    GraphicDevice,			// the graphic device
		a3DName.ToExtString(),	// view name
		"",						// domain (?)
		1000.0,					// view size
		V3d_XposYnegZpos,		// View projection
		Quantity_NOC_GRAY30,
		V3d_ZBUFFER,
		V3d_GOURAUD,
		V3d_WAIT,
		Standard_True,			// Computed mode (?)
		Standard_True,			// Default Computed mode (?)
		V3d_TEX_NONE			// (?)
	);
	
	// INITIALIZES THE VIEWER							 
    Viewer->Init();
	Viewer->SetDefaultLights();
	Viewer->SetLightOn();
	
	// CREATES THE INTERACTIVE CONTEXT
	Context = new AIS_InteractiveContext(Viewer);	
//	onCreateNewView();

	// COUNTS THE INSTANCES OF CascadeCtrl
	// THAT'S USED TO FREE GraphicDevice ON LAST CascadeCtrl DELETION
	Instances++;


} // END Constructor class CascadeDocument

/////////////////////////////////////////////////////////////////////////////////////////
// Destructor
CascadeDocument::~CascadeDocument()
{
	// FREES THE CONTEXT
	Context.Nullify();
	
	// FREES THE VIEWER
	Viewer.Nullify();
	
	// DECREMENT INSTANCE COUNTER, IF NULL FREES GraphicDevice
	//ASSERT(Instances > 0);
	if(!--Instances)
	{
		GraphicDevice.Nullify();
	}
	
} // END Destructor class CascadeDocument
