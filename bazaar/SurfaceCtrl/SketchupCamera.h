#ifndef _SurfaceCtrl_SketchupCamera_h_
#define _SurfaceCtrl_SketchupCamera_h_
#include "Camera.h"

namespace Upp{
class SketchupCamera : public UOGL_Camera {
	public:
		glm::vec3 focus = glm::vec3(0.0f,0.0f,0.0f); //point the camera will focus
		bool forceZoom = false;
		
		SketchupCamera(){}
		virtual SketchupCamera* Clone(){
			return new SketchupCamera(*this);
		}

		
		SketchupCamera& Init(){
			if( focus != glm::vec3(0,0,0))
				transform.SetPosition(focus - (2.0f * (focus)));
			else
				transform.SetPosition(0,0,5);
			return *this;
		}
	
		virtual glm::mat4 GetProjectionMatrix(Upp::Sizef SS){
			ScreenSize = SS;
			if(type == CT_PERSPECTIVE){
				return glm::perspective(glm::radians(GetFOV()),(float)( ScreenSize.cx / ScreenSize.cy),GetDrawDistanceMin(),GetDrawDisanceMax());//We calculate Projection here since multiple camera can have different FOV
			}else if(type == CT_ORTHOGRAPHIC){
				float distance = glm::distance(focus,transform.GetPosition())* (ScreenSize.cx/ScreenSize.cy);
				return glm::ortho(-distance ,distance ,-distance ,distance, 0.00001f, 10000.0f);
			}else{
				LOG("Swaping to Camera Perspective (cause of unknow type)");
				return glm::perspective(glm::radians(GetFOV()),(float)( ScreenSize.cx / ScreenSize.cy),GetDrawDistanceMin(),GetDrawDisanceMax());//We calculate Projection here since multiple camera can have different FOV
			}
		}
		
		virtual glm::mat4 GetViewMatrix(){
			return glm::lookAt( transform.GetPosition() + focus , focus , transform.GetUp());
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
			if(AbsA1 > AbsA2) a2 = 0.0f; else a1 = 0.0f;
			glm::vec3 v =  transform.GetPosition();
			glm::quat upRotation = Transform::GetQuaterion(a1,transform.GetWorldUp());
			glm::quat rightRotation = Transform::GetQuaterion(a2,glm::normalize(glm::cross(transform.GetUp(),v))); // Quat using the right vector
			v = glm::rotate(upRotation, v);
			v = glm::rotate(rightRotation, v);
			transform.SetPosition(v);
			transform.Rotate(glm::inverse(upRotation * rightRotation));
			return *this;
		}
		virtual SketchupCamera& ProcessMouseLeftMouvement(float xoffset, float yoffset){
			yoffset *= 0.05f * -1.0f;
			xoffset *= 0.05f;
			float Absx = sqrt(pow(xoffset,2));
			float Absy = sqrt(pow(yoffset,2));
			if(Absx > Absy){
				focus += transform.GetRight() * xoffset;
			}else{
				focus += transform.GetFront() * yoffset;
			}
			return *this;
		}
		
		virtual SketchupCamera& ProcessMouveMouvement(float xoffset, float yoffset){
			if(MouseMiddlePressed) return ProcessMouseWheelMouvement(xoffset,yoffset);
		//	if(MouseLeftPressed) return ProcessMouseLeftMouvement(xoffset,yoffset);
			return *this;
		}
		
		virtual bool ProcessKeyBoard(unsigned long Key,int count){
			return true;
		}
		
		virtual SketchupCamera& ProcessMouseScroll(float zdelta){
			float xoffset = (StartPress.x - (ScreenSize.cx/2)) * 0.005f;
			float yoffset = (StartPress.y) * 0.005f * -1.0;
			float Upoffset = (StartPress.y - (ScreenSize.cy/2)) * 0.005f;
			bool doX = false, doY = false;
			if(!forceZoom && ! type == CT_ORTHOGRAPHIC){
				/*if(sqrt(pow( StartPress.x - (ScreenSize.cx/2),2)) > (ScreenSize.cx/20)) doX = true;
				if(sqrt(pow( StartPress.y - (ScreenSize.cy/2),2)) > (ScreenSize.cy/20)) doY = true;*/
				doX = true;
				doY = true;
			}
			glm::vec3 scaling = (0.1f * (transform.GetPosition()));
			if(zdelta == - 120){
				    if(doX)focus -= transform.GetRight() * xoffset;
					if(doY){
						focus += transform.GetFront() * yoffset;
						focus += transform.GetUp() * Upoffset;
					}
					if(!doY && !doX) transform.SetPosition(transform.GetPosition() + scaling);
			}else{
				if(doX)focus += transform.GetRight() * xoffset;
				if(doY){
					focus -= transform.GetFront() * yoffset;
					focus -= transform.GetUp() * Upoffset;
				}
				if(!doY && !doX){
					float dot = sqrt(pow(glm::dot(transform.GetPosition(),scaling),2));
					if(dot > 1.0f)
						transform.SetPosition(transform.GetPosition() - scaling);
				}
			}
			return *this;
		}

};
}
#endif
