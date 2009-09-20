#ifndef _TestLeptonica_TestLeptonica_h
#define _TestLeptonica_TestLeptonica_h

#include <CtrlLib/CtrlLib.h>

#include <PixRaster/PixRaster.h>
#include <PixRasterCtrl/PixRasterCtrl.h>

using namespace Upp;

#define LAYOUTFILE <TestLeptonica/TestLeptonica.lay>
#include <CtrlCore/lay.h>

#define CHECKR(call, msg) \
	if(!call) \
	{ \
		PromptOK(msg); \
		return;  \
	}

class TestLeptonica : public WithTestLeptonicaLayout<TopWindow>
{
	public:
		typedef TestLeptonica CLASSNAME;
		
	protected:
	
		// button handlers
		void onLineRemoval(void);
		void onPageLayout(void);
		void onBaseLine(void);
		void onPageMarkers(void);
		void onQuit(void);
		
		// PIX raster
		PixRaster pixRaster;
		
	public:
		TestLeptonica();
		
		// button handlers
		
};

#endif

