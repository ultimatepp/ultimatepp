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
			if(MouseMiddlePressed){
				xoffset *= MouseSensitivity;
				yoffset *= MouseSensitivity;

				float a1 = (xoffset) *-1.0f;
				float AbsA1 = sqrt(pow(a1,2));
				float a2 =  yoffset *-1.0f;
				float AbsA2 = sqrt(pow(a2,2));
				if(AbsA1 > AbsA2) a2 = 0.0f;
				else a1 = 0.0f;
						
				/*glm::vec3 v =  transform.GetPosition() + focus;
				v = transform4(v,glm::toMat4(GetQuat(a1,transform.GetUp())));
				transform.SetRight(glm::normalize(glm::cross(transform.GetUp(),v)));
				v = transform4(v, glm::toMat4(GetQuat(a2,transform.GetRight())));
				transform.SetUp(glm::normalize(transform4(transform.GetUp(), glm::toMat4(GetQuat(a2,transform.GetRight())))));
				transform.SetPosition(v);*/
				
				glm::vec3 v =  transform.GetPosition() - focus;
				glm::quat upRotation = Transform::GetQuaterion(a1,transform.GetWorldUp());
				glm::quat rightRotation = Transform::GetQuaterion(a2,glm::normalize(glm::cross(transform.GetUp(),v))); // Quat using the right vector
				v = glm::rotate(upRotation, v);
				v = glm::rotate(rightRotation, v);
				//v = Transform::TransformVectorByMatrix(v,glm::toMat4(upRotation));
				//v = Transform::TransformVectorByMatrix(v,glm::toMat4(rightRotation));
				
				transform.Rotate(glm::inverse(upRotation * rightRotation));
				
				//transform.SetRight(glm::normalize(glm::cross(transform.GetUp(),v)));
				//transform.SetUp(glm::normalize(Transform::TransformVectorByMatrix(transform.GetUp(), glm::toMat4(Transform::GetQuaterion(a2,transform.GetRight())))));
				//Cout() << "Up : " << transform.GetUp().x <<"," << transform.GetUp().y << "," << transform.GetUp().z << EOL;
				//Cout() << "Right : " << transform.GetRight().x <<"," << transform.GetRight().y << "," << transform.GetRight().z << EOL;
				transform.SetPosition(v);
			}
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
			float result = glm::dot(transform.GetPosition(), glm::vec3(0.1f,0.1f,0.1f));
			
			if(zdelta  ==  - 120){
				if(result > 0){
					if(camPos.x > camPos.y  && camPos.x > camPos.z) transform.SetPosition(glm::vec3(camPos.x+1.0f,camPos.y,camPos.z));
					if(camPos.y > camPos.z  && camPos.y > camPos.x) transform.SetPosition(glm::vec3(camPos.x,camPos.y+1.0f,camPos.z));
					if(camPos.z > camPos.x  && camPos.z > camPos.y) transform.SetPosition(glm::vec3(camPos.x,camPos.y,camPos.z+1.0f));
				}else{
					if(camPos.x < camPos.y  && camPos.x < camPos.z) transform.SetPosition(glm::vec3(camPos.x-1.0f,camPos.y,camPos.z));
					if(camPos.y < camPos.z  && camPos.y < camPos.x) transform.SetPosition(glm::vec3(camPos.x,camPos.y-1.0f,camPos.z));
					if(camPos.z < camPos.x  && camPos.z < camPos.y) transform.SetPosition(glm::vec3(camPos.x,camPos.y,camPos.z-1.0f));
				}
			}else{
				if(result > 0){
					if(camPos.x > camPos.y  && camPos.x > camPos.z) transform.SetPosition(glm::vec3(camPos.x-1.0f,camPos.y,camPos.z));
					if(camPos.y > camPos.z  && camPos.y > camPos.x) transform.SetPosition(glm::vec3(camPos.x,camPos.y-1.0f,camPos.z));
					if(camPos.z > camPos.x  && camPos.z > camPos.y) transform.SetPosition(glm::vec3(camPos.x,camPos.y,camPos.z-1.0f));
				}else{
					if(camPos.x < camPos.y  && camPos.x < camPos.z) transform.SetPosition(glm::vec3(camPos.x+1.0f,camPos.y,camPos.z));
					if(camPos.y < camPos.z  && camPos.y < camPos.x) transform.SetPosition(glm::vec3(camPos.x,camPos.y+1.0f,camPos.z));
					if(camPos.z < camPos.x  && camPos.z < camPos.y) transform.SetPosition(glm::vec3(camPos.x,camPos.y,camPos.z+1.0f));
				}
			}
			LookAt(focus);
			return *this;
		}

};
}
#endif
