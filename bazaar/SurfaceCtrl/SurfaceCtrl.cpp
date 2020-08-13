#include "SurfaceCtrl.h"

namespace Upp{
SurfaceCtrl::SurfaceCtrl(){
	InitCamera();
	OnBegin = [&]{InitOpenGLFeatures();};
}
SurfaceCtrl::~SurfaceCtrl(){
	OnEnd();
}
void SurfaceCtrl::InitShader(){
	DrawMeshNoLight.AttachShader(OpenGLShader(GL_VERTEX_SHADER,
		#include "shaders/VertexSimple.glsl"
	)).AttachShader(OpenGLShader(GL_FRAGMENT_SHADER,
		#include "shaders/FragmentNoLight.glsl"
	)).Link();
	
	DrawMeshLight.AttachShader(OpenGLShader(GL_VERTEX_SHADER,
		#include "shaders/VertexSimple.glsl"
	)).AttachShader(OpenGLShader(GL_FRAGMENT_SHADER,
		#include "shaders/FragmentLightTesselated.glsl"
	)).AttachShader(OpenGLShader(GL_TESS_CONTROL_SHADER,
		#include "shaders/TesselationControlTriangles.glsl"
	)).AttachShader(OpenGLShader(GL_TESS_EVALUATION_SHADER,
		#include "shaders/TesselationEvaluationTriangles.glsl"
	)).AttachShader(OpenGLShader(GL_GEOMETRY_SHADER,
		#include "shaders/GeometrySimple.glsl"
	)).Link();
		
	DrawMeshLine.AttachShader(OpenGLShader(GL_VERTEX_SHADER,
		#include "shaders/VertexSimple.glsl"
	)).AttachShader(OpenGLShader(GL_GEOMETRY_SHADER,
		#include "shaders/GeometryTriangleTesselated.glsl"
	)).AttachShader(OpenGLShader(GL_FRAGMENT_SHADER,
		#include "shaders/FragmentBlackTesselated.glsl"
	)).Link();
	
	DrawMeshNormal.AttachShader(OpenGLShader(GL_VERTEX_SHADER,
		#include "shaders/Vertex.glsl"
	)).AttachShader(OpenGLShader(GL_GEOMETRY_SHADER,
		#include "shaders/GeometryNormal.glsl"
	)).AttachShader(OpenGLShader(GL_FRAGMENT_SHADER,
		#include "shaders/FragmentBlack.glsl"
	)).Link();
}
void SurfaceCtrl::InitOpenGLFeatures()noexcept{
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);//Gestion of alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Gestion de l'alpha sur les textures
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO); //Gestion pour le RGB uniquement
	InitShader();
}
void SurfaceCtrl::GLPaint(){
	if(!loaded){
		OnBegin();
		Axis = objProvider.Begin(GL_LINES).AddAxis(0,0,0,200000).End();
		CameraFocus = objProvider.Begin(GL_TRIANGLE_FAN).AddCube(0.0f,0.0f,0.0f,1,LtYellow()).End();
		loaded = true;
	}
	MemoryIgnoreLeaksBlock __;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	WhenPaint(); //The function wich loop arround all object and draw using proper VAO and shaders
	
	if(ShowAxis)
		Axis.Draw(camera.GetProjectionMatrix(), camera.GetViewMatrix(),camera.GetTransform().GetPosition(), DrawMeshNoLight,DrawMeshNoLight,DrawMeshNoLight,DrawMeshNoLight);
	if(ShowCameraFocus){
		if(allObjects.GetCount() > 0){
			CameraFocus.GetTransform().SetPosition(camera.GetFocus());
			CameraFocus.Draw(camera.GetProjectionMatrix(), camera.GetViewMatrix(),camera.GetTransform().GetPosition(), DrawMeshNoLight,DrawMeshNoLight,DrawMeshNoLight,DrawMeshNoLight);
		}
	}
}
void SurfaceCtrl::CreateObject(Surface& surf, Color color)noexcept{
	Object3D& obj = allObjects.Create<Object3D>(surf,color);
	obj.GetTransform().Rotate(-90.0f,glm::vec3(1.0f,0.0f,0.0f));
	obj.GetTransform().SetScale(glm::vec3(0.5f,0.5f,0.5f));
	//obj.GetTransform().Move(50.0f,10.0f,0.0f);
	obj.SetVolumeEnvelope(surf.env);
	obj.SetLineWidth(2.0f);
	ZoomToFit();
	Refresh();
}
void SurfaceCtrl::ZoomToFit()noexcept{
	double mxGlobal=0;
	for(Object3D& obj : allObjects){
		if(obj.GetVolumeEnvelope()){
			VolumeEnvelope env = *obj.GetVolumeEnvelope();
			double mx = max(max(env.maxX, env.maxY), env.maxZ);
			if(mx > mxGlobal) mxGlobal = mx;
		}
	}
	glm::vec3 camPos = camera.GetTransform().GetPosition();
	if(camPos.x > camPos.y  && camPos.x > camPos.z) camera.GetTransform().SetPosition(glm::vec3((float)mxGlobal,camPos.y,camPos.z));
	if(camPos.y > camPos.z  && camPos.y > camPos.x) camera.GetTransform().SetPosition(glm::vec3(camPos.x,(float)mxGlobal,camPos.z));
	if(camPos.z > camPos.x  && camPos.z > camPos.y) camera.GetTransform().SetPosition(glm::vec3(camPos.x,camPos.y,(float)mxGlobal));
	camera.GetTransform().SetRotation(0.0f,0.0f,0.0f);
}
void SurfaceCtrl::DrawAllObjects(){
	for(Object3D& obj : allObjects){
		obj.Draw(camera.GetProjectionMatrix(), camera.GetViewMatrix(),camera.GetTransform().GetPosition(), DrawMeshNoLight,DrawMeshLight,DrawMeshLine,DrawMeshNormal );
	}
}
void SurfaceCtrl::InitCamera()noexcept{
	camera.Init();
	camera.SetMouseSensitivity(0.2f);
	camera.SetMouvementSpeed(0.09f);
	camera.SetAllObjects(allObjects);
}
void SurfaceCtrl::GLResize(int w, int h){
	sizeW = w;
	sizeH = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	camera.SetScreenSize(w,h);
	Refresh();
}
bool SurfaceCtrl::Key(dword key,int count){
	if( key == K_Z){
		for(Object3D* obj : SelectedObject)
			obj->GetTransform().Move(camera.GetTransform().GetUp());
		camera.CenterFocus(SelectedObject);
	}
	if( key == K_S){
		for(Object3D* obj : SelectedObject)
			obj->GetTransform().Move(-(camera.GetTransform().GetUp()));
		camera.CenterFocus(SelectedObject);
	}
	if( key == K_Q){
		for(Object3D* obj : SelectedObject)
			obj->GetTransform().Move(-(camera.GetTransform().GetRight()));
		camera.CenterFocus(SelectedObject);
	}
	if( key == K_D){
		for(Object3D* obj : SelectedObject)
			obj->GetTransform().Move(camera.GetTransform().GetRight());
		camera.CenterFocus(SelectedObject);
	}
	if( key == K_R){
	/*	float data[] = { 230.0f/255.0f, 140.0f/255.0f, 30.0f/255.0f};
		if(allObjects.GetCount() > 0) allObjects[0].UpdateColors(0,allObjects[0].GetSurfaceCount()/2,data);*/
		camera.GetTransform().SetPosition(0,0,camera.GetTransform().GetPosition().z);
		camera.GetTransform().SetRotation(0.0f,0.0f,0.0f);
		
	}

	if( key == K_L){
		for(Object3D* obj : SelectedObject)
			obj->ShowMeshLine(!obj->GetShowMeshLine());
	}
	if( key == K_M){
		for(Object3D* obj : SelectedObject)
			obj->ShowMesh(!obj->GetShowMesh());
	}
	if( key == K_P){
		for(Object3D* obj : SelectedObject)
			obj->ShowLight(!obj->GetShowLight());
	}
	if( key == K_N){
		for(Object3D* obj : SelectedObject)
			obj->ShowMeshNormal(!obj->GetShowMeshNormal());
	}
	
	
	if(key == K_ADD){
		camera.SetFOV(camera.GetFOV() + 5);
	}
	if(key == K_SUBTRACT){
		camera.SetFOV(camera.GetFOV() - 5);
	}
	
	if(key == K_C){
		static unsigned short e = 0;
		camera.SetCameraType((CameraType)e);
		e++;if(e == 2) e = 0;
	}
	
	if(key == K_A){
		ShowAxis = !ShowAxis;
	}
	if(key == K_F){
		ShowCameraFocus = !ShowCameraFocus;
	}
	
	if(key == K_X){
		Cout() << "Current camera Position : " << camera.GetTransform().GetPosition().x << "," << camera.GetTransform().GetPosition().y << "," << camera.GetTransform().GetPosition().z << EOL;
	//	Cout() << "Current dezoom factor : " << camera.GetDezoomFactor() << EOL;
		Cout() << "Current quaterion : " << camera.GetTransform().GetRotation().w << "," << camera.GetTransform().GetRotation().x << "," << camera.GetTransform().GetRotation().y << "," << camera.GetTransform().GetRotation().z << EOL;
		Cout() << "Up vector : "<< camera.GetTransform().GetUp().x << "," << camera.GetTransform().GetUp().y << "," << camera.GetTransform().GetUp().z << EOL;
		Cout() << "Right vector : "<< camera.GetTransform().GetRight().x << "," << camera.GetTransform().GetRight().y << "," << camera.GetTransform().GetRight().z << EOL;
		Cout() << "Front vector : "<< camera.GetTransform().GetFront().x << "," << camera.GetTransform().GetFront().y << "," << camera.GetTransform().GetFront().z << EOL;
	}
	/*
		Material change
	*/
	if(key == K_U){ //Increase diffuse
		if(allObjects.GetCount() > 0){
			allObjects[0].GetMaterial().SetDiffuse(allObjects[0].GetMaterial().GetDiffuse() + glm::vec3(0.1f,0.1f,0.1f));
		}
	}
	if(key == K_I){ //decrease diffuse
		if(allObjects.GetCount() > 0){
			allObjects[0].GetMaterial().SetDiffuse(allObjects[0].GetMaterial().GetDiffuse() - glm::vec3(0.1f,0.1f,0.1f));
		}
	}
	if(key == K_J){ //Increase specular
		if(allObjects.GetCount() > 0){
			allObjects[0].GetMaterial().SetSpecular(allObjects[0].GetMaterial().GetSpecular() + glm::vec3(0.1f,0.1f,0.1f));
			LOG(allObjects[0].GetMaterial().GetSpecular().x + AsString(" ") + allObjects[0].GetMaterial().GetSpecular().y + AsString(" ") + allObjects[0].GetMaterial().GetSpecular().z);
		}
	}
	if(key == K_K){ //decrease specular
		if(allObjects.GetCount() > 0){
			allObjects[0].GetMaterial().SetSpecular(allObjects[0].GetMaterial().GetSpecular() - glm::vec3(0.1f,0.1f,0.1f));
			LOG( AsString(allObjects[0].GetMaterial().GetSpecular().x) + " " + AsString(allObjects[0].GetMaterial().GetSpecular().y) + " " + AsString(allObjects[0].GetMaterial().GetSpecular().z));
		}
	}
	if(key == K_Y){ //Increase Shininess
		if(allObjects.GetCount() > 0){
			allObjects[0].GetMaterial().SetShininess(allObjects[0].GetMaterial().GetShininess() + 0.1f);
			LOG(allObjects[0].GetMaterial().GetShininess());
		}
	}
	if(key == K_H){ //decrease Shininess
		if(allObjects.GetCount() > 0){
			allObjects[0].GetMaterial().SetShininess(allObjects[0].GetMaterial().GetShininess() - 0.1f);
			LOG(allObjects[0].GetMaterial().GetShininess());
		}
	}
	
	if(key == K_ESCAPE){ //removing all selection
		for(Object3D* obj : SelectedObject){
			obj->ShowBoundingBox(false);
		}
		SelectedObject.Clear();
		camera.ResetFocus();
	}
	Refresh();
	return true;
}
void SurfaceCtrl::MouseMove(Point p, dword keyflags){
	if(camera.MouseMiddlePressed || camera.MouseLeftPressed){
		camera.ProcessMouveMouvement(p.x - camera.lastPress.x,p.y - camera.lastPress.y);
		Refresh();
	}
	
	camera.lastPress = p;
}
void SurfaceCtrl::MouseWheel(Point p,int zdelta,dword keyflags){
//	camera.forceZoom = keyflags & K_CTRL;
	camera.ProcessMouseScroll(zdelta);
	Refresh();
}
void SurfaceCtrl::LeftDown(Point p, dword){
	camera.lastPress = p;
	camera.MouseLeftPressed = true;
	Object3D* obj = camera.ProcessMouseLeftClick(p.x,p.y);
	if(obj != nullptr){
		bool trouver = false;
		int e = 0;
		for(Object3D* ob : SelectedObject){
			if( obj == ob){
				trouver = true;
				obj->ShowBoundingBox(false);
				SelectedObject.Remove(e,1);
				break;
			}
			e++;
		}
		if(!trouver){
			obj->ShowBoundingBox(true);
			SelectedObject.Add(obj);
		}
		camera.CenterFocus(SelectedObject);
	}else{
		for(Object3D* obj : SelectedObject){
			obj->ShowBoundingBox(false);
		}
		SelectedObject.Clear();
		camera.ResetFocus();
	}
	
	Refresh();
}
void SurfaceCtrl::LeftUp(Point p, dword){
	camera.MouseLeftPressed = false;
}
void SurfaceCtrl::MiddleDown(Point p, dword keyflags){
	camera.MouseMiddlePressed = true;
	camera.ShiftPressed = keyflags & K_SHIFT;
	camera.lastPress = p;
}
void SurfaceCtrl::MiddleUp(Point p, dword keyflags){
	camera.MouseMiddlePressed = false;
	camera.ShiftPressed = false;
}
void SurfaceCtrl::MouseLeave(){
	camera.MouseMiddlePressed = false;
	camera.MouseLeftPressed = false;
	camera.ShiftPressed = false;
	return;
}
}