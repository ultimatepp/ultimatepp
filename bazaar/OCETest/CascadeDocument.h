#ifndef _CascadeDocument_h_
#define _CascadeDocument_h_

#include <Core/Core.h>

#include <OCE/OCE.h>

#ifdef WIN32
#include <WNT_Window.hxx>
#include <Graphic3d_WNTGraphicDevice.hxx>
#else
#include <Xw_Window.hxx>
#include <Graphic3d_GraphicDevice.hxx>
#endif

#include <V3d_Viewer.hxx>
#include <AIS_InteractiveContext.hxx>

class CascadeDocument
{
	private:
		// THE GRAPHIC DEVICE -- STATIC, COMMON TO ALL CascadeDocuments
#ifndef WIN32
		static Handle(Graphic3d_GraphicDevice) GraphicDevice;
#else
		static Handle(Graphic3d_WNTGraphicDevice) GraphicDevice;
#endif

		// THE VIEWER
		Handle(V3d_Viewer) Viewer;
		
		// THE INTERACTIVE CONTEXT
		Handle(AIS_InteractiveContext) Context;
		
		// NUMBER OF CREATED INSTANCES
		static int Instances;
		
	public:
	
		// Constructor
		CascadeDocument();

		// Destructor
		~CascadeDocument();

		// Graphic device property
#ifndef WIN32
		Handle(Graphic3d_GraphicDevice) const &GetGraphicDevice() {return GraphicDevice; };
#else
		Handle(Graphic3d_WNTGraphicDevice) const &GetGraphicDevice() {return GraphicDevice; };
#endif

		// Viewer property
		Handle(V3d_Viewer) const &GetViewer() { return Viewer; };
		
		// Interactive context property
		Handle(AIS_InteractiveContext) const &GetContext() { return Context; };


}; // END Class CascadeDocument

#endif
