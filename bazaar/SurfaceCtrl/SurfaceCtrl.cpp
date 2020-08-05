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
		Axis = objProvider.Begin(GL_LINES).AddAxis(0,0,0,20).End();
		loaded = true;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	WhenPaint(); //The function wich loop arround all object and draw using proper VAO and shaders
	if(ShowAxis)
		Axis.Draw(camera.GetProjectionMatrix(Upp::Sizef{sizeW,sizeH}), camera.GetViewMatrix(),camera.GetTransform().GetPosition(), DrawMeshNoLight,DrawMeshNoLight,DrawMeshNoLight,DrawMeshNoLight);
}
void SurfaceCtrl::CreateObject(Surface& surf, Color color)noexcept{
	Object3D& obj = allObjects.Create(surf,color);
	//obj.GetTransform().RotateFromAngles(-90.0f,glm::vec3(1.0f,0.0f,0.0f));
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
	if(camPos.x > camPos.y  && camPos.x > camPos.z) camera.GetTransform().SetNewPosition(glm::vec3((float)mxGlobal,camPos.y,camPos.z));
	if(camPos.y > camPos.z  && camPos.y > camPos.x) camera.GetTransform().SetNewPosition(glm::vec3(camPos.x,(float)mxGlobal,camPos.z));
	if(camPos.z > camPos.x  && camPos.z > camPos.y) camera.GetTransform().SetNewPosition(glm::vec3(camPos.x,camPos.y,(float)mxGlobal));
}
void SurfaceCtrl::DrawAllObjects(){
	for(Object3D& obj : allObjects){
		obj.Draw(camera.GetProjectionMatrix(Upp::Sizef{sizeW,sizeH}), camera.GetViewMatrix(),camera.GetTransform().GetPosition(), DrawMeshNoLight,DrawMeshLight,DrawMeshLine,DrawMeshNormal );
	}
}
void SurfaceCtrl::InitCamera()noexcept{
	camera.SetPosition(glm::vec3(0.0f,10.0f,30.0f));
}
void SurfaceCtrl::GLResize(int w, int h){
	sizeW = w;
	sizeH = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	Refresh();
}
bool SurfaceCtrl::Key(dword key,int count){
	if( key == K_Z){
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
		float data[] = { 230.0f/255.0f, 140.0f/255.0f, 30.0f/255.0f};
		if(allObjects.GetCount() > 0) allObjects[0].UpdateColors(0,allObjects[0].GetSurfaceCount()/2,data);
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
		camera.ProcessMouveMouvement(200,0);
	}
	if(key == K_RIGHT){
		camera.ProcessMouveMouvement(-200,0);
	}
	if(key == K_UP){
		camera.ProcessMouveMouvement(0,200);
	}
	if(key == K_DOWN){
		camera.ProcessMouveMouvement(0,-200);
	}
	if(key == K_ADD){
		//camera.SetFOV(camera.GetFOV() + 5);
		camera.ProcessMouseScroll(+120);
	}
	if(key == K_SUBTRACT){
		//camera.SetFOV(camera.GetFOV() - 5);
		camera.ProcessMouseScroll(-120);
	}
	if(key == K_C){
		static unsigned short e = 0;
		camera.SetCameraType((CameraType)e);
		e++;
		if(e == 2) e = 0;
	}
	if(key == K_A){
		ShowAxis = !ShowAxis;
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
void SurfaceCtrl::MouseMove(Point p, dword){
	SetFocus();
	camera.ProcessMouveMouvement(p.x - camera.StartPress.x,p.y - camera.StartPress.y);
	camera.StartPress = p;
	Refresh();
}
void SurfaceCtrl::MouseWheel(Point p,int zdelta,dword keyflags){
	//camera.ProcessMouseScroll(zdelta);
	if(zdelta > 0)
		camera.SetFOV(camera.GetFOV() - 1);
	else
		camera.SetFOV(camera.GetFOV() + 1);
	if(camera.GetFOV() <= 0 ) camera.SetFOV(1);
	if(camera.GetFOV() >= 180) camera.SetFOV(179);
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
	camera.StartPress = p;
	camera.MouseMiddlePressed = true;
}
void SurfaceCtrl::MiddleUp(Point p, dword keyflags){
	camera.MouseMiddlePressed = false;
}
void SurfaceCtrl::MouseLeave(){
	camera.MouseLeftPressed = false;
	camera.MouseMiddlePressed = false;
	return;
}
}