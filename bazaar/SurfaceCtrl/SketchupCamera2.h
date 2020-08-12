#ifndef _SurfaceCtrl_SketchupCamera2_h_
#define _SurfaceCtrl_SketchupCamera2_h_

#include "Camera.h"

namespace Upp{
class SketchupCamera2 : public UOGL_Camera{
	private:
		
		Vector<Object3D>* allObjects = nullptr;
		
		glm::vec3 UnProject2(float winX, float winY,float winZ){
			glm::mat4 View = GetViewMatrix() * glm::mat4(1.0f);
		    glm::mat4 projection = GetProjectionMatrix(ScreenSize);
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
		
		Vector<Object3D*> Pick(float x, float y){
			Vector<Object3D*>  intersect;
			if(allObjects){
				glm::vec3 start = UnProject2(x,y,0.0f);
				glm::vec3 end = UnProject2(x,y,1.0f);

				for (Object3D& obj : *(allObjects))
			    {
			       if (obj.TestLineIntersection(start,end))
			        {
			            intersect.Add(&obj);
			        }
			    }
			}
			return intersect;
		}
	
		void AdapteZoomFactor(Array<glm::vec3>& pos){
			if( pos.GetCount() > 0){
				/*glm::vec3 focus = pos[0];
				glm::vec3 vaxis = GetVirtualAxis();
				glm::vec3 diff =  glm::abs(vaxis - focus);
				float zo =  glm::dot(transform.GetFront()* 2.0f,diff);
				DezoomFactor += zo;*/
			}
		}
	
	
	public:
		bool forceZoom = false;
		float DezoomFactor = 25.0f; // 1.0f Mean no dezoom at all
		
		
		SketchupCamera2(){}
		virtual SketchupCamera2* Clone(){
			return new SketchupCamera2(*this);
		}

		
		SketchupCamera2& Init(){
			transform.SetPosition(0,20,20);
			return *this;
		}
		
		SketchupCamera2& SetAllObjects(Vector<Object3D>& all){allObjects = &all;return *this;}
	
		virtual glm::mat4 GetProjectionMatrix(Upp::Sizef SS){
			ScreenSize = SS;
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
		
		virtual SketchupCamera2& ProcessKeyboardMouvement(Camera_Movement direction){
			return *this;
		}
		
		glm::vec3 GetVirtualAxis(){
			glm::vec3 v =  transform.GetPosition();
			glm::vec3 virtualAxis =  v + ((transform.GetFront()) * (DezoomFactor * 2.0f));
			glm::vec3 test = v - virtualAxis;
			//Cout() <<"Distance between camera and virtual :" << test.x <<"," << test.y << "," << test.z << EOL;
			return virtualAxis;
		}
		float GetDezoomFactor(){return DezoomFactor;}
		
		
		virtual SketchupCamera2& ProcessMouseWheelTranslation(float xoffset,float yoffset){
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
		
		virtual SketchupCamera2& ProcessMouseWheelMouvement(float xoffset,float yoffset){
			xoffset *= MouseSensitivity;
			yoffset *= MouseSensitivity;
			float a1 = xoffset * -1.0f;
		//	float AbsA1 = sqrt(pow(a1,2));
			float a2 = yoffset * -1.0f;
		//	float AbsA2 = sqrt(pow(a2,2));
		//	if(AbsA1 > AbsA2) a2 = 0.0f; else a1 = 0.0f;
						
			glm::vec3 v =  transform.GetPosition();
			//glm::vec3 axis = v + ((transform.GetFront() * 10.0f) * (DezoomFactor * 2.0f));
			glm::vec3 axis = GetVirtualAxis();
			glm::vec3 between = v - axis;
					
			glm::quat upRotation = Transform::GetQuaterion(a1,transform.GetWorldUp());
			glm::quat rightRotation = Transform::GetQuaterion(a2,glm::normalize(glm::cross(transform.GetUp(),between)));
			between = glm::rotate(upRotation, between);
			between = glm::rotate(rightRotation, between);
			
			transform.SetPosition(axis + between);
			transform.Rotate(glm::inverse(upRotation * rightRotation));
			
			return *this;
		}
		virtual SketchupCamera2& ProcessMouseLeftClick(float xoffset, float yoffset){
			Vector<Object3D*> obj = Pick(xoffset ,yoffset);
			Array<glm::vec3> centers;
			for(Object3D* o : obj){
				if(o){
					o->ShowBoundingBox(true);
					centers.Add(o->GetBoundingBoxTransformed().GetCenter());
				}
			}
			AdapteZoomFactor(centers);
			return *this;
		}
		
		virtual SketchupCamera2& ProcessMouveMouvement(float xoffset, float yoffset){
			if(MouseMiddlePressed && !ShiftPressed ) return ProcessMouseWheelMouvement(xoffset,yoffset);
			if(MouseMiddlePressed && ShiftPressed ) return ProcessMouseWheelTranslation(xoffset,yoffset);
			return *this;
		}
		
		virtual bool ProcessKeyBoard(unsigned long Key,int count){
			return true;
		}
		
		virtual SketchupCamera2& ProcessMouseScroll(float zdelta){
			float xoffset = (StartPress.x - (ScreenSize.cx/2)) * 0.005f;
			float yoffset = (StartPress.y) * 0.005f * -1.0;
			float Upoffset = (StartPress.y - (ScreenSize.cy/2)) * 0.005f;
			bool doX = false, doY = false;
			if(!forceZoom && ! (type == CT_ORTHOGRAPHIC)){
				/*if(sqrt(pow( StartPress.x - (ScreenSize.cx/2),2)) > (ScreenSize.cx/20)) doX = true;
				if(sqrt(pow( StartPress.y - (ScreenSize.cy/2),2)) > (ScreenSize.cy/20)) doY = true;*/
				doX = true;
				doY = true;
			}
			//glm::vec3 scaling = (0.1f * (transform.GetPosition()));
			glm::vec3 scaling = - transform.GetFront()* 2.0f;
			if(zdelta == - 120){
				    if(doX)transform.SetPosition(transform.GetPosition() - (transform.GetRight() * xoffset));
					if(doY){
						transform.SetPosition(transform.GetPosition() +(transform.GetFront() * yoffset));
						transform.SetPosition(transform.GetPosition() + (transform.GetUp() * Upoffset));
					}
					if(!doY && !doX){
						transform.SetPosition(transform.GetPosition() + scaling);
						DezoomFactor +=1.0f;
					}
			}else{
				if(doX)transform.SetPosition(transform.GetPosition() + (transform.GetRight() * xoffset));
				if(doY){
					transform.SetPosition(transform.GetPosition() - (transform.GetFront() * yoffset));
					transform.SetPosition(transform.GetPosition() - (transform.GetUp() * Upoffset));
				}
				if(!doY && !doX){
					if(DezoomFactor > 2.0f){
						transform.SetPosition(transform.GetPosition() - scaling);
						DezoomFactor -=1.0f;
					}
				}
			}
			return *this;
		}
};
}
#endif
