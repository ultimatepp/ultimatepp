#ifndef _UltimateOpenGL_Camera_h_
#define _UltimateOpenGL_Camera_h_
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

		float MaxFOV = 130.0f;
	    float MinFOV = 10.0f;
	    bool  LimiteFOV = true;
	    float FOV =90.0f;

	    float DrawDisanceMax = 10000.0f;
		float DrawDistanceMin = 0.1f;
		
		float MouvementSpeed = 5.0f;
	    float MouseSensitivity = 0.1f;
		
		Upp::Sizef ScreenSize;
	public:
		UOGL_Camera(){}
		UOGL_Camera(UOGL_Camera& camera);
		UOGL_Camera& operator=(UOGL_Camera& camera);
		virtual UOGL_Camera* Clone()=0;
		virtual ~UOGL_Camera(){}

		UOGL_Camera& SetTransform(Transform& value);
		Transform& GetTransform();
		
		UOGL_Camera& SetCameraType(CameraType value);
		CameraType GetCameraType()const;
		
		UOGL_Camera& SetFOV(float value);
		float GetFOV()const;
		UOGL_Camera& SetMaxFOV(float value);
		float GetMaxFOV()const;
		UOGL_Camera& SetMinFOV(float value);
		float GetMinFOV()const;
		
		UOGL_Camera& EnableLimiteFOV();
		UOGL_Camera& DisableLimiteFOV();
		bool IsFOVLimited()const;
		
		UOGL_Camera& SetDrawDisanceMax(float value);
		float GetDrawDisanceMax()const;
		UOGL_Camera& SetDrawDistanceMin(float value);
		float GetDrawDistanceMin()const;
		
		virtual glm::mat4 GetProjectionMatrix(Upp::Sizef ScreenSize);
		virtual glm::mat4 GetViewMatrix() =0;
		
		virtual UOGL_Camera& SetPosition(glm::vec3 const& position);
		virtual UOGL_Camera& LookAt(glm::vec3 const& lookTo);
		 
		virtual UOGL_Camera& ProcessKeyboardMouvement(Camera_Movement direction)=0;
		virtual bool ProcessKeyBoard(unsigned long Key,int count); //To process key
		virtual UOGL_Camera& ProcessMouveMouvement(float xoffset, float yoffset)=0;
		virtual UOGL_Camera& ProcessMouseScroll(float yoffset);
		
		UOGL_Camera& SetMouvementSpeed(float value = 15.0f);
		float GetMouvementSpeed();
		UOGL_Camera& SetMouseSensitivity(float value = 0.01f);
		float GetMouseSensitivity();
		
		Point StartPress;
		bool MouseLeftPressed = false;
		bool MouseMiddlePressed = false;
};
/*
class CameraQuaterion :  public UOGL_Camera{
	protected:
		float GetRealMouseSensitivity();
	public:
		CameraQuaterion(); //be carefull of setting scene correctly
		CameraQuaterion(CameraQuaterion& cameraQuaterion);
		CameraQuaterion& operator=(CameraQuaterion& cameraQuaterion);
		virtual ~CameraQuaterion(){}
		virtual CameraQuaterion* Clone();
		
		
		//virtual glm::mat4 GetProjectionMatrix(Upp::Sizef ScreenSize)const;
		virtual glm::mat4 GetViewMatrix();
		
		virtual CameraQuaterion& ProcessKeyboardMouvement(Camera_Movement direction);
		virtual CameraQuaterion& ProcessMouveMouvement(float xoffset, float yoffset);
};

class CameraEuler : public UOGL_Camera{
	protected:
		float Yaw = 0.0f;
	    float Pitch = 0.0f;
	    float Roll = 0.0f;
	    
	    float MinPitch = -89.9f;
	    float MaxPitch = 89.9f;
	    bool ActivatePitch = true; //Activate rotation on pitch
	    bool ConstraintPitchEnable = true;
	    
	    float MinYaw = -179.9f;
	    float MaxYaw = 179.9f;
	    bool ActivateYaw = true; //Activate rotation on Yaw
	    bool ConstraintYawEnable = false;
	    
	    float MinRoll = -179.9f;
	    float MaxRoll = 179.9f;
	    bool ActivateRoll = false; //Activate rotation on Roll
	    bool ConstraintRollEnable = false;
	public:
		CameraEuler();
		CameraEuler(CameraEuler& cameraEuler);
		CameraEuler& operator=(CameraEuler& cameraEuler);
		virtual ~CameraEuler(){}
		virtual CameraEuler* Clone();
		
		CameraEuler& SetYaw(float value);
		CameraEuler& SetPitch(float value);
		CameraEuler& SetRoll(float value);
		
		float GetYaw()const;
		float GetPitch()const;
		float GetRoll()const;
		
		CameraEuler& SetMinPitch(float value);
		CameraEuler& SetMaxPitch(float value);
		CameraEuler& SetMinYaw(float value);
		CameraEuler& SetMaxYaw(float value);
		CameraEuler& SetMinRoll(float value);
		CameraEuler& SetMaxRoll(float value);
		
		float GetMinPitch()const;
		float GetMaxPitch()const;
		float GetMinYaw()const;
		float GetMaxYaw()const;
		float GetMinRoll()const;
		float GetMaxRoll()const;
		
		CameraEuler& EnablePitch();
		CameraEuler& EnableYaw();
		CameraEuler& EnableRoll();
		
		CameraEuler& DisablePitch();
		CameraEuler& DisableYaw();
		CameraEuler& DisableRoll();
		
		bool IsPitchEnable()const;
		bool IsYawEnable()const;
		bool IsRollchEnable()const;
		
		CameraEuler& EnableConstraintPitch();
		CameraEuler& EnableConstraintYaw();
		CameraEuler& EnableConstraintRoll();
		
		CameraEuler& DisableConstraintPitch();
		CameraEuler& DisableConstraintYaw();
		CameraEuler& DisableConstraintRoll();
		
		bool IsConstraintPitchEnable()const;
		bool IsConstraintYawEnable()const;
		bool IsConstraintRollchEnable()const;

		virtual glm::mat4 GetViewMatrix();
		
		virtual CameraEuler& LookAt(glm::vec3 const& lookTo);
		CameraEuler& LookAt(glm::vec3 const& lookTo,bool UseYaw, bool UsePitch,bool UseRoll);
		
		virtual CameraEuler& ProcessKeyboardMouvement(Camera_Movement direction);
		virtual CameraEuler& ProcessMouveMouvement(float xoffset, float yoffset);
};
*/
}
#endif
