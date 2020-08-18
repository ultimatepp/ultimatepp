#ifndef _UltimateOpenGL_UOGL_Camera_h_
#define _UltimateOpenGL_UOGL_Camera_h_
#include "Transform.h"

/*
 Camera object from Ultimate OpenGL
*/
namespace Upp{
enum Camera_Movement {CM_FORWARD,CM_BACKWARD,CM_LEFT,CM_RIGHT};// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraType{CT_PERSPECTIVE = 0 ,CT_ORTHOGRAPHIC = 1};//,CT_FRUSTUM = 2}; //Type of camera rendering
class UOGL_Camera{
	protected:
		Transform transform; //The Camera Transform object
	
		CameraType type = CT_PERSPECTIVE;

		float MaxFOV = 160.0f;
	    float MinFOV = 10.0f;
	    bool  LimiteFOV = true;
	    float FOV =45.0f;

	    float DrawDisanceMax = 10000.0f;
		float DrawDistanceMin = 0.1f;
		
		float MouvementSpeed = 0.09f;
	    float MouseSensitivity = 0.08f;
	    float ZoomSpeed = 10.0f;
		
		Upp::Sizef ScreenSize;
	public:
		
		Point lastPress;
		bool MouseLeftPressed = false;
		bool MouseMiddlePressed = false;
		bool ShiftPressed = false;
		
		UOGL_Camera(){}
		UOGL_Camera(UOGL_Camera& camera){*this = camera;}
		UOGL_Camera& operator=(UOGL_Camera& camera){
			ScreenSize = camera.ScreenSize;
			ZoomSpeed = camera.ZoomSpeed;
			MouseSensitivity = camera.MouseSensitivity;
			MouvementSpeed = camera.MouvementSpeed;
			transform = camera.transform;
			type = camera.type;
			MaxFOV = camera.MaxFOV;
			MinFOV = camera.MinFOV;
			LimiteFOV = camera.LimiteFOV;
			FOV = camera.FOV;
			DrawDisanceMax = camera.DrawDisanceMax;
			DrawDistanceMin = camera.DrawDistanceMin;
			return *this;
		}
		virtual ~UOGL_Camera(){}

		UOGL_Camera& SetTransform(Transform& value)noexcept{transform = value;return *this;}
		Transform& GetTransform()noexcept{return transform;}
		
		UOGL_Camera& SetCameraType(CameraType value)noexcept{type = value;  return *this;}
		CameraType GetCameraType()const noexcept{return type;}
		
		UOGL_Camera& SetFOV(float value)noexcept{if(LimiteFOV){if(value < MinFOV) FOV = MinFOV;else if(value > MaxFOV)FOV = MaxFOV;else FOV = value;}else{FOV = value;}	return *this;}
		float GetFOV()const noexcept{return FOV;}
		float GetMaxFOV()const noexcept{return MaxFOV;}
		float GetMinFOV()const noexcept{return MinFOV;}
		
		UOGL_Camera& SetMaxFOV(float value)noexcept{
			if(MaxFOV <= MinFOV){
				LOG("ERROR camera in Camera::SetMaxFOV(float) the float passed is equal or lower to MinFOV(" + Upp::AsString(MinFOV) +"), IT CANT BE Possible. MaxFOV Have been set to MinFOV + 10.0f");
				MaxFOV = MinFOV + 10.0f;
			}else{
				MaxFOV = value;
			}
			return *this;
		}
		UOGL_Camera& SetMinFOV(float value)noexcept{
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
		UOGL_Camera& EnableLimiteFOV()noexcept{LimiteFOV = true; return *this;}
		UOGL_Camera& DisableLimiteFOV()noexcept{LimiteFOV = false; return *this;}
		bool IsFOVLimited()const noexcept{return LimiteFOV;}
		
		UOGL_Camera& SetDrawDisanceMax(float value)noexcept{
			if(value <= DrawDistanceMin){
				LOG("ERROR camera in Camera::SetDrawDisanceMax(float) the float passed is equal or lower to DrawDistanceMin(" + Upp::AsString(DrawDistanceMin) +"), IT CANT BE Possible. DrawDistanceMax Have been set to DrawDistanceMin + 100.0f");
				DrawDisanceMax = DrawDistanceMin + 100.0f;
			}else if(value <= DrawDistanceMin){
				LOG("ERROR camera in Camera::SetDrawDisanceMax(float) the float passed is equal or lower to DrawDistanceMin(" + Upp::AsString(DrawDistanceMin) +"), IT CANT BE Possible. DrawDisanceMax Have been set to DrawDistanceMin + 100.0f");
				DrawDisanceMax = DrawDistanceMin + 100.0f;
			}else{
				DrawDisanceMax = value;
			}return *this;
		}

		float GetDrawDisanceMax()const noexcept{return DrawDisanceMax;}
		UOGL_Camera& SetDrawDistanceMin(float value)noexcept{
			if(value <= 0){
				LOG("ERROR in UOGL_Camera::SetDrawDistanceMin(float) the float passed is equal or lower to 0, IT CANT BE Possible. DrawDistanceMin Have been set to 0.001f");
				DrawDistanceMin = 0.001f;
			}else if(value >= DrawDisanceMax){
				LOG("ERROR in UOGL_Camera::SetDrawDistanceMin(float) the float passed is equal or Higher to DrawDisanceMax(" + Upp::AsString(DrawDisanceMax) +"), IT CANT BE Possible. DrawDistanceMin Have been set to 0.001f");
				DrawDistanceMin = 0.001f;
			}else{
				DrawDistanceMin = value;
			}
			return *this;
		}
		float GetDrawDistanceMin()const noexcept{return DrawDistanceMin;}
		
		glm::mat4 GetProjectionMatrix()const noexcept{
			if(type == CT_PERSPECTIVE){
				return glm::perspective(glm::radians(GetFOV()),(float)(ScreenSize.cx / ScreenSize.cy),GetDrawDistanceMin(),GetDrawDisanceMax());//We calculate Projection here since multiple camera can have different FOV
			}else if(type == CT_ORTHOGRAPHIC){
				float distance = glm::distance(glm::vec3(0,0,0),transform.GetPosition())* (ScreenSize.cx/ScreenSize.cy);
				float distanceY = glm::distance(glm::vec3(0,0,0),transform.GetPosition());
				return glm::ortho(-distance ,distance ,-distanceY ,distanceY, 0.00001f, 10000.0f);
			}else{
				LOG("Swaping to Camera Perspective (cause of unknow type)");
				return glm::perspective(glm::radians(GetFOV()),(float)( ScreenSize.cx / ScreenSize.cy),GetDrawDistanceMin(),GetDrawDisanceMax());//We calculate Projection here since multiple camera can have different FOV
			}
		}
		UOGL_Camera& LookAt(glm::vec3 const& lookAt)noexcept{transform.LookAt(lookAt);return *this;}
		virtual UOGL_Camera& ProcessMouseScroll(float yoffset)noexcept{
			if(LimiteFOV && FOV >= MinFOV && FOV <= MaxFOV){
				FOV -=yoffset;
				if(FOV <= MinFOV) FOV = MinFOV;
				if(FOV >= MaxFOV) FOV = MaxFOV;
			}else if(!LimiteFOV){
				FOV -= yoffset;
			}
			return *this;
		}
		virtual bool ProcessKeyBoard(unsigned long Key,int count)noexcept{return false;}
		
		UOGL_Camera& SetMouvementSpeed(float value)noexcept{MouvementSpeed = value;return *this;}
		UOGL_Camera& SetMouseSensitivity(float value)noexcept{MouseSensitivity = value;return *this;}
		UOGL_Camera& SetZoomSpeed(float value = 10.0f)noexcept{ZoomSpeed = value; return *this;}
		float GetMouvementSpeed()const noexcept{return MouvementSpeed;}
		float GetMouseSensitivity()const noexcept{return MouseSensitivity;}
		float GetZoomSpeed()const noexcept{return ZoomSpeed;}
		
		Upp::Sizef GetScreenSize()const noexcept{return ScreenSize;}
		UOGL_Camera& SetScreenSize(float width, float height)noexcept{ScreenSize = Sizef(width,height);return *this;}

};
}
#endif
