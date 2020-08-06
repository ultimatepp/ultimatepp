#include "Camera.h"
//Camera CLASS
namespace Upp{

UOGL_Camera::UOGL_Camera(UOGL_Camera& camera){
	*this = camera;
}
UOGL_Camera& UOGL_Camera::operator=(UOGL_Camera& camera){
	MouseSensitivity = camera.MouseSensitivity;
	MouvementSpeed = camera.MouvementSpeed;
	
	transform = camera.transform; //The Camera Transform object
	
	type = camera.type;
	
	MaxFOV = camera.MaxFOV;
	MinFOV = camera.MinFOV;
	LimiteFOV = camera.LimiteFOV;
	FOV = camera.FOV;
	
	DrawDisanceMax = camera.DrawDisanceMax;
	DrawDistanceMin = camera.DrawDistanceMin;
	return *this;
}
UOGL_Camera& UOGL_Camera::SetTransform(Transform& value){
	transform = value;
	return *this;
}
Transform& UOGL_Camera::GetTransform(){
	return transform;
}
UOGL_Camera& UOGL_Camera::SetCameraType(CameraType value){
	type = value;
	return *this;
}
CameraType UOGL_Camera::GetCameraType()const{
	return type;
}
UOGL_Camera& UOGL_Camera::SetFOV(float value){
	if(LimiteFOV){
		if(value < MinFOV) FOV = MinFOV;
		else if(value > MaxFOV)FOV = MaxFOV;
		else FOV = value;
	}else{
		FOV = value;
	}
	return *this;
}
float UOGL_Camera::GetFOV()const{return FOV;}
UOGL_Camera& UOGL_Camera::SetMaxFOV(float value){
	if(MaxFOV <= MinFOV){
		LOG("ERROR camera in Camera::SetMaxFOV(float) the float passed is equal or lower to MinFOV(" + Upp::AsString(MinFOV) +"), IT CANT BE Possible. MaxFOV Have been set to MinFOV + 10.0f");
		MaxFOV = MinFOV + 10.0f;
	}else{
		MaxFOV = value;
	}
	return *this;
}
float UOGL_Camera::GetMaxFOV()const{return MaxFOV;}
UOGL_Camera& UOGL_Camera::SetMinFOV(float value){
	//You can set a negativ number here, I dont see the point but you can
	//However, it cant be superior as MaxFOV
	if(MinFOV >= MaxFOV){
		LOG("ERROR camera in Camera::SetMinFOV(float) the float passed is equal or Higher to MaxFOV(" + Upp::AsString(MaxFOV) +"), IT CANT BE Possible. MinFOV Have been set to MaxFOV - 10.0f");
		MinFOV = MaxFOV -10.0f;
	}else{
		MinFOV = value;
	}
	return *this;
}
float UOGL_Camera::GetMinFOV()const{return MinFOV;}

UOGL_Camera& UOGL_Camera::EnableLimiteFOV(){LimiteFOV = true; return *this;}
UOGL_Camera& UOGL_Camera::DisableLimiteFOV(){LimiteFOV = false; return *this;}
bool UOGL_Camera::IsFOVLimited()const{return LimiteFOV;}

UOGL_Camera& UOGL_Camera::SetDrawDisanceMax(float value){
	if(value <= DrawDistanceMin){
		LOG("ERROR camera in Camera::SetDrawDisanceMax(float) the float passed is equal or lower to DrawDistanceMin(" + Upp::AsString(DrawDistanceMin) +"), IT CANT BE Possible. DrawDistanceMax Have been set to DrawDistanceMin + 100.0f");
		DrawDisanceMax = DrawDistanceMin + 100.0f;
		
	}else if(value <= DrawDistanceMin){
		LOG("ERROR camera in Camera::SetDrawDisanceMax(float) the float passed is equal or lower to DrawDistanceMin(" + Upp::AsString(DrawDistanceMin) +"), IT CANT BE Possible. DrawDisanceMax Have been set to DrawDistanceMin + 100.0f");
		DrawDisanceMax = DrawDistanceMin + 100.0f;
	}else{
		DrawDisanceMax = value;
	}
	return *this;
}
float UOGL_Camera::GetDrawDisanceMax()const{return DrawDisanceMax;}
UOGL_Camera& UOGL_Camera::SetDrawDistanceMin(float value){
	if(value <= 0){
		LOG("ERROR camera in Camera::SetDrawDistanceMin(float) the float passed is equal or lower to 0, IT CANT BE Possible. DrawDistanceMin Have been set to 0.001f");
		DrawDistanceMin = 0.001f;
	}else if(value >= DrawDisanceMax){
		LOG("ERROR camera in Camera::SetDrawDistanceMin(float) the float passed is equal or Higher to DrawDisanceMax(" + Upp::AsString(DrawDisanceMax) +"), IT CANT BE Possible. DrawDistanceMin Have been set to 0.001f");
		DrawDistanceMin = 0.001f;
	}else{
		DrawDistanceMin = value;
	}
	return *this;
}
float UOGL_Camera::GetDrawDistanceMin()const{return DrawDistanceMin;}

glm::mat4 UOGL_Camera::GetProjectionMatrix(Upp::Sizef SS){
	ScreenSize = SS;
	if(type == CT_PERSPECTIVE){
		return glm::perspective(glm::radians(GetFOV()),(float)( ScreenSize.cx / ScreenSize.cy),GetDrawDistanceMin(),GetDrawDisanceMax());//We calculate Projection here since multiple camera can have different FOV
	}else if(type == CT_ORTHOGRAPHIC){
		float coef = (GetMaxFOV() -  FOV)/8.5f;
		if(coef <= 0) coef = 1.0f;
		return glm::ortho(-(float)ScreenSize.cx/coef,(float)ScreenSize.cx/coef, -(float)ScreenSize.cy/coef,(float)ScreenSize.cy/coef, GetDrawDistanceMin(),GetDrawDisanceMax());
	}else{
		LOG("Swaping to Camera Perspective (cause of unknow type)");
		return glm::perspective(glm::radians(GetFOV()),(float)( ScreenSize.cx / ScreenSize.cy),GetDrawDistanceMin(),GetDrawDisanceMax());//We calculate Projection here since multiple camera can have different FOV
	}
}
UOGL_Camera& UOGL_Camera::SetPosition(glm::vec3 const& position){
	transform.SetNewPosition(position);
	return *this;
}
UOGL_Camera& UOGL_Camera::LookAt(glm::vec3 const& lookTo){
	transform.LookAt(lookTo,true);
	return *this;
}
UOGL_Camera& UOGL_Camera::ProcessMouseScroll(float yoffset){
	if(LimiteFOV && FOV >= MinFOV && FOV <= MaxFOV){
		FOV -=yoffset;
		if(FOV <= MinFOV) FOV = MinFOV;
		if(FOV >= MaxFOV) FOV = MaxFOV;
	}else if(!LimiteFOV){
		FOV -= yoffset;
	}
	return *this;
}
bool UOGL_Camera::ProcessKeyBoard(unsigned long Key,int count){
	//Default keyboard handler
	return false;
}

UOGL_Camera& UOGL_Camera::SetMouvementSpeed(float value){
	MouvementSpeed = value;
	return *this;
}
UOGL_Camera& UOGL_Camera::SetMouseSensitivity(float value){
	MouseSensitivity = value;
	return *this;
}
float UOGL_Camera::GetMouvementSpeed(){
	return MouvementSpeed;
}
float UOGL_Camera::GetMouseSensitivity(){
	return MouseSensitivity;
}

//CameraQuaterion CLASS

CameraQuaterion::CameraQuaterion(){} //be carefull of setting scene correctly
CameraQuaterion::CameraQuaterion(CameraQuaterion& cameraQuaterion) : UOGL_Camera(cameraQuaterion){
	MouvementSpeed = cameraQuaterion.MouvementSpeed;
	MouseSensitivity = cameraQuaterion.MouseSensitivity;
}
CameraQuaterion& CameraQuaterion::operator=(CameraQuaterion& cameraQuaterion){
	UOGL_Camera::operator=(cameraQuaterion);
	MouvementSpeed = cameraQuaterion.MouvementSpeed;
	MouseSensitivity = cameraQuaterion.MouseSensitivity;
	return *this;
}
CameraQuaterion* CameraQuaterion::Clone(){
	return new CameraQuaterion(*this);
}

float CameraQuaterion::GetRealMouseSensitivity(){
	return MouseSensitivity * 0.02f;
}
glm::mat4 CameraQuaterion::GetViewMatrix(){
	return transform.GetViewMatrix();
}
CameraQuaterion& CameraQuaterion::ProcessKeyboardMouvement(Camera_Movement direction){
	float velocity = MouvementSpeed;
	if (direction == CM_FORWARD)
		transform.Move(transform.GetFront()* velocity);
	if (direction == CM_BACKWARD)
	    transform.Move(transform.GetFront()* -velocity);
	if (direction == CM_LEFT)
	    transform.Move(transform.GetRight()* -velocity);
	if (direction == CM_RIGHT)
	    transform.Move(transform.GetRight()* velocity);
	return *this;
}
CameraQuaterion& CameraQuaterion::ProcessMouveMouvement(float xoffset, float yoffset){
	xoffset *= GetRealMouseSensitivity();
	yoffset *= GetRealMouseSensitivity();
	GetTransform().ProcessMouseMouvement(xoffset,yoffset,0);
	return *this;
}

//CameraEuler CLASS
CameraEuler::CameraEuler(){transform.UpdateByEuler(Pitch,Yaw,Roll);} //Be carefull of setting scene correctly
CameraEuler::CameraEuler(CameraEuler& cameraEuler) : UOGL_Camera(cameraEuler){
	Yaw = cameraEuler.Yaw;
	Pitch = cameraEuler.Pitch;
	Roll = cameraEuler.Roll;
	
	MinPitch = cameraEuler.MinPitch;
	MaxPitch = cameraEuler.MaxPitch;
	ActivatePitch = cameraEuler.ActivatePitch;
	ConstraintPitchEnable = cameraEuler.ConstraintPitchEnable;
	
	MinYaw = cameraEuler.MinYaw;
	MaxYaw = cameraEuler.MaxYaw;
	ActivateYaw = cameraEuler.ActivateYaw;
	ConstraintYawEnable = cameraEuler.ConstraintYawEnable;
	
	MinRoll = cameraEuler.MinRoll;
	MaxRoll = cameraEuler.MaxRoll;
	ActivateRoll = cameraEuler.ActivateRoll;
	ConstraintRollEnable = cameraEuler.ConstraintRollEnable;
	transform.UpdateByEuler(Pitch,Yaw,Roll);
}
CameraEuler& CameraEuler::operator=(CameraEuler& cameraEuler){
	UOGL_Camera::operator=(cameraEuler);
	Yaw = cameraEuler.Yaw;
	Pitch = cameraEuler.Pitch;
	Roll = cameraEuler.Roll;
	
	MinPitch = cameraEuler.MinPitch;
	MaxPitch = cameraEuler.MaxPitch;
	ActivatePitch = cameraEuler.ActivatePitch;
	ConstraintPitchEnable = cameraEuler.ConstraintPitchEnable;
	
	MinYaw = cameraEuler.MinYaw;
	MaxYaw = cameraEuler.MaxYaw;
	ActivateYaw = cameraEuler.ActivateYaw;
	ConstraintYawEnable = cameraEuler.ConstraintYawEnable;
	
	MinRoll = cameraEuler.MinRoll;
	MaxRoll = cameraEuler.MaxRoll;
	ActivateRoll = cameraEuler.ActivateRoll;
	ConstraintRollEnable = cameraEuler.ConstraintRollEnable;
	
	transform.UpdateByEuler(Pitch,Yaw,Roll);
	return *this;
}
CameraEuler* CameraEuler::Clone(){
	return new CameraEuler(*this);
}
CameraEuler& CameraEuler::SetYaw(float value){
	if(ActivateYaw){
		if(ConstraintYawEnable){
			if(value < MinYaw) Yaw = MinYaw;
			else if(value > MaxYaw) Yaw = MaxYaw;
			else Yaw = value;
		}else{
			Yaw = value;
		}
	}
	return *this;
}
CameraEuler& CameraEuler::SetPitch(float value){
	if(ActivatePitch){
		if(ConstraintPitchEnable){
			if(value < MinPitch) Pitch = MinPitch;
			else if(value > MaxPitch) Pitch = MaxPitch;
			else Pitch = value;
		}else{
			Pitch = value;
		}
	}
	return *this;
}
CameraEuler& CameraEuler::SetRoll(float value){
	if(ActivateRoll){
		if(ConstraintRollEnable){
			value =  Upp::roundr(value,1);
			if(value < MinRoll) Roll = MinRoll;
			else if(value > MaxRoll) Roll = MaxRoll;
			else Roll = value;
		}else{
			Roll = Upp::roundr(value,1);
		}
	}
	return *this;
}
float CameraEuler::GetYaw()const{
	return Yaw;
}
float CameraEuler::GetPitch()const{
	return Pitch;
}
float CameraEuler::GetRoll()const{
	return Roll;
}
CameraEuler& CameraEuler::SetMinPitch(float value){
	if(MinPitch >= MaxPitch){
		LOG("ERROR camera in CameraEuler::SetMinPitch(float) the float passed is equal or Higher to MaxPitch(" + Upp::AsString(MaxPitch) +"), IT CANT BE Possible. MinPitch Have been set to MaxPitch - 10.0f");
		MinPitch= MaxPitch -10.0f;
	}else{
		MinPitch= value;
	}
	return *this;
}
CameraEuler& CameraEuler::SetMaxPitch(float value){
	if(MaxPitch <= MinPitch){
		LOG("ERROR camera in Camera::SetMaxPitch(float) the float passed is equal or lower to MinPitch(" + Upp::AsString(MinPitch) +"), IT CANT BE Possible. MaxPitch Have been set to MinPitch + 10.0f");
		MaxPitch = MinPitch + 10.0f;
	}else{
		MaxPitch = value;
	}
	return *this;
}
CameraEuler& CameraEuler::SetMinYaw(float value){
	if(MinYaw >= MaxYaw){
		LOG("ERROR camera in CameraEuler::SetMinYaw(float) the float passed is equal or Higher to MaxYaw(" + Upp::AsString(MaxYaw) +"), IT CANT BE Possible. MinYaw Have been set to MaxYaw - 10.0f");
		MinYaw = MaxYaw -10.0f;
	}else{
		MinYaw = value;
	}
	return *this;
}
CameraEuler& CameraEuler::SetMaxYaw(float value){
	if(MaxYaw <= MinYaw){
		LOG("ERROR camera in Camera::SetMaxYaw(float) the float passed is equal or lower to MinYaw(" + Upp::AsString(MinYaw) +"), IT CANT BE Possible. MaxYaw Have been set to MinYaw + 10.0f");
		MaxYaw = MinYaw + 10.0f;
	}else{
		MaxYaw = value;
	}
	return *this;
}
CameraEuler& CameraEuler::SetMinRoll(float value){
	if(MinRoll >= MaxRoll){
		LOG("ERROR camera in CameraEuler::SetMinRoll(float) the float passed is equal or Higher to MaxRoll(" + Upp::AsString(MaxRoll) +"), IT CANT BE Possible. MinRoll Have been set to MaxRoll - 10.0f");
		MinRoll = MaxRoll -10.0f;
	}else{
		MinRoll = value;
	}
	return *this;
}
CameraEuler& CameraEuler::SetMaxRoll(float value){
		if(MaxRoll <= MinRoll){
		LOG("ERROR camera in Camera::SetMaxRoll(float) the float passed is equal or lower to MinRoll(" + Upp::AsString(MinRoll) +"), IT CANT BE Possible. MaxRoll Have been set to MinRoll + 10.0f");
		MaxRoll = MinRoll + 10.0f;
	}else{
		MaxRoll = value;
	}
	return *this;
}

float CameraEuler::GetMinPitch()const{
	return MinPitch;
}
float CameraEuler::GetMaxPitch()const{
	return MaxPitch;
}
float CameraEuler::GetMinYaw()const{
	return MinYaw;
}
float CameraEuler::GetMaxYaw()const{
	return MaxYaw;
}
float CameraEuler::GetMinRoll()const{
	return MinRoll;
}
float CameraEuler::GetMaxRoll()const{
	return MaxRoll;
}

CameraEuler& CameraEuler::EnablePitch(){
	ActivatePitch = true;
	return *this;
}
CameraEuler& CameraEuler::EnableYaw(){
	ActivateYaw = true;
	return *this;
}
CameraEuler& CameraEuler::EnableRoll(){
	ActivateRoll = true;
	return *this;
}

CameraEuler& CameraEuler::DisablePitch(){
	ActivatePitch = false;
	return *this;
}
CameraEuler& CameraEuler::DisableYaw(){
	ActivateYaw = false;
	return *this;
}
CameraEuler& CameraEuler::DisableRoll(){
	ActivateRoll = false;
	return *this;
}

bool CameraEuler::IsPitchEnable()const{
	return ActivatePitch;
}
bool CameraEuler::IsYawEnable()const{
	return ActivateYaw;
}
bool CameraEuler::IsRollchEnable()const{
	return ActivateRoll;
}

CameraEuler& CameraEuler::EnableConstraintPitch(){
	ConstraintPitchEnable = true;
	return *this;
}
CameraEuler& CameraEuler::EnableConstraintYaw(){
	ConstraintYawEnable = true;
	return *this;
}
CameraEuler& CameraEuler::EnableConstraintRoll(){
	ConstraintRollEnable = true;
	return *this;
}

CameraEuler& CameraEuler::DisableConstraintPitch(){
	ConstraintPitchEnable = false;
	return *this;
}
CameraEuler& CameraEuler::DisableConstraintYaw(){
	ConstraintYawEnable = false;
	return *this;
}
CameraEuler& CameraEuler::DisableConstraintRoll(){
	ConstraintRollEnable = true;
	return *this;
}

bool CameraEuler::IsConstraintPitchEnable()const{
	return ConstraintPitchEnable;
}
bool CameraEuler::IsConstraintYawEnable()const{
	return ConstraintYawEnable;
}
bool CameraEuler::IsConstraintRollchEnable()const{
	return ConstraintRollEnable;
}
glm::mat4 CameraEuler::GetViewMatrix(){
	return transform.GetViewMatrix();
}

CameraEuler& CameraEuler::LookAt(glm::vec3 const& lookTo){
	transform.LookAt(lookTo,true);
	glm::vec3 euler = transform.QuaterionToEuler(transform.GetQuaterion());
	if(ActivateYaw)   SetYaw(glm::degrees(euler.y));
	if(ActivatePitch) SetPitch(glm::degrees(euler.x));
	if(ActivateRoll)  SetRoll(glm::degrees(euler.z));
	else SetRoll(0);
	return *this;
}

CameraEuler& CameraEuler::LookAt(glm::vec3 const& lookTo,bool UseYaw, bool UsePitch,bool UseRoll){
	transform.LookAt(lookTo,true);
	glm::vec3 euler = transform.QuaterionToEuler(transform.GetQuaterion());
	if(ActivateYaw && UseYaw)     SetYaw(glm::degrees(euler.y));
	if(ActivatePitch && UsePitch) SetPitch(glm::degrees(euler.x));
	if(ActivateRoll && UseRoll)   SetRoll(glm::degrees(euler.z));
	else SetRoll(0);
	return *this;
}

CameraEuler& CameraEuler::ProcessKeyboardMouvement(Camera_Movement direction){
	float velocity = MouvementSpeed ;
	glm::vec3 position = transform.GetPosition();
	if (direction == CM_FORWARD)
	    position += transform.GetFront() * velocity;
	if (direction == CM_BACKWARD)
	    position -= transform.GetFront() * velocity;
	if (direction == CM_LEFT)
	    position -= transform.GetRight() * velocity;
	if (direction == CM_RIGHT)
	    position += transform.GetRight() * velocity;
	transform.SetNewPosition(position);
	return *this;
}
CameraEuler& CameraEuler::ProcessMouveMouvement(float xoffset, float yoffset){
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;
	SetYaw(Yaw + xoffset);
	SetPitch(Pitch + yoffset);
	SetRoll(0);
	
	
	
	transform.UpdateByEuler(Yaw,Pitch,Roll);
	/*
	glm::vec3 euler = transform.QuaterionToEuler(transform.GetQuaterion());
	Upp::Cout() << "After Conversion : Pitch :"<< glm::degrees(euler.x) << ", Yaw :" << glm::degrees(euler.y) << ", Roll :" << glm::degrees(euler.z) << Upp::EOL;
	*/
	return *this;
}
}