#ifndef _SurfaceCtrl_SketchupCamera_h_
#define _SurfaceCtrl_SketchupCamera_h_
#include "Camera.h"

namespace Upp{
class SketchupCamera : public UOGL_Camera {
	public:
		glm::vec3 focus = glm::vec3(0.0f,0.0f,0.0f);; //point the camera will focus
		
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
		
		virtual SketchupCamera& ProcessMouveMouvement(float xoffset, float yoffset){
			xoffset *= MouseSensitivity;
			yoffset *= MouseSensitivity;
	
			float a1 = xoffset * -1.0f;
			float AbsA1 = sqrt(pow(a1,2));
			float a2 = yoffset * -1.0f;
			float AbsA2 = sqrt(pow(a2,2));
			
			if(AbsA1 > AbsA2) a2 = 0.0f;
			else a1 = 0.0f;

			glm::vec3 v =  transform.GetPosition() - focus;
			glm::quat upRotation = Transform::GetQuaterion(a1,transform.GetWorldUp());
			glm::quat rightRotation = Transform::GetQuaterion(a2,glm::normalize(glm::cross(transform.GetUp(),v))); // Quat using the right vector
			v = glm::rotate(upRotation, v);
			v = glm::rotate(rightRotation, v);
			
			transform.Rotate(glm::inverse(upRotation * rightRotation));
			
			transform.SetPosition(v);
			return *this;
		}
		
		virtual bool ProcessKeyBoard(unsigned long Key,int count){
			/*
				if( Key == Upp::K_Z){
					objectToFocus->GetTransform().Move(glm::vec3(0,1 * 0.3f,0));
					return true;
				}else if( Key == Upp::K_S){
					objectToFocus->GetTransform().Move(glm::vec3(0,-1 * 0.3f,0));
					return true;
				}else if( Key == Upp::K_Q){
					objectToFocus->GetTransform().Move(glm::vec3(-1 * 0.3f,0,0));x
					return true;
				}else if( Key == Upp::K_D){
					objectToFocus->GetTransform().Move(glm::vec3(1 * 0.3f,0,0));
					return true;
				}
				*/
			return true;
		}
		
		virtual SketchupCamera& ProcessMouseScroll(float zdelta){
			glm::vec3 camPos = transform.GetPosition();
			float lenght = glm::length(camPos - focus);
			float result = glm::dot(glm::vec3(0.001f,0.001f,0.001f), transform.GetPosition());
			glm::vec3 TheVec = camPos - focus;

			if(zdelta  ==  - 120){
				if(result > 0){
					transform.SetPosition(camPos + ((0.1f * MouvementSpeed) * TheVec));
				}else{
					transform.SetPosition(camPos - ((0.1f * MouvementSpeed) * TheVec));
				}
			}else{
				//Zoom
				if(result > 0){
					transform.SetPosition(camPos - (0.1f * TheVec));
				}else{
					transform.SetPosition(camPos + (0.1f * TheVec));
				}
			}

			///transform.SetRotation(0.0f,0.0f,0.0f);
			return *this;
		}

};
}
#endif
