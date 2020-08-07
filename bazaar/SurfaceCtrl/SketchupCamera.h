#ifndef _SurfaceCtrl_SketchupCamera_h_
#define _SurfaceCtrl_SketchupCamera_h_
#include "Camera.h"

namespace Upp{
class SketchupCamera : public UOGL_Camera {
	public:
		glm::vec3 focus = glm::vec3(10.0f,10.0f,0.0f); //point the camera will focus
		
		SketchupCamera(){}
		virtual SketchupCamera* Clone(){
			return new SketchupCamera(*this);
		}

		
		SketchupCamera& Init(){
			SetPosition(glm::vec3(2.0f,3.0f,5.0f));
			LookAt(focus);
			return *this;
		}
		
		
		virtual glm::mat4 GetViewMatrix(){
			return glm::lookAt(transform.GetPosition(), focus , transform.GetUp());
		}

		virtual SketchupCamera& ProcessKeyboardMouvement(Camera_Movement direction){
			return *this;
		}
		
		virtual SketchupCamera& ProcessMouseWheelMouvement(float xoffset,float yoffset){
			xoffset *= MouseSensitivity;
			yoffset *= MouseSensitivity;
	
			float a1 = xoffset * -1.0f;
			float AbsA1 = sqrt(pow(a1,2));
			float a2 = yoffset * -1.0f;
			float AbsA2 = sqrt(pow(a2,2));
			
			if(AbsA1 > AbsA2) a2 = 0.0f;
			else a1 = 0.0f;

			glm::vec3 v =  transform.GetPosition() ;
			glm::quat upRotation = Transform::GetQuaterion(a1,transform.GetWorldUp());
			glm::quat rightRotation = Transform::GetQuaterion(a2,glm::normalize(glm::cross(transform.GetUp(),v))); // Quat using the right vector
			v = glm::rotate(upRotation, v);
			v = glm::rotate(rightRotation, v);
			
			transform.Rotate(glm::inverse(upRotation * rightRotation));
			
			transform.SetPosition(v);
			return *this;
		}
		virtual SketchupCamera& ProcessMouseLeftMouvement(float xoffset, float yoffset){
			xoffset *= (MouseSensitivity * MouvementSpeed);
			yoffset *= (MouseSensitivity * MouvementSpeed);
			
			Cout() << "yoffset : " << yoffset << EOL;
			
			transform.Move(0,yoffset,0);
			focus += glm::vec3(0.0f,yoffset,0.0f);
			return *this;
		}
		
		virtual SketchupCamera& ProcessMouveMouvement(float xoffset, float yoffset){
			if(MouseMiddlePressed) return ProcessMouseWheelMouvement(xoffset,yoffset);
			if(MouseLeftPressed) return ProcessMouseLeftMouvement(xoffset,yoffset);
		}
		
		virtual bool ProcessKeyBoard(unsigned long Key,int count){
			return true;
		}
		
		virtual SketchupCamera& ProcessMouseScroll(float zdelta){
			if(zdelta == - 120){
				transform.SetPosition(transform.GetPosition() + (0.1f * transform.GetPosition()));
				
			}else{
				transform.SetPosition(transform.GetPosition() - (0.1f * transform.GetPosition()));
			}
			return *this;
		}

};
}
#endif
