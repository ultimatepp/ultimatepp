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
		Axis.Draw(camera.GetProjectionMatrix(Upp::Sizef{sizeW,sizeH}), camera.GetViewMatrix(),camera.GetTransform().GetPosition(), DrawMeshNoLight,DrawMeshNoLight,DrawMeshNoLight,DrawMeshNoLight);
	if(ShowCameraFocus){
		CameraFocus.GetTransform().SetPosition(camera.focus);
		CameraFocus.Draw(camera.GetProjectionMatrix(Upp::Sizef{sizeW,sizeH}), camera.GetViewMatrix(),camera.GetTransform().GetPosition(), DrawMeshNoLight,DrawMeshNoLight,DrawMeshNoLight,DrawMeshNoLight);
	}
}
void SurfaceCtrl::CreateObject(Surface& surf, Color color)noexcept{
	Object3D& obj = allObjects.Create(surf,color);
	obj.GetTransform().Rotate(-90.0f,glm::vec3(1.0f,0.0f,0.0f));
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
		obj.Draw(camera.GetProjectionMatrix(Upp::Sizef{sizeW,sizeH}), camera.GetViewMatrix(),camera.GetTransform().GetPosition(), DrawMeshNoLight,DrawMeshLight,DrawMeshLine,DrawMeshNormal );
	}
}
void SurfaceCtrl::InitCamera()noexcept{
	camera.Init();
	camera.SetMouseSensitivity(0.80f);
	camera.SetMouvementSpeed(0.09f);
}
void SurfaceCtrl::GLResize(int w, int h){
	sizeW = w;
	sizeH = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	Refresh();
}
bool SurfaceCtrl::Key(dword key,int count){
	if( key == K_W){
		camera.ProcessKeyboardMouvement(CM_FORWARD);
	}
	if( key == K_S){
		camera.ProcessKeyboardMouvement(CM_BACKWARD);
	}
	if( key == K_Q){
		camera.ProcessKeyboardMouvement(CM_LEFT);
	}
	if( key == K_D){
		camera.ProcessKeyboardMouvement(CM_RIGHT);
	}
	if( key == K_R){
	/*	float data[] = { 230.0f/255.0f, 140.0f/255.0f, 30.0f/255.0f};
		if(allObjects.GetCount() > 0) allObjects[0].UpdateColors(0,allObjects[0].GetSurfaceCount()/2,data);*/
		camera.GetTransform().SetPosition(0,0,camera.GetTransform().GetPosition().z);
		camera.GetTransform().SetRotation(0.0f,0.0f,0.0f);
		
	}
	if( key == K_T){
		if(allObjects.GetCount() > 0) DUMP(allObjects[0].ReadColors(0,1));
	}
	if( key == K_L){
		if(allObjects.GetCount() > 0) allObjects[0].ShowMeshLine(!allObjects[0].GetShowMeshLine());
	}
	if( key == K_M){
		if(allObjects.GetCount() > 0) allObjects[0].ShowMesh(!allObjects[0].GetShowMesh());
	}
	if( key == K_P){
		if(allObjects.GetCount() > 0) allObjects[0].ShowLight(!allObjects[0].GetShowLight());
	}
	if( key == K_N){
		if(allObjects.GetCount() > 0) allObjects[0].ShowMeshNormal(!allObjects[0].GetShowMeshNormal());
	}
	if(key == K_LEFT){
		camera.ProcessMouseWheelMouvement(2 * camera.GetMouvementSpeed(),0);
	}
	if(key == K_RIGHT){
		camera.ProcessMouseWheelMouvement(-2 * camera.GetMouvementSpeed(),0);
	}
	if(key == K_UP){
		camera.ProcessMouseWheelMouvement(0,2 * camera.GetMouvementSpeed());
	}
	if(key == K_DOWN){
		camera.ProcessMouseWheelMouvement(0,-2 * camera.GetMouvementSpeed());
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
		if(e == 1 && allObjects.GetCount() > 0){
			camera.focus = allObjects[0].GetTransform().GetPosition();
		}
		e++;
		if(e == 2) e = 0;
	}
	if(key == K_A){
		ShowAxis = !ShowAxis;
	}
	if(key == K_F){
		ShowCameraFocus = !ShowCameraFocus;
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
	Refresh();
	return true;
}
void SurfaceCtrl::MouseMove(Point p, dword keyflags){
	if(camera.MouseMiddlePressed || camera.MouseLeftPressed){
		camera.ProcessMouveMouvement(p.x - camera.StartPress.x,p.y - camera.StartPress.y);
		Refresh();
	}
	camera.forceZoom = keyflags & K_CTRL;
	camera.StartPress = p;
}
void SurfaceCtrl::MouseWheel(Point p,int zdelta,dword keyflags){
	camera.ProcessMouseScroll(zdelta);
	Refresh();
}
void SurfaceCtrl::LeftDown(Point p, dword){
	camera.StartPress = p;
	camera.MouseLeftPressed = true;
	return;
}
void SurfaceCtrl::LeftUp(Point p, dword){
	camera.MouseLeftPressed = false;
	return;
}
void SurfaceCtrl::MiddleDown(Point p, dword keyflags){
	camera.MouseMiddlePressed = true;
	camera.StartPress = p;
}
void SurfaceCtrl::MiddleUp(Point p, dword keyflags){
	camera.MouseMiddlePressed = false;
}
void SurfaceCtrl::MouseLeave(){
	camera.MouseMiddlePressed = false;
	camera.MouseLeftPressed = false;
	return;
}
}