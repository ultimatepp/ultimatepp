#include "SurfaceCtrl_Demo.h"


namespace Upp{
	SurfaceCtrl_Demo::SurfaceCtrl_Demo()
	{
		CtrlLayout(*this, "SurfaceCtrl demo. STL viewer");
		filename.SetText( GetFileDirectory(__FILE__) + "Stanford_Bunny_sample.stl" );
				
		butOpen.WhenAction = [&] {
			try {

				Object3D& obj = canvas.CreateObject();
				obj.LoadModel(~filename).Init();

				obj.GetTransform().Rotate(90.0f,glm::vec3(1.0f,0.0f,0.0f));
				obj.GetTransform().SetScale(glm::vec3(1.0f,1.0f,1.0f));
				obj.SetLineWidth(2.0f);
				
				canvas.SetDefaultShader(obj);
				
				canvas.Refresh();
			} catch (Exc e) {
				Exclamation(DeQtf(e));
			}
		};
		OpenSurface.WhenAction = [&]{
			try {
				Object3D& obj = canvas.CreateObject();
				obj.LoadModel(GetFileDirectory(__FILE__) + "landscape/landscape.fbx").AttachTexture(obj.InsertTexture(GetFileDirectory(__FILE__) + "landscape/landscape.jpg"),0,obj.GetMeshes().GetCount()).Init();
				canvas.SetDefaultShader(obj);
				canvas.Refresh();
			} catch (Exc e) {
				Exclamation(DeQtf(e));
			}
		};
		OpenEarth.WhenAction = [&]{
			try {
				Object3D& obj = canvas.CreateObject();
				obj.LoadModel(GetFileDirectory(__FILE__) + "earth/earth.obj").AttachTexture(obj.InsertTexture(GetFileDirectory(__FILE__) + "earth/earth.png",-1,FLIP_MIRROR_VERT),0,obj.GetMeshes().GetCount()).Init();
				obj.GetTransform().Rotate(-180,glm::vec3(1.0f,0.0f,0.0f));
				canvas.SetDefaultShader(obj);
				canvas.Refresh();
			} catch (Exc e) {
				Exclamation(DeQtf(e));
			}
		};
		Ultimate.WhenAction = [&]{
			try {
				Object3D& obj = canvas.CreateObject();
				obj.LoadModel(GetFileDirectory(__FILE__) + "ultimate/upp.obj").Init();
				canvas.SetDefaultShader(obj);
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

