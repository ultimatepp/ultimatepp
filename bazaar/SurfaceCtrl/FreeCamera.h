#ifndef _SurfaceCtrl_FreeCamera_h_
#define _SurfaceCtrl_FreeCamera_h_
#include "Camera.h"

namespace Upp{
	
class FreeCamera : public UOGL_Camera {
	private:
		float yaw = -90.0f;
		float pitch = 0.0f;

	public:
	FreeCamera(){}
	FreeCamera(FreeCamera& free){*this = free;}
	FreeCamera& operator=(FreeCamera& free){UOGL_Camera::operator=(free);return *this;}
	virtual FreeCamera* Clone(){return new FreeCamera(*this);}


	void CalculateFrontRightUp(){
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		
		transform.SetFront(glm::normalize(direction));
		transform.SetRight(glm::normalize(glm::cross(transform.GetFront(), transform.GetWorldUp())));
		transform.SetUp(glm::normalize(glm::cross(transform.GetRight(), transform.GetFront())));
		transform.EulerToQuaterion(yaw,pitch,0.0f);
	}

	virtual glm::mat4 GetViewMatrix(){
		return glm::lookAt(transform.GetPosition(), transform.GetPosition() + transform.GetFront(), transform.GetUp());
	}
	
	virtual FreeCamera& LookAt(glm::vec3 const& lookTo){
		glm::mat4 mat = glm::lookAt(transform.GetPosition(), lookTo , transform.GetUp());
		if (mat[0][0] == 1.0f)
        {
            yaw = atan2f(mat[0][2], mat[2][3]);
            pitch = 0;
        }else if (mat[0][0] == -1.0f)
        {
            yaw = atan2f(mat[0][2],  mat[2][3]);
            pitch = 0;
        }else
        {
            yaw = atan2(-mat[2][0],mat[0][0]);
            pitch = asin(mat[1][0]);
        }
        CalculateFrontRightUp();
		return *this;
	}

	
	virtual FreeCamera& ProcessKeyboardMouvement(Camera_Movement direction){
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
	
	virtual FreeCamera& ProcessMouveMouvement(float xoffset, float yoffset){
		if(MouseLeftPressed){
			xoffset *= MouseSensitivity;
			yoffset *= MouseSensitivity;
			
			yaw += xoffset;
			pitch += yoffset;
			
			if(pitch >  89.0f) pitch = 89.0f;
			else if(pitch < - 89.0f) pitch = - 89.0f;
			
			CalculateFrontRightUp();
		}
		return *this;
	}
	
	FreeCamera& SetYaw(float y){yaw = y;return *this;}
	FreeCamera& SetPitch(float p){pitch = p;if(pitch >  89.0f) pitch = 89.0f;else if(pitch < - 89.0f) pitch = - 89.0f;return *this;}
	
};
}
#endif
