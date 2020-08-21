#include "SurfaceCtrl_Demo.h"


namespace Upp{
	SurfaceCtrl_Demo::SurfaceCtrl_Demo()
	{
		CtrlLayout(*this, "SurfaceCtrl demo. STL viewer");
		filename.SetText( GetFileDirectory(__FILE__) + "Stanford_Bunny_sample.stl" );
		
		butOpen.WhenAction = [&] {
			try {
				Object3D& obj = canvas.CreateObject();
				obj.LoadStl(~filename,Gray());
				obj.GetTransform().Rotate(-90.0f,glm::vec3(1.0f,0.0f,0.0f));
				obj.GetTransform().SetScale(glm::vec3(0.1f,0.1f,0.1f));
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
	
	bool SurfaceCtrl_Demo::Key(dword key, int count){
		return canvas.Key(key,count);
	}
}

GUI_APP_MAIN
{
	Upp::StdLogSetup( Upp::LOG_COUT | Upp::LOG_FILE);
	Upp::SurfaceCtrl_Demo().Run();
}

