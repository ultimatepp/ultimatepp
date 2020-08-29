#include "SurfaceCtrl_Demo.h"
#include <Functions4U/Functions4U_Gui.h>

namespace Upp{
	SurfaceCtrl_Demo::SurfaceCtrl_Demo()
	{
		CtrlLayout(*this, "SurfaceCtrl demo. STL viewer");
		filename.SetText( GetFileDirectory(__FILE__) + "Stanford_Bunny_sample.stl" );
			
		cameraView <<= 0;
		cameraView.WhenAction = [&]{
			if(cameraView == 0){
				canvas.GetCamera().SetCameraType(CT_PERSPECTIVE);
			}else if(cameraView == 1){
				canvas.GetCamera().SetCameraType(CT_ORTHOGRAPHIC);
			}else{
				canvas.GetCamera().SetCameraType(CT_PERSPECTIVE);
			}
			canvas.Refresh();
		};
		axis = canvas.IsAxisEnable();
		axis.WhenAction = [&]{
			canvas.ShowAxis(axis.Get());
			canvas.Refresh();
		};
		
		fovSelector <<= canvas.GetCamera().GetFOV();
		fovSelector.MinMax(0,160);
		fovSelector.WhenAction =[&]{
			canvas.GetCamera().SetFOV(fovSelector);
			canvas.Refresh();
		};
		
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
		
		camPosX <<= THISBACK(UpdatePosition);
		camPosY <<= THISBACK(UpdatePosition);
		camPosZ <<= THISBACK(UpdatePosition);
		
		quatW <<= THISBACK(UpdateRotation);
		quatX <<= THISBACK(UpdateRotation);
		quatY <<= THISBACK(UpdateRotation);
		quatZ <<= THISBACK(UpdateRotation);
		
		resetPos.WhenAction = [&]{
			canvas.GetCamera().GetTransform().SetPosition(0,0,20);
			canvas.GetCamera().GetTransform().SetRotation(0,glm::vec3(0,0,0));
			canvas.Refresh();
		};
		
		canvas.WhenPaint = [&] {
			canvas.DrawAllObjects();
			RetrieveCameraInformation();
		};
		Sizeable().Zoomable();
	}
		
	bool SurfaceCtrl_Demo::Key(dword key, int count){
		return canvas.Key(key,count);
	}

	void SurfaceCtrl_Demo::RetrieveCameraInformation(){
		Transform&  t = canvas.GetCamera().GetTransform();
		camPosX = t.GetPosition().x;
		camPosY = t.GetPosition().y;
		camPosZ = t.GetPosition().z;
		glm::quat q = t.GetRotation();
		quatW = q.w;
		quatX = q.x;
		quatY = q.y;
		quatZ = q.z;
		frontvec.SetLabel("("+ AsString( t.GetFront().x,2) +", " + AsString(t.GetFront().y,2) +", " + AsString(t.GetFront().z,2) + ")");
		upvec.SetLabel("("+ AsString(t.GetUp().x,2) +", " + AsString(t.GetUp().y,2) +", " + AsString(t.GetUp().z,2) + ")");
		rightvec.SetLabel("("+ AsString(t.GetRight().x,2) +", " + AsString(t.GetRight().y,2) +", " + AsString(t.GetRight().z,2) + ")");
		
	}
	void SurfaceCtrl_Demo::UpdatePosition(){
		canvas.GetCamera().GetTransform().SetPosition(camPosX,camPosY,camPosZ);
	}
	void SurfaceCtrl_Demo::UpdateRotation(){
		canvas.GetCamera().GetTransform().SetRotation(glm::quat(quatW,quatX,quatY,quatZ));
	}
}

GUI_APP_MAIN
{
	Upp::ConsoleOutput con(true);
	Upp::StdLogSetup( Upp::LOG_COUT | Upp::LOG_FILE);
	Upp::SurfaceCtrl_Demo().Run();
}

