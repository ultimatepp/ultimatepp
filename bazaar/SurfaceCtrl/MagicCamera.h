#ifndef _SurfaceCtrl_MagicCamera_h_
#define _SurfaceCtrl_MagicCamera_h_
#include "UOGL_Camera.h"
#include "Object3D.h"

namespace Upp{
class MagicCamera : public UOGL_Camera{
	public:
		MagicCamera(){}
		MagicCamera& Init(){transform.SetPosition(0, 0, 20); focus = glm::vec3(0.0f,0.0f,0.0f); return *this;}
		
		bool IsOnObject(){return OnObject;}
		glm::mat4 GetProjectionMatrix()const noexcept;
		glm::mat4 GetViewMatrix()const noexcept;
		
		glm::vec3 GetFocus(){return focus;}
		MagicCamera& SetFocus(glm::vec3 f){focus = f; return *this;}
		MagicCamera& SetFocus(float x, float y , float z){focus = glm::vec3(x,y,z); return *this;}
		
		int Pick(float x, float y,const Upp::Vector<Object3D>& allObjects)const noexcept; //Return Picked object ID between a vector of object
		MagicCamera& DetermineRotationPoint(Point& p,const Upp::Vector<Object3D>& allObjects, const Upp::Vector<int>& allSelecteds)noexcept; //Find a new focus point depending on cursor position and all Object3D
	
		virtual MagicCamera& LookAt(const glm::vec3& lookat)noexcept;
		void ViewFromAxe(bool AxeX, bool AxeY, bool AxeZ, bool Inverse = false)noexcept; // Will set camera on axe selected axe
	
		MagicCamera& ProcessMouseWheelTranslation(float xoffset,float yoffset); //Move the camera depending on X and Y offset
		MagicCamera& MouseWheelMouvement(float xoffset,float yoffset)noexcept; //Rotate arround axis
		MagicCamera& ProcessMouseScroll(float zdelta, float multiplier = 1.0f)noexcept; //Zoom or move via MouseWheel depending on OnObject Boolean
		
		MagicCamera& ProcessKeyboardMouvement(CameraMovementDirection direction){return *this;} //Handler for keyboard, Useless in this kind of camera
		bool ProcessKeyBoard(unsigned long Key,int count)noexcept{return true;} //Handler for keyboard, Useless in this kind of camera

	private:
		bool OnObject = false;
		glm::vec3 focus;
		
		glm::vec3 UnProject2(float winX, float winY,float winZ)const noexcept;
		bool PickFocus(float x, float y);
	};
}

#endif
