#include "SurfaceCtrl_Demo.h"


namespace Upp{
	SurfaceCtrl_Demo::SurfaceCtrl_Demo()
	{
		CtrlLayout(*this, "SurfaceCtrl demo. STL viewer");
		filename.SetText( GetFileDirectory(__FILE__) + "Stanford_Bunny_sample.stl" );
		ActiveFocus(canvas);
		butOpen.WhenAction = [&] {
			try {
				bool isText;
				String header;
				Surface surf;
				LoadStl(~filename, surf, isText, header);
				surf.GetPanelParams();
				surf.GetLimits();
				surf.GetSurface();
				surf.GetVolume();
				canvas.CreateObject(surf,Gray());
			} catch (Exc e) {
				Exclamation(DeQtf(e));
			}
		};
		canvas.WhenPaint = [&] {
			canvas.DrawAllObjects();
		};
		Sizeable().Zoomable();
	}
	
	void SurfaceCtrl_Demo::Layout(){
		canvas.GLResize(GetSize().cx,GetSize().cy);
	}
}

GUI_APP_MAIN
{
	Upp::StdLogSetup( Upp::LOG_COUT | Upp::LOG_FILE);
	Upp::SurfaceCtrl_Demo().Run();
}

