#include "SurfaceCtrl.h"

namespace Upp{
SurfaceCtrl::SurfaceCtrl(){
	InitCamera();
	GLResize(600,800);
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
		#include "shaders/FragmentLight.glsl"
	)).Link();
	
	DrawMeshLine.AttachShader(OpenGLShader(GL_VERTEX_SHADER,
		#include "shaders/Vertex.glsl"
	)).AttachShader(OpenGLShader(GL_GEOMETRY_SHADER,
		#include "shaders/GeometryTriangle.glsl"
	)).AttachShader(OpenGLShader(GL_FRAGMENT_SHADER,
		#include "shaders/FragmentBlack.glsl"
	)).Link();
	
	DrawMeshNormal.AttachShader(OpenGLShader(GL_VERTEX_SHADER,
		#include "shaders/Vertex.glsl"
	)).AttachShader(OpenGLShader(GL_GEOMETRY_SHADER,
		#include "shaders/GeometryNormal.glsl"
	)).AttachShader(OpenGLShader(GL_FRAGMENT_SHADER,
		#include "shaders/FragmentBlack.glsl"
	)).Link();
}

//Starting function
void SurfaceCtrl::InitCamera()noexcept{
	camera.Init();
	camera.SetMouseSensitivity(0.2f);
	camera.SetMouvementSpeed(0.09f);
}
void SurfaceCtrl::InitOpenGLFeatures()noexcept{
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);//Gestion of alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Gestion de l'alpha sur les textures
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO); //Gestion pour le RGB uniquement
	InitShader();
}
//Action on all objects vector
Object3D& SurfaceCtrl::CreateObject()noexcept{
	return allObjects.Add();
}

int SurfaceCtrl::FindObject(int ID)const noexcept{
	for(int e = 0; e < allObjects.GetCount(); e++){
		const Object3D& obj = allObjects[e];
		if(obj.GetID() == ID){
			return e;
		}
	}
	return -1;
}
Object3D& SurfaceCtrl::GetObject(int ID)noexcept{
	ASSERT_(FindObject(ID) != -1, "Invalide Object ID: " + AsString(ID));
	return allObjects[FindObject(ID)];
}

void SurfaceCtrl::DeleteObject(int ID)noexcept{
	for(int e = 0; e < allObjects.GetCount(); e++){
		Object3D& obj = allObjects[e];
		if(obj.GetID() == ID){
			allObjects.Remove(e,1);
			for(int i = 0; i < allSelected.GetCount(); i++){
				if( allSelected[i] == ID ){
					allSelected.Remove(i,1);
					break;
				}
			}
			break;
		}
	}
}
void SurfaceCtrl::DeleteAllObjects()noexcept{
	allSelected.Clear();
	allObjects.Clear();
}
void SurfaceCtrl::SetDefaultShader(Object3D& obj){ //Set default to the object in arg
	obj.SetProgramLight(DrawMeshLight);
	obj.SetProgramNoLight(DrawMeshNoLight);
	obj.SetProgramNormal(DrawMeshNormal);
	obj.SetProgramLine(DrawMeshLine);
}
void SurfaceCtrl::DrawAllObjects()noexcept{
	for(Object3D& obj : allObjects){
		obj.Draw(camera.GetProjectionMatrix(), camera.GetViewMatrix(),camera.GetTransform().GetPosition());
	}
}
//Change selected object vector
void SurfaceCtrl::AddSelectedObject(int ID)noexcept{
	int iterator = FindObject(ID);
	if(iterator != -1){
		for(int i = 0; i < allSelected.GetCount(); i++){
			if( allSelected[i] == ID ){
				return;
			}
		}
		allObjects[iterator].ShowBoundingBox(true);
		allSelected.Add(ID);
	}
}
void SurfaceCtrl::UpdateSelectedObjectViaMouse(Point& p, dword keyflags)noexcept{
	bool ShiftPress = keyflags & K_SHIFT;
	int ID = camera.Pick(p.x,p.y,allObjects);
	if(ID != -1){
		if(!ShiftPress){
			ClearSelectedObject();
		}
		AddSelectedObject(ID);
		camera.SetFocus(GetCenterPoint());
	}else{
		if(!ShiftPress){
			ClearSelectedObject();
			camera.SetFocus(glm::vec3(0.0f,0.0f,0.0f));
		}
	}
}
glm::vec3 SurfaceCtrl::GetCenterPoint()const noexcept{
	glm::vec3 center(0.0f,0.0f,0.0f);
	bool find = false;
	for(int e = 0; e < allSelected.GetCount(); e++){
		int id = allSelected[e];
		for(int i = 0; i < allObjects.GetCount(); i++){
			const Object3D& obj = allObjects[i];
			if(obj.GetID() == id){
				find = true;
				if(e == 0) center = obj.GetBoundingBoxTransformed().GetCenter();
				else
					center = glm::lerp(center,obj.GetBoundingBoxTransformed().GetCenter(),0.5f);
				break;
			}
		}
	}
	return center;
}
void SurfaceCtrl::RemoveSelectedObject(int ID)noexcept{
	for(int e = 0; e < allSelected.GetCount(); e++){
		int id = allSelected[e];
		if(id == ID){
			int iterator = FindObject(id);
			if(iterator != -1)
				allObjects[iterator].ShowBoundingBox(false);
			allSelected.Remove(e,1);
			break;
		}
	}
}
void SurfaceCtrl::ClearSelectedObject()noexcept{
	for(int id : allSelected){
		int iterator = FindObject(id);
		if(iterator != -1)
			allObjects[iterator].ShowBoundingBox(false);
	}
	allSelected.Clear();
}

//Change Object selected
void SurfaceCtrl::MoveAllSelectedObjects(glm::vec3 move)noexcept{ //Move all selected object
	for(int e = 0; e < allSelected.GetCount(); e++){
		int id = allSelected[e];
		int iterator = FindObject(id);
		if(iterator != -1){
			allObjects[iterator].GetTransform().Move(move);
		}
	}
}
void SurfaceCtrl::RotateAllSelectedObjects(glm::quat rotation)noexcept{ //Rotate all selected object
	for(int e = 0; e < allSelected.GetCount(); e++){
		int id = allSelected[e];
		int iterator = FindObject(id);
		if(iterator != -1){
			allObjects[iterator].GetTransform().Rotate(rotation);
		}
	}
}
void SurfaceCtrl::DeleteAllSelectedObjects()noexcept{ //Delete all selected object
	for(int e = 0; e < allSelected.GetCount() ; e++){
		DeleteObject(allSelected[e]);
		e--;
	}
	allSelected.Clear();
}

//time option
void SurfaceCtrl::ProcessTime()noexcept{
	double currentFrame = GetEllapsedTime(); //Calcules le nombre de frames par seconde //Changement made by Iñaki
	DeltaTime = currentFrame - lastFrame;//Calcules le nombre de frames par seconde
	lastFrame = currentFrame;//Calcules le nombre de frames par seconde
	//Fps counter
	bufferFrame++;
	if( currentFrame -  LastTime >= 1.0){
		frameCount =bufferFrame;
		bufferFrame=0;
		LastTime = currentFrame;
	}
}
double SurfaceCtrl::GetEllapsedTime()noexcept{
	if(TimerStarted){
		end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = end-start;
		Timer =diff.count();
		return Timer;
	}else{
		LOG("Timer has not been started, Start it by using SurfaceCtrl::StartTimer();");
		return 0;
	}
}
double SurfaceCtrl::GetDeltaTime()noexcept{
	if(TimerStarted){
		return DeltaTime;
	}else{
		LOG("Timer has not been started, Start it by using SurfaceCtrl::StartTimer();");
		return 0;
	}
}

//Application event
void SurfaceCtrl::GLPaint(){
	if(!loaded){
		OnBegin();
		Axis = objProvider.Begin(GL_LINES).CreateAxis(20000).End();
		SetDefaultShader(Axis);
		CameraFocus = objProvider.Begin(GL_TRIANGLE_FAN).AddCube(0.0f,0.0f,0.0f,1,LtYellow()).End();
		SetDefaultShader(CameraFocus);
		loaded = true;
	}
	if(TimerStarted)ProcessTime();
	MemoryIgnoreLeaksBlock __;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	WhenPaint(); //The function wich loop arround all object and draw using proper VAO and shaders
	
	if(showAxis)
		Axis.Draw(camera.GetProjectionMatrix(), camera.GetViewMatrix(),camera.GetTransform().GetPosition());
	if(ShowCameraFocus){
		if(allObjects.GetCount() > 0){
			CameraFocus.GetTransform().SetPosition(camera.GetFocus());
			CameraFocus.Draw(camera.GetProjectionMatrix(), camera.GetViewMatrix(),camera.GetTransform().GetPosition());
		}
	}
	if(fastMode) Refresh();
}
void SurfaceCtrl::GLResize(int w, int h){
	sizeW = w;
	sizeH = h;
	HSizePos(GetRect().TopLeft().x, w - GetRect().BottomRight().x).VSizePos(GetRect().TopLeft().y, h - GetRect().BottomRight().y);
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	camera.SetScreenSize(w,h);
	if(!fastMode)Refresh();
}

//Input event
void SurfaceCtrl::MouseMove(Point p, dword keyflags){
	camera.ShiftPressed = keyflags & K_SHIFT;
	
	float XOffset = p.x - camera.lastPress.x;
	float YOffset = p.y - camera.lastPress.y;
	if(camera.MouseMiddlePressed){
		if(camera.ShiftPressed)
			camera.ProcessMouseWheelTranslation(XOffset,YOffset);
		else
			camera.MouseWheelMouvement(XOffset,YOffset);
		if(!fastMode) Refresh();
	}else if(camera.MouseLeftPressed){
		glm::vec3 x = GetCamera().GetTransform().GetRight() * (XOffset * GetCamera().GetMouvementSpeed());
		glm::vec3 y = GetCamera().GetTransform().GetUp() * ((YOffset * -1.0f) * GetCamera().GetMouvementSpeed());
		MoveAllSelectedObjects(x + y);
		if(!fastMode) Refresh();
	}
	camera.lastPress = p;
	
}

void SurfaceCtrl::MouseWheel(Point p,int zdelta,dword keyflags){
	camera.DetermineRotationPoint(p,allObjects);
	camera.ProcessMouseScroll(zdelta);
	if(!fastMode) Refresh();
}
void SurfaceCtrl::LeftDown(Point p, dword keyflags){
	SetFocus();
	camera.lastPress = p;
	camera.MouseLeftPressed = true;
	UpdateSelectedObjectViaMouse(p,keyflags);
	if(!fastMode)Refresh();
}
void SurfaceCtrl::LeftUp(Point p, dword keyflags){
	camera.MouseLeftPressed = false;
}
void SurfaceCtrl::MiddleDown(Point p, dword keyflags){
	SetFocus();
	camera.MouseMiddlePressed = true;
	camera.lastPress = p;
	camera.DetermineRotationPoint(p,allObjects);
}
void SurfaceCtrl::MiddleUp(Point p, dword keyflags){
	camera.MouseMiddlePressed = false;
	camera.ShiftPressed = false;
}
void SurfaceCtrl::MouseLeave(){
	camera.MouseMiddlePressed = false;
	camera.MouseLeftPressed = false;
	return;
}
bool SurfaceCtrl::Key(dword key,int count){
	if( key == K_R){
		float data[] = { 230.0f/255.0f, 140.0f/255.0f, 30.0f/255.0f, 0.5f};
		if(allObjects.GetCount() > 0 && allObjects[0].GetMeshes().GetCount() > 0) allObjects[0].UpdateColors(0,0,(allObjects[0].GetMeshes()[0].GetVertices().GetCount()/3)/2,data);
	}
	if( key == K_DELETE){
		DeleteAllSelectedObjects();
	}
	if( key == K_L){
		for(int id : allSelected){
			int iterator = FindObject(id);
			if(iterator != -1){
				allObjects[iterator].ShowMeshLine(!allObjects[iterator].GetShowMeshLine());
			}
		}
	}
	if( key == K_M){
		for(int id : allSelected){
			int iterator = FindObject(id);
			if(iterator != -1){
				allObjects[iterator].ShowMesh(!allObjects[iterator].GetShowMesh());
			}
		}
	}
	if( key == K_P){
		for(int id : allSelected){
			int iterator = FindObject(id);
			if(iterator != -1){
				allObjects[iterator].ShowLight(!allObjects[iterator].GetShowLight());
			}
		}
	}
	if( key == K_N){
		for(int id : allSelected){
			int iterator = FindObject(id);
			if(iterator != -1){
				allObjects[iterator].ShowMeshNormal(!allObjects[iterator].GetShowMeshNormal());
			}
		}
	}
	if(key == K_ADD){
		camera.SetFOV(camera.GetFOV() + 5);
	}
	if(key == K_SUBTRACT){
		camera.SetFOV(camera.GetFOV() - 5);
	}
	
	if(key == K_C){
		static unsigned short e = 1;
		camera.SetCameraType((CameraType)e);
		e++;if(e == 2) e = 0;
	}
	
	if(key == K_A){
		showAxis = !showAxis;
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
	if(key == K_ESCAPE){ //removing all selection
		ClearSelectedObject();
		camera.SetFocus(glm::vec3(0.0f,0.0f,0.0f));
	}
	
	if(key & K_CTRL && key & K_A){
		ClearSelectedObject();
		for(Object3D& obj : allObjects){
			AddSelectedObject(obj.GetID());
		}
	}
	
	if(!fastMode)Refresh();
	return true;
}
}