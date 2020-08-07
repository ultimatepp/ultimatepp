#ifndef _UltimateOpenGL_Transform_h_
#define _UltimateOpenGL_Transform_h_
#include <Core/Core.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <plugin/glm/glm.hpp>
#include <plugin/glm/gtc/matrix_transform.hpp>
#include <plugin/glm/gtc/type_ptr.hpp>
#include <plugin/glm/gtx/quaternion.hpp>
#include <plugin/glm/ext/quaternion_trigonometric.hpp>
#include <plugin/glm/gtx/norm.hpp>
#include <plugin/glm/gtx/string_cast.hpp>
#include <plugin/glm/gtx/matrix_decompose.hpp>
/*
	Transform Object from UltimateOpenGL
*/
namespace Upp{
class Transform{
	private:
		glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
		
		glm::vec3 WorldFront = glm::vec3(0.0, 0.0, -1.0);
		glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 WorldRight = glm::vec3(1.0f, 0.0f, 0.0f);
		
		glm::vec3 Position = glm::vec3(0.0f);
		glm::quat Rotation = glm::angleAxis(glm::radians(90.0f),glm::vec3(0.0f,1.0f,0.0f));
		glm::vec3 Scale = glm::vec3(1.0f);
		
		void RecalculateFURW(){
			Front = glm::rotate(glm::inverse(Rotation), WorldFront);
		    Right = glm::rotate(glm::inverse(Rotation), WorldRight);
		    Up = glm::rotate(glm::inverse(Rotation), WorldUp);
		}
	public:
		Transform(){}
		Transform(const Transform& _transform){*this = _transform;}
		Transform& operator=(const Transform& _transform){Front = _transform.Front; Up = _transform.Up; Right = _transform.Right; WorldUp = _transform.WorldUp; Position = _transform.Position; Rotation = _transform.Rotation; Scale = _transform.Scale; return *this;}
		virtual ~Transform(){}
	
		Transform& SetFront(const glm::vec3& vec3)noexcept{Front = vec3; return *this;}
		Transform& SetUp(const glm::vec3& vec3)noexcept{Up = vec3; return *this;}
		Transform& SetRight(const glm::vec3& vec3)noexcept{Right = vec3; return *this;}
		Transform& SetWorldUp(const glm::vec3& vec3)noexcept{WorldUp = vec3; return *this;}
		Transform& SetWorldFront(const glm::vec3& vec3)noexcept{WorldFront = vec3; return *this;}
		Transform& SetWorldRight(const glm::vec3& vec3)noexcept{WorldRight = vec3; return *this;}
		Transform& SetScale(const glm::vec3& vec3)noexcept{Scale = vec3; return *this;}
		Transform& SetPosition(const glm::vec3& vec3)noexcept{Position = vec3; return *this;}
		Transform& SetPosition(float x, float y, float z)noexcept{Position.x =x; Position.y = y; Position.z = z; return *this;}
		Transform& SetRotation(const glm::quat& quat)noexcept{Rotation = quat; RecalculateFURW(); return *this;}
		Transform& SetRotation(float Yaw, float Pitch, float Roll)noexcept{	Rotation = glm::quat(glm::vec3(Pitch, Yaw , Roll)); RecalculateFURW(); return *this;}
		Transform& SetRotation(float angleDegree, const glm::vec3& axis)noexcept{float radHalfAngle =(float) glm::radians(angleDegree) / 2.0f; float sinVal = glm::sin(radHalfAngle); float cosVal = glm::cos(radHalfAngle); float xVal = axis.x * sinVal; float yVal = axis.y * sinVal; float zVal = axis.z * sinVal; Rotation = glm::quat(cosVal,xVal, yVal, zVal); RecalculateFURW(); return *this;}
		
		glm::vec3 GetFront()const noexcept{return Front;}
		glm::vec3 GetUp()const noexcept{return Up;}
		glm::vec3 GetRight()const noexcept{return Right;}
		glm::vec3 GetWorldUp()const noexcept{return WorldUp;}
		glm::vec3 GetWorldFront()const noexcept{return WorldFront;}
		glm::vec3 GetWorldRight()const noexcept{return WorldRight;}
		glm::vec3 GetPosition()const noexcept{return Position;}
		glm::quat GetRotation()const noexcept{return Rotation;}
		glm::vec3 GetScale()const noexcept{return Scale;}
		
		Transform& Move(const glm::vec3& vec3)noexcept{Position += vec3; return *this;}
		Transform& Move(float x, float y, float z)noexcept{Position += glm::vec3(x,y,z); return *this;}
		
		Transform& Rotate(const glm::quat& quat)noexcept{Rotation *= quat; RecalculateFURW(); return *this;}
		Transform& Rotate(float Yaw, float Pitch, float Roll)noexcept{Rotation *= glm::quat(glm::vec3(Pitch, Yaw , Roll)); RecalculateFURW(); return *this;}
		Transform& Rotate(float angleDegree, const glm::vec3& axis){float radHalfAngle =(float) glm::radians(angleDegree) / 2.0f; float sinVal = glm::sin(radHalfAngle); float cosVal = glm::cos(radHalfAngle);	float xVal = axis.x * sinVal; float yVal = axis.y * sinVal; float zVal = axis.z * sinVal; Rotation *= glm::quat(cosVal,xVal, yVal, zVal); RecalculateFURW(); return *this;}
	
		Transform& LookAt(const glm::vec3& LookAt)noexcept{
			glm::vec3 direction = LookAt - Position;
			float directionLength = glm::length(direction);
			if(directionLength > 0.0001){
				direction /= directionLength;
				SetRotation(glm::quatLookAt(glm::normalize(direction), Up));
			}
			return *this;
		}
			
		glm::mat4 GetViewMatrix()const noexcept{return glm::lookAt(Position,Position + Front, Up);}
		glm::mat4 GetModelMatrix()const noexcept{return glm::translate(glm::mat4(1.0f),Position) * glm::mat4_cast(Rotation) * glm::scale(glm::mat4(1.0f),Scale);}

		//Static functions
		static glm::quat GetQuaterion(float Yaw, float Pitch, float Roll)noexcept{
			return glm::quat(glm::vec3(Pitch, Yaw , Roll));
		}
		static glm::quat GetQuaterion(float angleDegree, const glm::vec3& axis)noexcept{
			float radHalfAngle =(float) glm::radians(angleDegree) / 2.0f;
			float sinVal = glm::sin(radHalfAngle);
			float cosVal = glm::cos(radHalfAngle);
			float xVal = axis.x * sinVal;
			float yVal = axis.y * sinVal;
			float zVal = axis.z * sinVal;
			return glm::quat(cosVal,xVal, yVal, zVal);
		}
		
		static glm::vec3 TransformVectorByMatrix(const glm::vec3& vector,const glm::mat4& matrix){
			float w = vector[0]* matrix[0][3] + vector[1] * matrix[1][3] + vector[2] * matrix[2][3] + matrix[3][3];
			glm::vec3 ret;
			ret.x = (vector[0]*matrix[0][0]+vector[1]*matrix[1][0]+vector[2]*matrix[2][0]+matrix[3][0])/w;
			ret.y = (vector[0]*matrix[0][1]+vector[1]*matrix[1][1]+vector[2]*matrix[2][1]+matrix[3][1])/w;
			ret.z = (vector[0]*matrix[0][2]+vector[1]*matrix[1][2]+vector[2]*matrix[2][2]+matrix[3][2])/w;
			return ret;
		}
};

	
	/*
class Transform{
	protected:
		Upp::Vector<Transform*> childrens;

		glm::vec3 position = glm::vec3(0.0f);
		glm::quat quaterion = glm::quat(1.0f,0.0f,0.0f,0.0f);
		glm::quat secondQuaterion = glm::quat(1.0f,0.0f,0.0f,0.0f);

		glm::vec3 scale= glm::vec3(1.0f);
		glm::mat4 modelMatrix = glm::mat4(1.0f);

		glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		
		
	public:
		Transform();
		Transform(const Transform& _transform);
		Transform& operator=(const Transform& _transform);
		~Transform(){}
		
		//******************Setter/Getter*******************
		Transform& SetFront(const glm::vec3& _front);
		Transform& SetUp(const glm::vec3& _up);
		Transform& SetRight(const glm::vec3& _right);
		Transform& SetWorldUp(const glm::vec3& _worldUp);

		const glm::vec3& GetFront()const;
		const glm::vec3& GetUp()const;
		const glm::vec3& GetRight()const;
		const glm::vec3& GetWorldUp()const;
		const glm::vec3& GetPosition()const;
		const glm::quat& GetQuaterion()const;
		const glm::quat& GetSecondQuaterion()const;
		const glm::vec3& GetScallabeMatrix()const;
		const glm::mat4 GetModelMatrixScaller()const;
		const Upp::Vector<Transform*>& GetChildrens()const;

		//******************Childs part********************
		Transform& ClearChildrens();
		Transform& AddChildren(Transform& _ptr);
		Transform& AddChildrens(const Upp::Vector<Transform*>& _childrens);
		Transform& RemoveChildrens(const Upp::Vector<Transform*>& _childrens);
		Transform& RemoveChildren(Transform& _ptr);
		int NumberOfChildrens();

		glm::mat4 GetModelMatrice()const;
		glm::mat4 GetViewMatrix()const;

		//******************Position part******************
		Transform& Move(glm::vec3 move,bool updateChildrens = true); //Move the position from the vec3 arg
		Transform& MoveFromAngle(glm::vec3 move,bool updateChildrens = true); //Move but care about the angle
		Transform& SetNewPosition(glm::vec3 newPosition,bool updateChildrens = true);//set the new position of object //It update all the child by : newPose + (old parent pose - old child pos);

		//******************Scale part*********************
		Transform& Scale(glm::vec3 scallar,bool updateChildrens=true);
		Transform& SetNewScale(glm::vec3 newScallar,bool updateChildrens=true); //we ensure the scall is made by mutiply 1.0f model matrix to new scallar

		//******************Rotation part******************
		Transform& Rotate(glm::quat quaterion,bool updateChildrens = true);
		Transform& SetNewRotation(glm::quat newQuaterion,bool updateChildrens = true);
		Transform& RotateFromAngles(float angleInDegree,glm::vec3 EulerAngles,bool updateChildrens = true);
		Transform& SetNewRotationFromAngles(float angleInDegree,glm::vec3 EulerAngles,bool updateChildrens = true);

		//******************Second Rotation part***********
		Transform& SecondRotate(glm::quat _quaterion,bool updateChildrens = true);
		Transform& SetNewSecondRotation(glm::quat newQuaterion,bool updateChildrens = true);
		Transform& SecondRotateFromAngles(float angleInDegree,glm::vec3 EulerAngles,bool updateChildrens = true);
		Transform& SetNewSecondRotationFromAngles(float angleInDegree,glm::vec3 EulerAngles,bool updateChildrens = true);

		//******************Quick function part*************
		Transform& LookAt(glm::vec3 const& lookTo,bool inverse = false,bool updateChildrens = true);
		Transform& ProcessMouseMouvement(float Yaw,float Pitch,float Roll,bool updateChildrens = true);
		Transform& UpdateByEuler(float Yaw,float Pitch,float Roll,bool updateChildrens = true);
		Transform& RecalculateFrontUpRight(bool updateChildrens = false);
			
		glm::quat QuatLookAt(glm::vec3 const& lookFrom,glm::vec3 const& lookTo,glm::vec3 const& up);
		glm::vec3 QuaterionToEuler(glm::quat const& quaterion);
		glm::quat EulerToQuaterion(float Yaw,float Pitch,float Roll);
		glm::vec3 ExtractPositionFromModelMatrix(const glm::mat4& modelMatrix);
		glm::quat CreateQuaterion(float AngleInRadians, glm::vec3 Dir);
};

*/
}
#endif
