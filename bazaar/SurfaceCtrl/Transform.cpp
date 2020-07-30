#include "Transform.h"
/*
	Transform object from UltimateOpenGL
*/
namespace Upp{
Transform::Transform(){
	Up = glm::vec3(0.0f,1.0f,0.0f);
	Right = glm::vec3(1.0f,0.0f,0.0f);
}
Transform::Transform(const Transform& _transform){
	position = _transform.position;
	quaterion = _transform.quaterion;
	secondQuaterion = _transform.secondQuaterion;
	
	scale= _transform.scale;
	modelMatrix = _transform.modelMatrix;
	
	Front = _transform.Front;
	Up =_transform.Up;
	Right = _transform.Right;
	WorldUp = _transform.WorldUp;
}
Transform& Transform::operator=(const Transform& _transform){
	position = _transform.position;
	quaterion = _transform.quaterion;
	secondQuaterion = _transform.secondQuaterion;
	
	scale= _transform.scale;
	modelMatrix = _transform.modelMatrix;
	
	Front = _transform.Front;
	Up =_transform.Up;
	Right = _transform.Right;
	WorldUp = _transform.WorldUp;
	return *this;
}
//******************Setter/Getter*******************
Transform& Transform::SetFront(const glm::vec3& _front){
	Front = _front;
	return *this;
}
Transform& Transform::SetUp(const glm::vec3& _up){
	Up = _up;
	return *this;
}
Transform& Transform::SetRight(const glm::vec3& _right){
	Right = _right;
	return *this;
}
Transform& Transform::SetWorldUp(const glm::vec3& _worldUp){
	WorldUp = _worldUp;
	return *this;
}
const glm::vec3& Transform::GetFront()const{
	return Front;
}
const glm::vec3& Transform::GetUp()const{
	return Up;
}
const glm::vec3& Transform::GetRight()const{
	return Right;
}
const glm::vec3& Transform::GetWorldUp()const{
	return WorldUp;
}
const glm::vec3& Transform::GetPosition()const{
	return position;
}
const glm::quat& Transform::GetQuaterion()const{
	return quaterion;
}
const glm::quat& Transform::GetSecondQuaterion()const{
	return secondQuaterion;
}
const glm::vec3& Transform::GetScallabeMatrix()const{
	return scale;
}
const glm::mat4 Transform::GetModelMatrixScaller()const{
	return modelMatrix;
}
const Upp::Vector<Transform*>& Transform::GetChildrens()const{
	return childrens;
}
//******************Childs part********************
Transform& Transform::ClearChildrens(){
	childrens.Clear();
	return *this;
}
Transform& Transform::AddChildren(Transform& _ptr){
	bool trouver =false;
	for(Transform* t : childrens){
		if(t == &_ptr){
			trouver = true;
			break;
		}
	}
	if(!trouver) childrens.Add(&_ptr);
	else LOG("Warning : Transform& Transform::AddChild(Transform&)  Transform adresse " + Upp::AsString(&_ptr) +" is still children of this transform");
	return *this;
}
Transform& Transform::AddChildrens(const Upp::Vector<Transform*>& _childrens){
	bool trouver =false;
	for(Transform* t : _childrens){
		trouver =false;
		for(Transform* t2 : childrens){
			if(t == t2){
				trouver = true;
				break;
			}
		}
		if(!trouver) childrens.Add(t);
		else LOG("Warning : Transform& Transform::AddChilds(const Upp::Vector<Transform*>&)  Transform adresse " + Upp::AsString(t) +" is still children of this transform");
	}
	return *this;
}
Transform& Transform::RemoveChildrens(const Upp::Vector<Transform*>& _childrens){
	int cpt = 0;
	for(Transform* t : _childrens){
		cpt=0;
		for(Transform* t2 : childrens){
			if(t == t2){
				childrens.Remove(cpt,1);
				break;
			}
			cpt++;
		}
		if(cpt > childrens.GetCount())LOG("Warning : Transform& Transform::RemoveChilds(const Upp::Vector<Transform*>&) Transform adresse " + Upp::AsString(t) +" is not a children of this transform");
	}
	return *this;
}
Transform& Transform::RemoveChildren(Transform& _ptr){
	int cpt = 0;
	for(Transform* t : childrens){
		if(&_ptr == t){
			childrens.Remove(cpt,1);
			break;
		}
		cpt++;
	}
	if(cpt > childrens.GetCount()) LOG("Warning : Transform& Transform::RemoveChild(Transform&) Transform adresse " + Upp::AsString(&_ptr) +" is not child of this transform");
	return *this;
}
int Transform::NumberOfChildrens(){
	return childrens.GetCount();
}

glm::mat4 Transform::GetModelMatrice()const{
	return glm::translate(glm::mat4(1.0f),position) * glm::mat4_cast(quaterion) * modelMatrix;
}
glm::mat4 Transform::GetViewMatrix()const{
	return glm::lookAt(position,position + Front, Up);
/*	glm::mat4 rotate = glm::mat4_cast(quaterion);
	glm::mat4 translate = glm::mat4(1.0f);
	translate = glm::translate(translate,-position);
	return rotate * translate;*/
}
//******************Position part******************
Transform& Transform::Move(glm::vec3 move,bool updateChildrens){//Move the position from the vec3 arg
	position  += move;
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->Move(move,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::MoveFromAngle(glm::vec3 move,bool updateChildrens ){//Move but care about the angle
	glm::mat4 rotation = glm::mat4_cast(quaterion);
	position = ExtractPositionFromModelMatrix(glm::translate(glm::mat4(1.0f),-move) * rotation);
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->MoveFromAngle(move,updateChildrens);
		}
	}
	return *this;
}

Transform& Transform::SetNewPosition(glm::vec3 newPosition,bool updateChildrens){//set the new position of object //It update all the child by : newPose + (old parent pose - old child pos);
	glm::vec3 buffer = position;
	buffer +=newPosition;
	position = newPosition;
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->Move(buffer,updateChildrens);
		}
	}
	return *this;
}
//******************Scale part*********************
Transform& Transform::Scale(glm::vec3 scallar,bool updateChildrens){
	scale = scallar;
	modelMatrix =  glm::scale(modelMatrix,scale);
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->Scale(scallar,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::SetNewScale(glm::vec3 newScallar,bool updateChildrens){//we ensure the scall is made by mutiply 1.0f model matrix to new scallar
	scale = newScallar;
	modelMatrix =  glm::scale(glm::mat4(1.0f),scale);
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->SetNewScale(scale,updateChildrens);
		}
	}
	return *this;
}
//******************Rotation part******************
Transform& Transform::Rotate(glm::quat _quaterion,bool updateChildrens){
	quaterion =quaterion *_quaterion ;
	RecalculateFrontUpRight(false);
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->Rotate(_quaterion,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::SetNewRotation(glm::quat newQuaterion,bool updateChildrens){
	quaterion =newQuaterion;
	RecalculateFrontUpRight(false);
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->SetNewRotation(newQuaterion,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::RotateFromAngles(float angleInDegree,glm::vec3 EulerAngles,bool updateChildrens){
	if(EulerAngles.x > 0){
		Rotate(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(1.0f,0.0f,0.0f)),updateChildrens);
	}
	if(EulerAngles.y > 0){
		Rotate(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(0.0f,1.0f,0.0f)),updateChildrens);
	}
	if(EulerAngles.z > 0){
		Rotate(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(0.0f,0.0f,1.0f)),updateChildrens);
	}
	return *this;
}
Transform& Transform::SetNewRotationFromAngles(float angleInDegree,glm::vec3 EulerAngles,bool updateChildrens){
	if(EulerAngles.x > 0){
		SetNewRotation(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(1.0f,0.0f,0.0f)),updateChildrens);
	}
	if(EulerAngles.y > 0){
		SetNewRotation(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(0.0f,1.0f,0.0f)),updateChildrens);
	}
	if(EulerAngles.z > 0){
		SetNewRotation(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(0.0f,0.0f,1.0f)),updateChildrens);
	}
	return *this;
}
//******************Second Rotation part***********
Transform& Transform::SecondRotate(glm::quat _quaterion,bool updateChildrens){
	secondQuaterion =secondQuaterion *_quaterion ;
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->SecondRotate(_quaterion,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::SetNewSecondRotation(glm::quat newQuaterion,bool updateChildrens){
	quaterion =newQuaterion;
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->SetNewSecondRotation(newQuaterion,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::SecondRotateFromAngles(float angleInDegree,glm::vec3 EulerAngles,bool updateChildrens){
	if(EulerAngles.x > 0){
		SecondRotate(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(1.0f,0.0f,0.0f)),updateChildrens);
	}
	if(EulerAngles.y > 0){
		SecondRotate(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(0.0f,1.0f,0.0f)),updateChildrens);
	}
	if(EulerAngles.z > 0){
		SecondRotate(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(0.0f,0.0f,1.0f)),updateChildrens);
	}
	return *this;
}
Transform& Transform::SetNewSecondRotationFromAngles(float angleInDegree,glm::vec3 EulerAngles,bool updateChildrens){
	if(EulerAngles.x > 0){
		SetNewSecondRotation(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(1.0f,0.0f,0.0f)),updateChildrens);
	}
	if(EulerAngles.y > 0){
		SetNewSecondRotation(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(0.0f,1.0f,0.0f)),updateChildrens);
	}
	if(EulerAngles.z > 0){
		SetNewSecondRotation(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(0.0f,0.0f,1.0f)),updateChildrens);
	}
	return *this;
}
//******************Quick function part******************
Transform&  Transform::LookAt(glm::vec3 const& lookTo,bool inverse,bool updateChildrens){
	glm::quat buffer = QuatLookAt(position,lookTo,Up);
	if(inverse) buffer = glm::inverse(buffer);
	SetNewRotation(buffer,updateChildrens);
	return *this;
}
Transform& Transform::ProcessMouseMouvement(float Yaw,float Pitch,float Roll,bool updateChildrens){
	glm::quat key_quat = glm::quat(glm::vec3(-Pitch, Yaw, Roll));
	quaterion = key_quat * quaterion;
	quaterion = glm::normalize(quaterion);
	RecalculateFrontUpRight(false);
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->ProcessMouseMouvement(Yaw,Pitch,Roll,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::UpdateByEuler(float Yaw,float Pitch,float Roll,bool updateChildrens){
	quaterion = EulerToQuaterion(Yaw,-Pitch,Roll);
	quaterion = glm::normalize(quaterion);
	RecalculateFrontUpRight(false);
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->UpdateByEuler(Yaw,Pitch,Roll,updateChildrens);
		}
	}
	return *this;
}

Transform& Transform::RecalculateFrontUpRight(bool updateChildrens){
	Front = glm::rotate(glm::inverse(quaterion), glm::vec3(0.0, 0.0,-1.0));
    Right = glm::rotate(glm::inverse(quaterion), glm::vec3(1.0, 0.0, 0.0));
    Up    = glm::rotate(glm::inverse(quaterion), WorldUp);
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->RecalculateFrontUpRight(updateChildrens);
		}
	}
	return *this;
}

//Taken from https://stackoverflow.com/questions/18172388/glm-quaternion-lookat-function
glm::quat Transform::QuatLookAt(glm::vec3 const& lookFrom,glm::vec3 const& lookTo,glm::vec3 const& up){
    glm::vec3  direction       = lookTo - lookFrom;
    float      directionLength = glm::length(direction);
    if(!(directionLength > 0.0001))
        return glm::quat(1, 0, 0, 0);
    direction /= directionLength;
    return glm::quatLookAt(direction, up);
}

//Taken from http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm
glm::vec3 Transform::QuaterionToEuler(glm::quat const& q1){
	double heading  = 0;
	double attitude = 0;
	double bank     = 0;
	double sqw = q1.w*q1.w;
    double sqx = q1.x*q1.x;
    double sqy = q1.y*q1.y;
    double sqz = q1.z*q1.z;
	double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	double test = q1.x*q1.y + q1.z*q1.w;
	if (test > 0.499*unit) { // singularity at north pole
		heading = 2 * atan2(q1.x,q1.w);
		attitude = glm::pi<double>() /2;
		bank = 0;
		return glm::vec3(heading,attitude,bank);
	}
	if (test < -0.499*unit) { // singularity at south pole
		heading = -2 * atan2(q1.x,q1.w);
		attitude = -glm::pi<double>() /2;
		bank = 0;
		return glm::vec3(heading,attitude,bank);
	}
    heading = atan2(2*q1.y*q1.w-2*q1.x*q1.z , sqx - sqy - sqz + sqw);
	bank = asin(2*test/unit);
	attitude = atan2(2*q1.x*q1.w-2*q1.y*q1.z , -sqx + sqy - sqz + sqw);
	return glm::vec3(attitude,heading,bank);
}
//Taken from http://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/index.htm
glm::quat Transform::EulerToQuaterion(float Yaw,float Pitch,float Roll){
	glm::quat myXQuat = glm::angleAxis<float>(glm::radians(Pitch), glm::vec3(1, 0, 0));
	glm::quat myYQuat = glm::angleAxis<float>(glm::radians(Yaw), glm::vec3(0, 1, 0));
	glm::quat myZQuat = glm::angleAxis<float>(glm::radians(Roll), glm::vec3(0, 0, 1));
	return myXQuat * myYQuat * myZQuat;
}
glm::vec3 Transform::ExtractPositionFromModelMatrix(const glm::mat4& modelMatrix){
	glm::mat3 rotMat(modelMatrix);
	glm::vec3 d(modelMatrix[3]);
	
	glm::vec3 retVec = -d * rotMat;
	return retVec;
}

}