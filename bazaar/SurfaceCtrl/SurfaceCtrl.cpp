#include "SurfaceCtrl.h"
#include <plugin/jpg/jpg.h>
#include <PdfDraw/PdfDraw.h>
#define IMAGECLASS SurfaceCtrlImg
#define IMAGEFILE <SurfaceCtrl/SurfaceCtrl.iml>
#include <Draw/iml.h>

namespace Upp{
SurfaceCtrl::SurfaceCtrl(){
	WhenBegin = [&]{};
}
SurfaceCtrl::~SurfaceCtrl(){
	WhenEnd();
}

void SurfaceCtrl::Init()noexcept{
	ExecuteGL([&]{
		glEnable(GL_DEPTH_TEST);
	    glEnable(GL_MULTISAMPLE);
		glEnable(GL_BLEND);//Gestion of alpha
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Gestion de l'alpha sur les textures
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO); //Gestion pour le RGB uniquement
		InitShader();
		
		Axis = objProvider.Begin(GL_LINES).CreateAxis(20000).End();
		SetDefaultShader(Axis);
		CameraFocus = objProvider.Begin(GL_TRIANGLE_FAN).AddCube(0.0f,0.0f,0.0f,1,LtYellow()).End();
		SetDefaultShader(CameraFocus);
				
		WhenBegin();
		loaded = true;
	});
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

void SurfaceCtrl::ZoomToFit(){
	float mi = FLT_MAX;
	float ma = FLT_MIN;
	glm::vec3 center(0.0f,0.1f,0.0f);
	int cpt = 0;
	for(Object3D& obj : allObjects){
		BoundingBox b =  obj.GetBoundingBoxTransformed();
		mi = min(mi , min(min(b.GetMin().x,b.GetMin().y),b.GetMin().z));
		ma = max(ma , max(max(b.GetMax().x,b.GetMax().y),b.GetMax().z));
		if(cpt == 0)
			center = b.GetCenter();
		else
			center = glm::lerp(center,b.GetCenter(),0.5f);
		cpt++;
	}
	
	camera.LookAt(center);
	if(allObjects.GetCount()){
		float size = ma-mi * 1.5f ;
		glm::vec3 newPos = camera.GetTransform().GetFront() * -size;
		newPos.y = glm::abs(newPos.y);
		camera.GetTransform().SetPosition(newPos);
		camera.LookAt(center);
	}
}

//Starting function
void SurfaceCtrl::InitCamera()noexcept{
	camera.Init();
	camera.SetMouseSensitivity(0.2f);
	camera.SetMouvementSpeed(0.09f);
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
	int ID = camera.Pick(float(p.x), float(p.y), allObjects);
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
		//	float length =  glm::length(camera.GetTransform().GetPosition() - allObjects[iterator].GetTransform().GetPosition());
			//Cout()  << length << EOL;
			allObjects[iterator].GetTransform().Move(move  );
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
	ONCELOCK{
		Layout();
		Init(); //Load axis, focus point, skybox, etc...
		InitCamera();
	}
	if(TimerStarted)ProcessTime();
	MemoryIgnoreLeaksBlock __;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	WhenPaint(); //The function wich loop arround all object and draw using proper VAO and shaders
	if(showAxis){
		if(depthAxis)glDepthFunc(GL_ALWAYS);  // change depth function so depth test always passes
		Axis.Draw(camera.GetProjectionMatrix(), camera.GetViewMatrix(),camera.GetTransform().GetPosition());
		if(depthAxis)glDepthFunc(GL_LESS);  // change depth function so depth test passes when values are equal to depth buffer's content
	}
	if(showCameraFocus){
		CameraFocus.GetTransform().SetPosition(camera.GetFocus());
		CameraFocus.Draw(camera.GetProjectionMatrix(), camera.GetViewMatrix(),camera.GetTransform().GetPosition());
	}
	//Draw skybox :
	glm::mat4 proj;
	if(camera.GetCameraType() == CameraType::ORTHOGRAPHIC){
		camera.SetCameraType(CameraType::PERSPECTIVE);
		proj = camera.GetProjectionMatrix();
		camera.SetCameraType(CameraType::ORTHOGRAPHIC);
	}else{
		proj = camera.GetProjectionMatrix();
	}
	skybox.Draw(proj , camera.GetViewMatrix());
	if(fastMode) Refresh();
}
void SurfaceCtrl::GLResize(int w, int h){
	if(w != 0)sizeW = float(w);
	if(h != 0)sizeH = float(h);
	glViewport(0, 0, int(sizeW), int(sizeH));
	camera.SetScreenSize(sizeW, sizeH);
	if(!fastMode)Refresh();
}

//Input event
bool SurfaceCtrl::Key(dword key,int count){
	if( key == K_DELETE){
		DeleteAllSelectedObjects();
	} else if(key == K_ESCAPE){ //removing all selection
		ClearSelectedObject();
		camera.SetFocus(glm::vec3(0.0f,0.0f,0.0f));
	} else if(key & K_CTRL && key & K_A){
		ClearSelectedObject();
		for(Object3D& obj : allObjects){
			AddSelectedObject(obj.GetID());
		}
	} else if (key == K_CTRL_F)
		ZoomToFit();
	else if (key == K_CTRL_ADD || key == (K_CTRL|K_PLUS))
		ProcessZoom(Point(0,0),120);
	else if (key == K_CTRL_SUBTRACT || key == (K_CTRL|K_MINUS))
		ProcessZoom(Point(0,0),-120);
	else if (key == K_CTRL_C)
		ExecuteGL(THISFN(SaveToClipboard), true);
	else if (key == K_CTRL_S)
		ExecuteGL(THISFN(SaveToFile), true);
	else
		return false;
	if(!fastMode)Refresh();
	return true;
}

void SurfaceCtrl::ProcessZoom(Point p, int zdelta, float multiplier){
	camera.DetermineRotationPoint(p,allObjects,allSelected);
	camera.ProcessMouseScroll(float(zdelta),multiplier);
	if(!fastMode) Refresh();
}
void SurfaceCtrl::ViewFromAxe(Point p, bool AxeX, bool AxeY, bool AxeZ){ // Will set camera on axe selected axe
	camera.DetermineRotationPoint(p,allObjects,allSelected);
	camera.ViewFromAxe(AxeX,AxeY,AxeZ);
	if(!fastMode) Refresh();
}

Image SurfaceCtrl::HandleEvent(int event, Point p, int zdelta, dword keyflags){
	Image returnImage = SurfaceCtrlImg::PreciseCursor();
	if ((event & Ctrl::ACTION) == Ctrl::MOUSEWHEEL){
		ProcessZoom(p,zdelta);
	}else if ((event & Ctrl::ACTION) == Ctrl::MOUSELEAVE) {
		camera.MouseMiddlePressed = false;
		camera.MouseLeftPressed = false;
	}else if ((event & Ctrl::BUTTON) == buttonRotation && (event & Ctrl::ACTION) == Ctrl::DOWN) {
		SetFocus();
		camera.MouseMiddlePressed = true;
		camera.lastPress = p;
		camera.DetermineRotationPoint(p,allObjects,allSelected);
	} else if ((event & Ctrl::BUTTON) == buttonDrag && (event & Ctrl::ACTION) == Ctrl::DOWN) {
		SetFocus();
		camera.lastPress = p;
		camera.MouseLeftPressed = true;
		UpdateSelectedObjectViaMouse(p,keyflags);
		if(!fastMode)Refresh();
	} else {
		if ((event & Ctrl::BUTTON) == buttonRotation && (event & Ctrl::ACTION) == Ctrl::UP){
			camera.MouseMiddlePressed = false;
			camera.ShiftPressed = false;
		} else if ((event & Ctrl::BUTTON) == buttonDrag && (event & Ctrl::ACTION) == Ctrl::UP){
			camera.MouseLeftPressed = false;
		} else if((event & Ctrl::ACTION) == Ctrl::MOUSEMOVE) {
			GetMouseLeft();
			camera.ShiftPressed = keyflags & K_SHIFT;
			float XOffset = float(p.x - camera.lastPress.x);
			float YOffset = float(p.y - camera.lastPress.y);
			if(camera.MouseMiddlePressed){
				if(camera.ShiftPressed){
					camera.ProcessMouseWheelTranslation(XOffset,YOffset);
				}else{
					camera.MouseWheelMouvement(XOffset,YOffset);
				}
				if(!fastMode) Refresh();
			}else if(camera.MouseLeftPressed){
				glm::vec3 x = GetCamera().GetTransform().GetRight() * (XOffset * GetCamera().GetMouvementSpeed());
				glm::vec3 y = GetCamera().GetTransform().GetUp() * ((YOffset * -1.0f) * GetCamera().GetMouvementSpeed());
				MoveAllSelectedObjects(x + y);
				if(!fastMode) Refresh();
			}
			camera.lastPress = p;
		}
	}
	
	if(event == CURSORIMAGE && GetMouseMiddle() && camera.MouseMiddlePressed && camera.ShiftPressed ) returnImage = SurfaceCtrlImg::TranslationArrow();
	else if(event == CURSORIMAGE && GetMouseMiddle() && camera.MouseMiddlePressed ) returnImage = SurfaceCtrlImg::RotationArrow();
	else if(event == CURSORIMAGE && GetMouseLeft() && camera.MouseLeftPressed ) returnImage = SurfaceCtrlImg::HandGrab();
	return returnImage;
}

Image SurfaceCtrl::MouseEvent(int event, Point p, int zdelta, dword keyflags) {
	if (((event & Ctrl::BUTTON) == Ctrl::RIGHT) && ((event & Ctrl::ACTION) == Ctrl::UP))
		MenuBar::Execute(THISBACK1(ContextMenu,p));
	Image img = HandleEvent(event, p, zdelta, keyflags);
	Refresh();
	return img;
}
void SurfaceCtrl::ContextMenu(Bar& bar,const Point& p) {
	bar.Add(t_("Fit to data"), SurfaceCtrlImg::ShapeHandles(), [&]{ZoomToFit();}).Key(K_CTRL_F).Help(t_("Zoom to fit visible all data"));
	bar.Add(t_("Zoom +"),SurfaceCtrlImg::ZoomPlus(),  [&]{ProcessZoom(p,120,5);}) .Key(K_CTRL|K_ADD).Help(t_("Zoom in (closer)"));
	bar.Add(t_("Zoom -"),SurfaceCtrlImg::ZoomMinus(), [&]{ProcessZoom(p,-120,5);}).Key(K_CTRL|K_SUBTRACT).Help(t_("Zoom out (away)"));
	bar.Add(t_("View from X axis"),[&]{ViewFromAxe(p,true,false,false);});
	bar.Add(t_("View from Y axis"),[&]{ViewFromAxe(p,false,true,false);});
	bar.Add(t_("View from Z axis"),[&]{ViewFromAxe(p,false,false,true);});
	bar.Separator();
	bar.Add(t_("Copy image"),  SurfaceCtrlImg::Copy(), [&]{ExecuteGL(THISFN(SaveToClipboard), false);}).Key(K_CTRL_C).Help(t_("Copy image to clipboard"));
	bar.Add(t_("Save image"),  SurfaceCtrlImg::Save(), [&]{ExecuteGL(THISFN(SaveToFile), false);}).Key(K_CTRL_S).Help(t_("Save image to file"));
}


Image SurfaceCtrl::GetImage() {
	Size sz = GetSize();
	ImageBuffer ib(sz);

	glGetError();
	glReadPixels(0, 0, sz.cx, sz.cy, GL_BGRA_EXT, GL_UNSIGNED_BYTE, static_cast<GLvoid*>(~ib));
	if (GL_NO_ERROR != glGetError())
		return Null;
	
	Buffer<RGBA> temp(sz.cx);
	for(int i = 0; i < sz.cy/2; i++){
		memcpy(temp,ib[i], sz.cx*sizeof(RGBA));
		memcpy(ib[i], ib[sz.cy-1-i], sz.cx*sizeof(RGBA));
		memcpy(ib[sz.cy-1-i], temp, sz.cx*sizeof(RGBA));
	}
	
	for (int i = 0; i < sz.cy; i++)
		for (int x = 0; x < sz.cx; x++)
			ib[i][x].a = 255;

	return ib;
}

void SurfaceCtrl::SaveToClipboard() {
	GuiLock __;
	Image image = GetImage();
	if (IsNull(image)) {
		Exclamation(t_("Imposible to get view image"));
		return;
	}
	WriteClipboardImage(image);
}

void SurfaceCtrl::OnTypeImage(FileSel *_fs) {
	FileSel &fs = *_fs;
	int id = fs.type.GetIndex();
	
	if (id == 0)
		fs.file = ForceExt(GetFileName(~fs), ".jpg");
	else if (id == 1)
		fs.file = ForceExt(GetFileName(~fs), ".png");
	else if (id == 2)
		fs.file = ForceExt(GetFileName(~fs), ".pdf");
}

void SurfaceCtrl::SaveToFile() {
	GuiLock __;

	Image image = GetImage();
	if (IsNull(image)) {
		Exclamation(t_("Imposible to get view image"));
		return;
	}
	
	FileSel fs;
	fs.Type(Format(t_("%s bitmap file"), "jpeg"), "*.jpg");
	fs.Type(Format(t_("%s bitmap file"), "png"), "*.png");
	fs.Type(Format(t_("%s vector file"), "pdf"), "*.pdf");
	fs.AllFilesType();
	if (!defaultFileName.IsEmpty())
		fs = defaultFileName;
	else
		fs = String(t_("Mesh view")) + ".jpg";
	
	String ext = GetFileExt(~fs);
	fs.DefaultExt(ext);
	int idt = 0;
	if (ext == ".jpg" || ext == ".jpeg")
		idt = 0;
	else if (ext == ".png")
		idt = 1;
	else if (ext == ".pdf")
		idt = 2;
	fs.ActiveType(idt);

	fs.ActiveDir(GetFileFolder(defaultFileName));
	fs.type.WhenAction = THISBACK1(OnTypeImage, &fs);
    if(!fs.ExecuteSaveAs(t_("Saving image to file"))) {
        Exclamation(t_("Image has not been saved"));
        return;
    }
    String fileName = defaultFileName = ~fs;
	if (GetFileExt(fileName) == ".png") {
		WaitCursor waitcursor;
		PNGEncoder encoder;
		encoder.SaveFile(fileName, image);
	} else if (GetFileExt(fileName) == ".jpg") {
		WaitCursor waitcursor;
		JPGEncoder encoder(jpgQuality);
		encoder.SaveFile(fileName, image);
	} else if (GetFileExt(fileName) == ".pdf") {
		WaitCursor waitcursor;
		PdfDraw pdf(GetSize().cx, GetSize().cy);
		pdf.DrawImage(0, 0, image);
		SaveFile(fileName, pdf.Finish());
	} else{
		Exclamation(Format(t_("File format \"%s\" not found"), GetFileExt(fileName)));
	}
}


}