#ifndef _SurfaceCtrl_TrackBallCamera_h_
#define _SurfaceCtrl_TrackBallCamera_h_
#include <plugin/glm/gtc/constants.hpp>
#include <plugin/glm/gtx/rotate_vector.hpp>
#include "Camera.h"

//#include <iostream>
//#include <iomanip>

namespace Upp{
	
class TrackBallCamera : public CameraEuler {
	private :
		glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f);
	public:
		glm::vec3 focus = glm::vec3(0.0f,0.0f,0.0f);; //point the camera will focus
		
		TrackBallCamera(){
			ConstraintPitchEnable = false;
			ConstraintYawEnable = false;
		}
		TrackBallCamera& Init(){
			SetPosition(glm::vec3(0.0f,0.0f,5.0f));
			LookAt(focus);
			return *this;
		}
		virtual glm::mat4 GetViewMatrix(){
			return glm::lookAt(focus - transform.GetPosition(),focus, up);
		}

		
		virtual TrackBallCamera& ProcessKeyboardMouvement(Camera_Movement direction){
			return *this;
		}
		
		
		
		glm::vec3 transform4(glm::vec3& vector,const glm::mat4& matrix){
			float w = vector[0]* matrix[0][3] + vector[1] * matrix[1][3] + vector[2] * matrix[2][3] + matrix[3][3];
			glm::vec3 ret;
			ret.x = (vector[0]*matrix[0][0]+vector[1]*matrix[1][0]+vector[2]*matrix[2][0]+matrix[3][0])/w;
			ret.y = (vector[0]*matrix[0][1]+vector[1]*matrix[1][1]+vector[2]*matrix[2][1]+matrix[3][1])/w;
			ret.z = (vector[0]*matrix[0][2]+vector[1]*matrix[1][2]+vector[2]*matrix[2][2]+matrix[3][2])/w;
			return ret;
		}
		
		
		glm::vec3 RotateAround(float angle, glm::vec3& vector,const glm::vec3& axis){
			float radHalfAngle =(float) glm::radians(angle) / 2.0f;
			float sinVal = glm::sin(radHalfAngle);
			float cosVal = glm::cos(radHalfAngle);
			float xVal = axis.x * sinVal;
			float yVal = axis.y * sinVal;
			float zVal = axis.z * sinVal;
			auto rotation = glm::quat(cosVal,xVal, yVal, zVal);
			return transform4(vector, glm::toMat4(rotation));
		}
		
		virtual TrackBallCamera& ProcessMouveMouvement(float xoffset, float yoffset){
			xoffset *= MouseSensitivity;
			yoffset *= MouseSensitivity;
		
		
			//Another approach
			// Determine rotation angles from the change in mouse position
			float a1 = xoffset/2.0f;
			float a2 = yoffset/2.0f;
			
			// Rotate the target->eye vectoraround the up vector
			glm::vec3 v =  transform.GetPosition() + focus;
			v = RotateAround(a1,v,up);
			
			// Determine the right vector and rotate the target->eye and up around it
			glm::vec3 r = glm::cross(up,v);
			r = glm::normalize(r);
			v = RotateAround(a2,v,r);
			up = RotateAround(a2,up,r);
			up = glm::normalize(up);
			transform.SetNewPosition(v);
			

			//transform.SetNewRotation(glm::quatLookAt(focus - transform.GetPosition(), glm::vec3(0.0f,1.0f,0.0f)) );
			
			
			
			
			
			//working trackball
		/*
			//I must include this quaterion madness in transform class
			float radHalfAngle = glm::radians(yoffset) / 2.0;
			float sinVal = glm::sin(radHalfAngle);
			float cosVal = glm::cos(radHalfAngle);
			float xVal = 1.0f * sinVal;
			float yVal = 0.0f * sinVal;
			float zVal = 0.0f * sinVal;
			auto rot1 = glm::quat(cosVal,xVal, yVal, zVal);
			
			
			radHalfAngle = glm::radians(-xoffset) / 2.0;
			sinVal = glm::sin(radHalfAngle);
			cosVal = glm::cos(radHalfAngle);
			xVal = 0.0f * sinVal;
			yVal = 1.0f * sinVal;
			zVal = 0.0f * sinVal;
			auto rot2 = glm::quat(cosVal,xVal, yVal, zVal);
		
			glm::mat4 position = glm::translate(glm::mat4(1.0f), -transform.GetPosition()) *  glm::toMat4(rot1 * rot2) ;
			transform.SetNewPosition( transform.ExtractPositionFromModelMatrix(position));
			
			transform.SetNewRotation(glm::quatLookAt(focus - transform.GetPosition(), glm::vec3(0.0f,1.0f,0.0f)) );
				
		*/
				
				
				
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
		
		virtual TrackBallCamera& ProcessMouseScroll(float zdelta){
			glm::vec3 pos = transform.GetPosition();
			float result = glm::dot(transform.GetPosition(), glm::vec3(0.1f,0.1f,0.1f));
			//on dezome
			if(zdelta  ==  - 120){
				
				if(result > 0)
					transform.SetNewPosition(transform.GetPosition() + 1.0f );
				else
					transform.SetNewPosition(transform.GetPosition() - 1.0f );
			}else{
				//on zome
				if(result > 0)
					transform.SetNewPosition(transform.GetPosition() - 1.0f );
				else
					transform.SetNewPosition(transform.GetPosition() + 1.0f );
			}
			//LookAt(focus);
			return *this;
		}

};
}
#endif
