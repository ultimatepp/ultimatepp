#ifndef _ExempleUltimateOpenGL_VisualCamera_h_
#define _ExempleUltimateOpenGL_VisualCamera_h_
#include "Camera.h"
#include "Object3D.h"

namespace Upp{
class VisualCamera : public CameraEuler{ //By redefining ProcessKeyboard and processMouse, I ensure the camera wont react to it ! but however will have their own routine of displacement on object it focus
	protected:
		Object3D* objectToFocus = nullptr;
	public:
		VisualCamera(){}
		VisualCamera(VisualCamera& camera):CameraEuler(*this){
			objectToFocus = camera.objectToFocus;
		}
		VisualCamera& operator=(VisualCamera& camera){
			CameraEuler::operator=(*this);
			objectToFocus = camera.objectToFocus;
			return *this;
		}
		virtual ~VisualCamera(){}
		VisualCamera& SetObjectToFocus(Object3D& toFocus){
			objectToFocus = &toFocus;
			return *this;
		}
		virtual VisualCamera& ProcessKeyboardMouvement(Camera_Movement direction){
			if(objectToFocus){
				//We gonna do nothings on keyboard
			}
			return *this;
		}
		virtual VisualCamera& ProcessMouveMouvement(float xoffset, float yoffset){
			if(objectToFocus){
				xoffset *= MouseSensitivity;
				yoffset *= MouseSensitivity;
				SetYaw(Yaw + xoffset);
				SetPitch(Pitch + yoffset);
				SetRoll(0);
				objectToFocus->GetTransform().UpdateByEuler(Yaw,Pitch,Roll);
			}
			return *this;
		}
		virtual bool ProcessKeyBoard(unsigned long Key,int count){
			if(objectToFocus){
				if( Key == Upp::K_Z){
					objectToFocus->GetTransform().Move(glm::vec3(0,1 * 0.3f,0));
					return true;
				}else if( Key == Upp::K_S){
					objectToFocus->GetTransform().Move(glm::vec3(0,-1 * 0.3f,0));
					return true;
				}else if( Key == Upp::K_Q){
					objectToFocus->GetTransform().Move(glm::vec3(-1 * 0.3f,0,0));
					return true;
				}else if( Key == Upp::K_D){
					objectToFocus->GetTransform().Move(glm::vec3(1 * 0.3f,0,0));
					return true;
				}
			}
			return true;
		}
		
		virtual VisualCamera& ProcessMouseScroll(float yoffset){
			if(objectToFocus){
				//We just gonna Zoom or dezome the object by scalling our camera position;
				glm::vec3 pos = transform.GetPosition();
				if(yoffset> 0){
					//DeZoom to the object
					if(pos.z <= 50)
						transform.Move(glm::vec3(0,0,+1));
				}else{
					if(pos.z >= 10)
					//Zome the object
					transform.Move(glm::vec3(0,0,-1));
				}
			}
			return *this;
		}
};
}

#endif
