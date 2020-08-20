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
#include <plugin/glm/gtx/compatibility.hpp>
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
		glm::quat Rotation = Transform::GetQuaterion(0.0f,glm::vec3(0.0f,0.0f,0.0f));
		glm::vec3 Scale = glm::vec3(1.0f);
		
		void RecalculateFURW(){
			Front = glm::rotate(glm::inverse(Rotation), WorldFront);
		    Right = glm::rotate(glm::inverse(Rotation), WorldRight);
		    Up = glm::rotate(glm::inverse(Rotation), WorldUp);
		}
	public:
		Transform(){}
		Transform(const Transform& _transform){*this = _transform;}
		Transform& operator=(const Transform& _transform){Front = _transform.Front; Up = _transform.Up; Right = _transform.Right; WorldUp = _transform.WorldUp; WorldFront = _transform.WorldFront; WorldRight = _transform.WorldRight; Position = _transform.Position; Rotation = _transform.Rotation; Scale = _transform.Scale; return *this;}
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
	
		Transform& LookAt(const glm::vec3& LookAt, glm::vec3 customUp = glm::vec3(0.0f,0.0f,0.0f))noexcept{
			glm::vec3 direction = LookAt - Position;
			float directionLength = glm::length(direction);
			if(directionLength > 0.0001){
				direction /= directionLength;
				if(customUp != glm::vec3(0.0f,0.0f,0.0f))
					SetRotation(glm::quatLookAt(glm::normalize(direction), customUp));
				else
					SetRotation(glm::quatLookAt(glm::normalize(direction), Up));
			}
			return *this;
		}
			
		glm::mat4 GetViewMatrix()const noexcept{return glm::lookAt(Position,Position + Front, Up);}
		glm::mat4 GetModelMatrix()const noexcept{
			glm::mat4 ModelMatrix = glm::mat4(1.0f);
			ModelMatrix = glm::translate(ModelMatrix, Position); //position of cube
		    ModelMatrix = glm::scale(ModelMatrix, Scale); //Scale (not used so glm::vec3(1.0f))
			ModelMatrix *= glm::mat4_cast(Rotation);
			return ModelMatrix;
		}

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
}
#endif
