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
			if(axis.Get())
				depthaxis.Enable();
			else
				depthaxis.Disable();
			canvas.Refresh();
		};
		depthaxis = canvas.IsDepthAxisEnable();
		depthaxis.WhenAction = [&]{
			canvas.UseDepthAxis(depthaxis.Get());
			canvas.Refresh();
		};
		
		skybox = canvas.GetSkybox().IsShown();  
		skybox.WhenAction = [&]{
			if(!canvas.GetSkybox().IsLoaded()){
				canvas.ExecuteGL([&]{
				canvas.GetSkybox().Init(
				StreamRaster::LoadFileAny( GetSurfaceCtrlDirectory() + "skybox/right.jpg"),
				StreamRaster::LoadFileAny( GetSurfaceCtrlDirectory() + "skybox/left.jpg"),
				StreamRaster::LoadFileAny( GetSurfaceCtrlDirectory() + "skybox/top.jpg"),
				StreamRaster::LoadFileAny( GetSurfaceCtrlDirectory() + "skybox/bottom.jpg"),
				StreamRaster::LoadFileAny( GetSurfaceCtrlDirectory() + "skybox/front.jpg"),
				StreamRaster::LoadFileAny( GetSurfaceCtrlDirectory() + "skybox/back.jpg"));
				});
			}
			canvas.GetSkybox().Show(skybox.Get());
			canvas.Refresh();
		};
		
		fovSelector <<= canvas.GetCamera().GetFOV();
		fovSelector.MinMax(0,160);
		fovSelector.WhenAction =[&]{
			canvas.GetCamera().SetFOV(float(fovSelector));
			canvas.Refresh();
		};
		
		ShowFocus = canvas.IsCameraFocusShow();
		ShowFocus.WhenAction = [&]{
			canvas.ShowCameraFocus(ShowFocus.Get());
			canvas.Refresh();
		};
		
		butOpen.WhenAction = [&] {
			try {
				canvas.ExecuteGL([&]{
				Object3D& obj = canvas.CreateObject();
				obj.LoadModel(~filename).Init();
				obj.GetTransform().Rotate(90.0f,glm::vec3(1.0f,0.0f,0.0f));
				obj.GetTransform().SetScale(glm::vec3(1.0f,1.0f,1.0f));
				obj.SetLineWidth(2.0f);
				canvas.SetDefaultShader(obj);
				canvas.ZoomToFit();
				});
				canvas.Refresh();
			} catch (Exc e) {
				Exclamation(DeQtf(e));
			}
		};
		OpenEarth.WhenAction = [&]{
			try {
				canvas.ExecuteGL([&]{
					Object3D& obj = canvas.CreateObject();
					obj.LoadModel(GetFileDirectory(__FILE__) + "earth/earth.obj").AttachTexture(obj.InsertTexture(GetFileDirectory(__FILE__) + "earth/earth.png",-1,FLIP_MIRROR_VERT),0,obj.GetMeshes().GetCount()).Init();
					obj.GetTransform().Rotate(-180,glm::vec3(1.0f,0.0f,0.0f));
					canvas.SetDefaultShader(obj);
					canvas.ZoomToFit();
				});
				canvas.Refresh();
			} catch (Exc e) {
				Exclamation(DeQtf(e));
			}
		};
		Ultimate.WhenAction = [&]{
			try {
				canvas.ExecuteGL([&]{
					Object3D& obj = canvas.CreateObject();
					obj.LoadModel(GetFileDirectory(__FILE__) + "ultimate/upp.obj").Init();
					canvas.SetDefaultShader(obj);
					canvas.ZoomToFit();
				});
				canvas.Refresh();
			} catch (Exc e) {
				Exclamation(DeQtf(e));
			}
		};
		
		zoomToFit.WhenAction = [&]{
			canvas.ZoomToFit();
			canvas.Refresh();
		};
		
		camPosX <<= THISBACK(UpdatePosition);
		camPosY <<= THISBACK(UpdatePosition);
		camPosZ <<= THISBACK(UpdatePosition);
		
		quatW <<= THISBACK(UpdateRotation);
		quatX <<= THISBACK(UpdateRotation);
		quatY <<= THISBACK(UpdateRotation);
		quatZ <<= THISBACK(UpdateRotation);
		
		focusX <<= THISBACK(UpdateFocus);
		focusY <<= THISBACK(UpdateFocus);
		focusZ <<= THISBACK(UpdateFocus);
		
		ObjPosX <<= THISBACK(UpdateObjectInformation);
		ObjPosY <<= THISBACK(UpdateObjectInformation);
		ObjPosZ <<= THISBACK(UpdateObjectInformation);
		ObjQuatW <<= THISBACK(UpdateObjectInformation);
		ObjQuatX <<= THISBACK(UpdateObjectInformation);
		ObjQuatY <<= THISBACK(UpdateObjectInformation);
		ObjQuatZ <<= THISBACK(UpdateObjectInformation);
		ObjScalX <<= THISBACK(UpdateObjectInformation);
		ObjScalY <<= THISBACK(UpdateObjectInformation);
		ObjScalZ <<= THISBACK(UpdateObjectInformation);

		ObjShowSkin.WhenAction = [&]{
			if(TreeIDSelected > 0){
				int iterator = canvas.FindObject(TreeIDSelected);
				if( iterator != -1){
					Object3D& obj = canvas.GetObject(TreeIDSelected);
					obj.ShowMesh(ObjShowSkin.Get());
					canvas.Refresh();
				}
			}
		};
		ObjShowLine.WhenAction = [&]{
			if(TreeIDSelected > 0){
				int iterator = canvas.FindObject(TreeIDSelected);
				if( iterator != -1){
					Object3D& obj = canvas.GetObject(TreeIDSelected);
					obj.ShowMeshLine(ObjShowLine.Get());
					canvas.Refresh();
				}
			}
		};
		ObjShowNormal.WhenAction = [&]{
			if(TreeIDSelected > 0){
				int iterator = canvas.FindObject(TreeIDSelected);
				if( iterator != -1){
					Object3D& obj = canvas.GetObject(TreeIDSelected);
					obj.ShowMeshNormal(ObjShowNormal.Get());
					canvas.Refresh();
				}
			}
		};
		ObjShowLight.WhenAction = [&]{
			if(TreeIDSelected > 0){
				int iterator = canvas.FindObject(TreeIDSelected);
				if( iterator != -1){
					Object3D& obj = canvas.GetObject(TreeIDSelected);
					obj.ShowLight(ObjShowLight.Get());
					canvas.Refresh();
				}
			}
		};
		
		resetPos.WhenAction = [&]{
			canvas.GetCamera().GetTransform().SetPosition(0,0,20);
			canvas.GetCamera().GetTransform().SetRotation(0,glm::vec3(0,0,0));
			canvas.Refresh();
		};
		
		canvas.WhenPaint = [&] {
			canvas.DrawAllObjects();
			RetrieveCameraInformation();
			RetrieveObjectInformation();
		};
		
		AllObjects.WhenLeftClick = [&]{
			if(AllObjects.GetSel().GetCount() > 0){
				int sel = atoi(AllObjects.Get(AllObjects.GetSel()[0]).ToString());
				TreeIDSelected = sel;
				RetrieveObjectInformation();
			}
		};
		
		Sizeable().Zoomable();
	}
		
	bool SurfaceCtrl_Demo::Key(dword key, int count){
		return canvas.Key(key,count);
	}

	void SurfaceCtrl_Demo::RetrieveCameraInformation(){
		Transform&  t = canvas.GetCamera().GetTransform();
		camPosX <<= t.GetPosition().x;
		camPosY <<= t.GetPosition().y;
		camPosZ <<= t.GetPosition().z;
		glm::quat q = t.GetRotation();
		quatW <<= q.w;
		quatX <<= q.x;
		quatY <<= q.y;
		quatZ <<= q.z;
		frontvec.SetLabel("("+ AsString( t.GetFront().x,2) +", " + AsString(t.GetFront().y,2) +", " + AsString(t.GetFront().z,2) + ")");
		upvec.SetLabel("("+ AsString(t.GetUp().x,2) +", " + AsString(t.GetUp().y,2) +", " + AsString(t.GetUp().z,2) + ")");
		rightvec.SetLabel("("+ AsString(t.GetRight().x,2) +", " + AsString(t.GetRight().y,2) +", " + AsString(t.GetRight().z,2) + ")");
		
		focusX <<= canvas.GetCamera().GetFocus().x;
		focusY <<= canvas.GetCamera().GetFocus().y;
		focusZ <<= canvas.GetCamera().GetFocus().z;
		onObjectBool.SetLabel( AsString(canvas.GetCamera().IsOnObject()));

		AllObjects.Clear();
		AllObjects.SetRoot(Image(), "Application");
		int i = AllObjects.Add(0,Image(),"All Objects");
		for(const Object3D& obj : canvas.GetAllObjects()){
			int e = AllObjects.Add(i,Image(), AsString(obj.GetID()));
			AllObjects.Add(e, Image(), "Position: " + AsString(obj.GetTransform().GetPosition().x,2) +", " + AsString(obj.GetTransform().GetPosition().y,2) +", " + AsString(obj.GetTransform().GetPosition().z,2));
			AllObjects.Add(e, Image(), "Rotation: " + AsString(obj.GetTransform().GetRotation().w,2) +", " + AsString(obj.GetTransform().GetRotation().x,2) +", " + AsString(obj.GetTransform().GetRotation().y,2)+", " + AsString(obj.GetTransform().GetRotation().z,2));
			AllObjects.Add(e, Image(), "Scale: " + AsString(obj.GetTransform().GetScale().x,2) +", " + AsString(obj.GetTransform().GetScale().y,2) +", " + AsString(obj.GetTransform().GetScale().z,2));
			AllObjects.Add(e, Image(), "Meshes count: " + AsString(obj.GetMeshes().GetCount()));
		}
		i = AllObjects.Add(0,Image(),"Selected Objects");
		for(const int& in : canvas.GetSelectedObject()){
			AllObjects.Add(i,Image(), AsString(in));
		}
		AllObjects.OpenDeep(0,true);
	}
	void SurfaceCtrl_Demo::UpdatePosition(){
		canvas.GetCamera().GetTransform().SetPosition(float(camPosX),float(camPosY),float(camPosZ));
	}
	void SurfaceCtrl_Demo::UpdateRotation(){
		canvas.GetCamera().GetTransform().SetRotation(glm::quat(float(quatW),float(quatX),float(quatY),float(quatZ)));
	}
	void SurfaceCtrl_Demo::UpdateFocus(){
		canvas.GetCamera().SetFocus(float(focusX),float(focusY),float(focusZ));
	}
	void SurfaceCtrl_Demo::RetrieveObjectInformation(){
		if(TreeIDSelected > 0){
			int iterator = canvas.FindObject(TreeIDSelected);
			if( iterator != -1){
				Object3D& obj = canvas.GetObject(TreeIDSelected);
				ObjShowSkin = obj.GetShowMesh();
				ObjShowNormal = obj.GetShowMeshNormal();
				ObjShowLine = obj.GetShowMeshLine();
				ObjShowLight = obj.GetShowLight();
				
				ObjPosX <<= obj.GetTransform().GetPosition().x;
				ObjPosY <<= obj.GetTransform().GetPosition().y;
				ObjPosZ <<= obj.GetTransform().GetPosition().z;
				
				ObjQuatW <<= obj.GetTransform().GetRotation().w;
				ObjQuatX <<= obj.GetTransform().GetRotation().x;
				ObjQuatY <<= obj.GetTransform().GetRotation().y;
				ObjQuatZ <<= obj.GetTransform().GetRotation().z;
				
				ObjScalX <<= obj.GetTransform().GetScale().x;
				ObjScalY <<= obj.GetTransform().GetScale().y;
				ObjScalZ <<= obj.GetTransform().GetScale().z;
			}
		}
	}
	void SurfaceCtrl_Demo::UpdateObjectInformation(){
		if(TreeIDSelected > 0){
			int iterator = canvas.FindObject(TreeIDSelected);
			if( iterator != -1){
				Object3D& obj = canvas.GetObject(TreeIDSelected);
				obj.GetTransform().SetPosition(float(ObjPosX),float(ObjPosY),float(ObjPosZ));
				obj.GetTransform().SetRotation(glm::quat(float(ObjQuatW),float(ObjQuatX),float(ObjQuatY),float(ObjQuatZ)));
				obj.GetTransform().SetScale(glm::vec3((float)ObjScalX,(float)ObjScalY,(float)ObjScalZ));
			}
		}
	}
}

GUI_APP_MAIN
{
	Upp::ConsoleOutput con(true);
	Upp::StdLogSetup( Upp::LOG_COUT | Upp::LOG_FILE);
	Upp::SurfaceCtrl_Demo().Run();
}

