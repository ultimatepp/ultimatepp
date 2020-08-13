#ifndef _SurfaceCtrl_MagicCamera_h_
#define _SurfaceCtrl_MagicCamera_h_

#include "Camera.h"
namespace Upp{
	class MagicCamera : public UOGL_Camera{
	private:
		Vector<Object3D>* allObjects = nullptr;
		
		glm::vec3 focus;
		
		glm::vec3 UnProject2(float winX, float winY,float winZ){
			glm::mat4 View = GetViewMatrix() * glm::mat4(1.0f);
		    glm::mat4 projection = GetProjectionMatrix();
			glm::mat4 viewProjInv = glm::inverse(projection * View);
			winY = ScreenSize.cy - winY;
			glm::vec4 clickedPointOnSreen;
			clickedPointOnSreen.x = ((winX - 0.0f) / (ScreenSize.cx)) *2.0f -1.0f;
			clickedPointOnSreen.y = ((winY - 0.0f) / (ScreenSize.cy)) * 2.0f -1.0f;
			clickedPointOnSreen.z = 2.0f*winZ-1.0f;
			clickedPointOnSreen.w = 1.0f;
		    glm::vec4 clickedPointOrigin  =  viewProjInv * clickedPointOnSreen;
		    return glm::vec3(clickedPointOrigin.x / clickedPointOrigin.w,clickedPointOrigin.y / clickedPointOrigin.w,clickedPointOrigin.z / clickedPointOrigin.w);
		}
		
		Object3D* Pick(float x, float y){
			Object3D*  intersect = nullptr;
			if(allObjects){
				double distance = 100000.0f;
				
				glm::vec3 start = UnProject2(x,y,0.0f);
				glm::vec3 end = UnProject2(x,y,1.0f);
				
				for (Object3D& obj : *(allObjects)){
			       if (obj.TestLineIntersection(start,end)){
			            double dis = glm::length(transform.GetPosition() - obj.GetTransform().GetPosition());
			            if( dis < distance){
							distance = dis;
							intersect = &obj;
			            }
			        }
			    }
			}
			return intersect;
		}
		
	public:
		MagicCamera(){}
		MagicCamera& Init(){transform.SetPosition(0, 10, 20); focus = glm::vec3(0.0f,0.0f,0.0f); return *this;}
		
		MagicCamera& SetAllObjects(Vector<Object3D>& all){allObjects = &all;return *this;}
	
		glm::mat4 GetProjectionMatrix(){
			if(type == CT_PERSPECTIVE){
				return glm::perspective(glm::radians(GetFOV()),(float)( ScreenSize.cx / ScreenSize.cy),GetDrawDistanceMin(),GetDrawDisanceMax());//We calculate Projection here since multiple camera can have different FOV
			}else if(type == CT_ORTHOGRAPHIC){
				float distance = glm::distance(glm::vec3(0,0,0),transform.GetPosition())* (ScreenSize.cx/ScreenSize.cy);
				float distanceY = glm::distance(glm::vec3(0,0,0),transform.GetPosition());
				return glm::ortho(-distance ,distance ,-distanceY ,distanceY, 0.00001f, 10000.0f);
			//	return glm::ortho(-glm::distance(focus,transform.GetPosition()) ,glm::distance(focus,transform.GetPosition()) ,-glm::distance(focus,transform.GetPosition()) ,glm::distance(focus,transform.GetPosition()), 0.00001f, 10000.0f);
			}else{
				LOG("Swaping to Camera Perspective (cause of unknow type)");
				return glm::perspective(glm::radians(GetFOV()),(float)( ScreenSize.cx / ScreenSize.cy),GetDrawDistanceMin(),GetDrawDisanceMax());//We calculate Projection here since multiple camera can have different FOV
			}
		}
		
		virtual glm::mat4 GetViewMatrix(){
			return glm::lookAt( transform.GetPosition() ,  transform.GetPosition() + transform.GetFront() , transform.GetUp());
		}
		
		virtual MagicCamera& ProcessKeyboardMouvement(Camera_Movement direction){
			return *this;
		}
		
		glm::vec3 GetVirtualAxis(){
			return focus;
		}
		
		virtual MagicCamera& ProcessMouseWheelTranslation(float xoffset,float yoffset){
			yoffset *= 0.05f * -1.0f;
			xoffset *= 0.05f;
			float Absx = sqrt(pow(xoffset,2));
			float Absy = sqrt(pow(yoffset,2));
			if(Absx > Absy){
				transform.Move(transform.GetRight() * xoffset);
			}else{
				transform.Move(transform.GetUp() * yoffset);
			}
			return *this;
		}
		
		virtual MagicCamera& ProcessMouseWheelMouvement(float xoffset,float yoffset){
			xoffset *= MouseSensitivity;
			yoffset *= MouseSensitivity;
			
			float a1 = xoffset * -1.0f;
			float a2 = yoffset * -1.0f;
			
			glm::vec3 axis = GetVirtualAxis();
			
			glm::vec3 pos = focus - transform.GetPosition();
			float angle = glm::dot(glm::normalize(transform.GetFront()),glm::normalize(pos));
			
			float distance = glm::distance(focus,transform.GetPosition());
			if(angle < 0.98f){
				if (angle  < 0){
					focus = glm::vec3(0.0f,0.0f,0.0f);
				}
				axis =  transform.GetPosition() + (transform.GetFront()*10.0f);
			}
			if(distance > 70) transform.SetPosition(transform.GetPosition() *0.999f);
			
			glm::vec3 between = transform.GetPosition() - axis;
			glm::quat upRotation = Transform::GetQuaterion(a1,transform.GetWorldUp());
			glm::quat rightRotation = Transform::GetQuaterion(a2, transform.GetRight());
			//glm::quat rightRotation = Transform::GetQuaterion(a2,glm::normalize(glm::cross(transform.GetUp(),v)));
			
			between = glm::rotate(upRotation, between);
			between = glm::rotate(rightRotation, between);
			
			transform.SetPosition(axis + between);
			transform.Rotate(glm::inverse(upRotation * rightRotation));
			
			
			return *this;
		}
		Object3D* ProcessMouseLeftClick(float xoffset, float yoffset){
			Object3D* obj = Pick(xoffset ,yoffset);
			/*for(Object3D* o : obj){
				if(o){
					o->ShowBoundingBox(true);
					centers.Add(o->GetBoundingBoxTransformed().GetCenter());
				}
			}
			CenterFocus(centers);*/
			return obj;
		}
		
		virtual MagicCamera& ProcessMouveMouvement(float xoffset, float yoffset){
			if(MouseMiddlePressed && !ShiftPressed ) return ProcessMouseWheelMouvement(xoffset,yoffset);
			if(MouseMiddlePressed && ShiftPressed ) return ProcessMouseWheelTranslation(xoffset,yoffset);
			return *this;
		}
		
		virtual bool ProcessKeyBoard(unsigned long Key,int count)noexcept{
			return true;
		}
		
		virtual MagicCamera& ProcessMouseScroll(float zdelta)noexcept{
			float xoffset = (lastPress.x - (ScreenSize.cx/2)) * 0.005f;
			float yoffset = (lastPress.y) * 0.005f * -1.0;
			float Upoffset = (lastPress.y - (ScreenSize.cy/2)) * 0.005f;
			bool doX = false, doY = false;
			if(!(type == CT_ORTHOGRAPHIC)){
				/*if(sqrt(pow( StartPress.x - (ScreenSize.cx/2),2)) > (ScreenSize.cx/20)) doX = true;
				if(sqrt(pow( StartPress.y - (ScreenSize.cy/2),2)) > (ScreenSize.cy/20)) doY = true;*/
				doX = true;
				doY = true;
			}
			glm::vec3 scaling = (0.1f * (transform.GetPosition()));
			if(zdelta == - 120){
				    if(doX)transform.SetPosition(transform.GetPosition() - (transform.GetRight() * xoffset));
					if(doY){
						transform.SetPosition(transform.GetPosition() +(transform.GetFront() * yoffset));
						transform.SetPosition(transform.GetPosition() + (transform.GetUp() * Upoffset));
					}
					if(!doY && !doX)
						transform.SetPosition(transform.GetPosition() + scaling);
			}else{
				if(doX)transform.SetPosition(transform.GetPosition() + (transform.GetRight() * xoffset));
				if(doY){
					transform.SetPosition(transform.GetPosition() - (transform.GetFront() * yoffset));
					transform.SetPosition(transform.GetPosition() - (transform.GetUp() * Upoffset));
				}
				if(!doY && !doX){
					float dot = sqrt(pow(glm::dot(transform.GetPosition(),scaling),2));
					if(dot > 1.0f)
						transform.SetPosition(transform.GetPosition() - scaling);
				}
			}
			return *this;
		}
	
		glm::vec3 GetFocus(){return focus;}
		MagicCamera& ResetFocus(){focus = glm::vec3(0,0,0); return *this;}
		
		MagicCamera& CenterFocus(const Vector<Object3D*>&  selectedObj){
			if(selectedObj.GetCount() > 0){
				glm::vec3 center = selectedObj[0]->GetBoundingBoxTransformed().GetCenter(); // The crash is occuring here, During copy operator, the Append function on vector make things crash
				if(selectedObj.GetCount() > 1){
					for(int e = 1; e < selectedObj.GetCount(); e++){
						glm::vec3 center2 = selectedObj[e]->GetBoundingBoxTransformed().GetCenter();
						center = glm::lerp(center,center2,0.5f);
					}
				}
				focus = center;
			}
			return *this;
		}
	
	};
}

#endif
