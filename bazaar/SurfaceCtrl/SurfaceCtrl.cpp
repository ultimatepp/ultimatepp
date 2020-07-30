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
		loaded = true;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	WhenPaint(); //The function wich loop arround all object and draw using proper VAO and shaders

	/*
	//Here I start use OpenGL draw commands to draw my axis
	//Not good idea
	// https://stackoverflow.com/questions/63176073/opengl-cohabitation-of-shader-and-drawing-command
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&(camera.GetProjectionMatrix(Upp::Sizef{sizeW,sizeH})[0][0])); //Setting up projection

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&(camera.GetViewMatrix()[0][0])); //Setting up viewMatrix

	if(ShowAxis)GLDrawFunction::PaintAxis(0, 0, 0,50); //a function that draw axis
	*/
	
}

void SurfaceCtrl::CreateObject(Surface& surf, Color color)noexcept{
	Object3D& obj = allObjects.Create(surf,color);
	//obj.GetTransform().RotateFromAngles(-90.0f,glm::vec3(1.0f,0.0f,0.0f));
	obj.SetVolumeEnvelope(surf.env);
	ZoomToFit();
	Refresh();
}

void SurfaceCtrl::ZoomToFit()noexcept{
	double mxGlobal=0;
	for(Object3D& obj : allObjects){
		VolumeEnvelope env = obj.GetVolumeEnvelope();
		double mx = max(max(env.maxX, env.maxY), env.maxZ);
		if(mx > mxGlobal) mxGlobal = mx;
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

void SurfaceCtrl::ProcessMouse(float yaw,float pitch ){
	camera.ProcessMouveMouvement(yaw,pitch);
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
		camera.SetFOV(camera.GetFOV() + 5);
	}
	if(key == K_SUBTRACT){
		camera.SetFOV(camera.GetFOV() - 5);
	}
	
	if(key == K_C){
		static unsigned short e = 0;
		camera.SetCameraType((CameraType)e);
		e++;
		if(e == 3) e = 0;
	}
	
	if(key == K_A){
		ShowAxis = !ShowAxis;
	}
	
	Refresh();
	return true;
}
void SurfaceCtrl::MouseMove(Point p, dword){
	SetFocus();
	if(pressed){
		ProcessMouse(p.x - StartPress.x,p.y - StartPress.y);
		StartPress = p;
		Refresh();
	}
}
void SurfaceCtrl::MouseWheel(Point p,int zdelta,dword keyflags){
	camera.ProcessMouseScroll(zdelta);
	Refresh();
}
void SurfaceCtrl::LeftDown(Point p, dword){
	StartPress = p;
	pressed = true;
	return;
}
void SurfaceCtrl::LeftUp(Point p, dword){
	pressed = false;
	return;
}
void SurfaceCtrl::MouseLeave(){
	pressed = false;
	return;
}
}