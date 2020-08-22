#include "SurfaceCtrl_Demo.h"


namespace Upp{
	SurfaceCtrl_Demo::SurfaceCtrl_Demo()
	{
		CtrlLayout(*this, "SurfaceCtrl demo. STL viewer");
		filename.SetText( GetFileDirectory(__FILE__) + "Stanford_Bunny_sample.stl" );
		
		butOpen.WhenAction = [&] {
			try {
				Surface surf;
				{
					Object3D& obj = canvas.CreateObject();
					obj.LoadModel(~filename);
					surf = obj.GetSurface();
					canvas.DeleteObject(obj.GetID());
				}
				Object3D& obj = canvas.CreateObject();
				obj.LoadSurface(surf, Green());
				obj.GetTransform().Rotate(90.0f,glm::vec3(1.0f,0.0f,0.0f));
				obj.GetTransform().SetScale(glm::vec3(0.1f,0.1f,0.1f));
				obj.SetLineWidth(2.0f);
				canvas.SetDefaultShader(obj);
			
			
			/*
				Object3D& obj2 = canvas.CreateObject();
				obj2.LoadModel("C:\\Users\\hamon\\Downloads\\boat_v2_L2.123cb2c2d48f-7519-476c-8274-b5bcd578c751\\cube.obj");
				obj2.GetTransform().Rotate(-90.0f,glm::vec3(1.0f,0.0f,0.0f));
				//obj2.GetTransform().SetScale(glm::vec3(0.01f,0.01f,0.01f));
				obj2.SetLineWidth(2.0f);
				canvas.SetDefaultShader(obj2);
				*/
				
				canvas.Refresh();
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

