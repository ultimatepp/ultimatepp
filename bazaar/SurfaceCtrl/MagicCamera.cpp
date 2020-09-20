#include "MagicCamera.h"
namespace Upp{
glm::mat4 MagicCamera::GetProjectionMatrix()const noexcept{
	if(type == CameraType::PERSPECTIVE){
		return glm::perspective(glm::radians(GetFOV()),float( ScreenSize.cx / ScreenSize.cy),GetDrawDistanceMin(),GetDrawDisanceMax());//We calculate Projection here since multiple camera can have different FOV
	}else if(type == CameraType::ORTHOGRAPHIC){
		float distance = glm::distance(glm::vec3(0,0,0),transform.GetPosition())* float(ScreenSize.cx/ScreenSize.cy);
		float distanceY = glm::distance(glm::vec3(0,0,0),transform.GetPosition());
		return glm::ortho(-distance ,distance ,-distanceY ,distanceY, 0.00001f, 10000.0f);
	//	return glm::mat4();
	}else{
		LOG("Swaping to Camera Perspective (cause of unknow type)");
		return glm::perspective(glm::radians(GetFOV()),(float)( ScreenSize.cx / ScreenSize.cy),(-GetDrawDisanceMax())*10.0f,(GetDrawDisanceMax())*10.0f);//We calculate Projection here since multiple camera can have different FOV
	}
}
glm::mat4 MagicCamera::GetViewMatrix()const noexcept{
	return glm::lookAt( transform.GetPosition() ,  transform.GetPosition() + transform.GetFront() , transform.GetUp());
}

glm::vec3 MagicCamera::UnProject2(float winX, float winY,float winZ)const noexcept{
	glm::mat4 View = GetViewMatrix() * glm::mat4(1.0f);
    glm::mat4 projection = GetProjectionMatrix();
	glm::mat4 viewProjInv = glm::inverse(projection * View);
	winY = float(ScreenSize.cy) - winY;
	glm::vec4 clickedPointOnSreen;
	clickedPointOnSreen.x = ((winX - 0.0f) / float(ScreenSize.cx)) *2.0f -1.0f;
	clickedPointOnSreen.y = ((winY - 0.0f) / float(ScreenSize.cy)) * 2.0f -1.0f;
	clickedPointOnSreen.z = 2.0f*winZ-1.0f;
	clickedPointOnSreen.w = 1.0f;
    glm::vec4 clickedPointOrigin  =  viewProjInv * clickedPointOnSreen;
    return glm::vec3(clickedPointOrigin.x / clickedPointOrigin.w,clickedPointOrigin.y / clickedPointOrigin.w,clickedPointOrigin.z / clickedPointOrigin.w);
}

MagicCamera& MagicCamera::MouseWheelMouvement(float xoffset,float yoffset)noexcept{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;
	
	float a1 = xoffset * -1.0f;
	float a2 = yoffset * -1.0f;
	
	glm::vec3 pos = focus - transform.GetPosition();
	float angle = glm::dot(glm::normalize(transform.GetFront()),glm::normalize(pos));
	
	glm::vec3 between;
	
	if(type == CameraType::ORTHOGRAPHIC){
		between =  transform.GetPosition();
		focus = glm::vec3(0.0f,0.0f,0.0f);
	}else{
		if(angle < 0.90f){
			if (angle  < 0){
				focus = glm::vec3(0.0f,0.0f,0.0f);
			}
			focus =  transform.GetPosition() + (transform.GetFront()*10.0f);
		}
		between = transform.GetPosition() - focus;
	}
	
	glm::quat upRotation = Transform::GetQuaterion(a1,transform.GetWorldUp());
	glm::quat rightRotation = Transform::GetQuaterion(a2, transform.GetRight());
	
	between = glm::rotate(upRotation, between);
	between = glm::rotate(rightRotation, between);
	
	transform.SetPosition(focus + between);
	transform.Rotate(glm::inverse(upRotation * rightRotation));
	
	return *this;
}

MagicCamera& MagicCamera::ProcessMouseScroll(float zdelta, float multiplier)noexcept{
	//Must call DetermineRotationPoint before
	float xoffset = (lastPress.x - (float(ScreenSize.cx)/2.0f)) * 0.05f;
	float yoffset = (lastPress.y) * 0.05f * -1.0f;
	float Upoffset = (lastPress.y - (float(ScreenSize.cy)/2.0f)) * 0.05f;
	
	glm::vec3 scaling = (0.1f *  (transform.GetPosition() - focus))* multiplier;
	
	if(zdelta == - 120){
		    if(!OnObject){
				transform.SetPosition(transform.GetPosition() - ((transform.GetRight() * xoffset)* multiplier));
				transform.SetPosition(transform.GetPosition() + ((transform.GetFront() * yoffset)* multiplier));
				transform.SetPosition(transform.GetPosition() + ((transform.GetUp() * Upoffset)* multiplier));
			}else{
				transform.SetPosition(transform.GetPosition() + scaling);
			}
	}else{
		if(!OnObject){
			transform.SetPosition(transform.GetPosition() + ((transform.GetRight() * xoffset)* multiplier));
			transform.SetPosition(transform.GetPosition() - ((transform.GetFront() * yoffset)* multiplier));
			transform.SetPosition(transform.GetPosition() - ((transform.GetUp() * Upoffset)* multiplier));
		}else{
			float length = glm::length(GetTransform().GetPosition() - focus);
			if(length > 2.0f)
				transform.SetPosition(transform.GetPosition() - (scaling));
		}
	}
	return *this;
}

MagicCamera& MagicCamera::ProcessMouseWheelTranslation(float xoffset,float yoffset){
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


int MagicCamera::Pick(float x, float y,const Upp::Vector<Object3D>& allObjects)const noexcept{
	int intersect = -1;
	double distance = 100000.0f;
	glm::vec3 start = UnProject2(x,y,0.0f);
	glm::vec3 end = UnProject2(x,y,1.0f);
	for (const Object3D& obj : allObjects){
       if (obj.TestLineIntersection(start,end)){
            double dis = glm::length(transform.GetPosition() - obj.GetTransform().GetPosition());
            if( dis < distance){
				distance = dis;
				intersect =obj.GetID();
            }
        }
    }
	return intersect;
}
bool MagicCamera::PickFocus(float x, float y){
	glm::vec3 start = UnProject2(x,y,0.0f);
	glm::vec3 end = UnProject2(x,y,1.0f);
	
	glm::vec3 focusMin = focus - 0.5f;
	glm::vec3 focusMax = focus + 0.5f;
	
	glm::vec3 center     = (focusMin + focusMax) * 0.5f;
    glm::vec3 extents    = focusMax - focus;
    glm::vec3 lineDir    = 0.5f * (end - start);
    glm::vec3 lineCenter = start + lineDir;
    glm::vec3 dir        = lineCenter - center;

    float ld0 = abs(lineDir.x);
    if (abs(dir.x) > (extents.x + ld0))
        return false;

    float ld1 = abs(lineDir.y);
    if (abs(dir.y) > (extents.y + ld1))
        return false;

    float ld2 = abs(lineDir.z);
    if (abs(dir.z) > (extents.z + ld2))
        return false;

    glm::vec3 vCross = glm::cross(lineDir, dir);
    if (abs(vCross.x) > (extents.y * ld2 + extents.z * ld1))
        return false;

    if (abs(vCross.y) > (extents.x * ld2 + extents.y * ld0))
        return false;
   
    if (abs(vCross.z) > (extents.x * ld1 + extents.y * ld0))
        return false;

    return true;
}


MagicCamera& MagicCamera::DetermineRotationPoint(Point& p,const Upp::Vector<Object3D>& allObjects, const Upp::Vector<int>& allSelecteds)noexcept{
	if(allSelecteds.GetCount() == 0){
		int obj = Pick(float(p.x),float(p.y),allObjects);
		if(obj != -1){
			for(const Object3D& o : allObjects){
				if(o.GetID() == obj){
					focus = o.GetBoundingBoxTransformed().GetCenter();
					OnObject = true;
				}
			}
		}else{
			if(PickFocus(float(p.x),float(p.y))){
				OnObject = true;
			}else{
				OnObject = false;
				glm::vec3 pos = focus - transform.GetPosition();
				float angle = glm::dot(glm::normalize(transform.GetFront()),glm::normalize(pos));
				if(angle > 0.95f){
					focus = glm::vec3(0.0f,0.0f,0.0f);
				}
			}
		}
	}else{
		OnObject = true;
	}
	return *this;
}

MagicCamera& MagicCamera::LookAt(const glm::vec3& lookat)noexcept{
	glm::vec3 direction = glm::normalize( lookat - transform.GetPosition());
    if(!(glm::length(direction) > 0.0001)){ // Check if the direction is valid; Also deals with NaN
        transform.SetRotation(glm::quat(1, 0, 0, 0));
		return *this;
    }
	//Check if We must use relative Up
	glm::vec3 upToUse = transform.GetWorldUp();
    if(glm::abs(glm::dot(direction, transform.GetWorldUp())) > .9999f) upToUse = transform.GetUp();
    //Check if parallel
	if(glm::vec3(0.0f) == glm::cross(transform.GetUp(),direction) ) direction = glm::normalize(lookat - (transform.GetPosition() + glm::vec3(0.001f,0.0f,0.001f))); //Change position and recalculate direction
	//Calcul new quaternion
    transform.SetRotation(glm::inverse(glm::quatLookAt(direction, upToUse)));
	return *this;
}

void MagicCamera::ViewFromAxe(bool AxeX, bool AxeY, bool AxeZ, bool Inverse)noexcept{ // Will set camera on axe selected axe
	float length = glm::length(transform.GetPosition() - focus);
	if(Inverse) length *= -1.0f;
	
	glm::vec3 pos = focus;
	if(AxeX){
		pos.x += length;
	}if(AxeY){
		pos.y += length;
	}if(AxeZ){
		pos.z += length;
	}

	
	transform.SetPosition(pos);
	LookAt(focus);
}
}
