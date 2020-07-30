#ifndef _SurfaceCtrl_Object3D_h_
#define _SurfaceCtrl_Object3D_h_
#include <Core/Core.h>
#include <GLCtrl_glad/GLCtrl_glad.h>
#include <Surface/Surface.h>
#include "Transform.h"
#include "Shader.h"
namespace Upp{
enum DrawType { DT_TRIANGLE, DT_QUAD };
/*
struct Light{
	Light(){
		Position =vec3( 1.2, 80.0, 2.0);
		Ambient =vec3( 0.9, 0.9, 0.9);
		Diffuse =vec3( 0.5, 0.5, 0.5);
		Specular =vec3( 0.0, 1.0, 1.0);
	}
	Light(glm::vec3 _position, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular){
		Position = _position;
	    Ambient  = _ambient;
	    Diffuse  = _diffuse;
	    Specular = _specular;
	}
	
	glm::vec3 Position;
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
};*/
class Surface;
class Object3D : public Upp::Moveable<Object3D>{
	private:
		GLuint VerticesVBO = 0;
		GLuint ColorVBO = 0;
		GLuint NormalVBO = 0;
		
		GLuint VAO = 0;
		
		Color lineColor = Black();
		float lineOpacity = 0.5f;
		
		Color normalColor = Red();
		float normalOpacity = 0.5f;
		float normalLenght = 1.0f;
		
		unsigned int SurfaceCount = 0;
		VolumeEnvelope *env = nullptr;
		
		bool showMesh = true;
		bool showMeshLine = true;
		bool showMeshNormal = false;
		bool showLight = true;
			
		Transform transform;
		
		bool UpdateBuffer(GLuint buffer, int SurfaceNumber,int count , const float * data)noexcept;
		Vector<float> ReadBuffer(GLuint buffer, int SurfaceNumber,int count)noexcept;
	public:
		Object3D(Surface& surface,Upp::Color = Green());
				
		Object3D& ShowMesh(bool b = true)noexcept{showMesh = b; return *this;}
		Object3D& ShowMeshLine(bool b = true)noexcept{showMeshLine = b; return *this;}
		Object3D& ShowMeshNormal(bool b = true)noexcept{showMeshNormal = b; return *this;}
		Object3D& ShowLight(bool b = true)noexcept{showLight = b; return *this;}
		
		bool GetShowMesh()const noexcept{return showMesh;}
		bool GetShowMeshLine()const noexcept{return showMeshLine;}
		bool GetShowMeshNormal()const noexcept{return showMeshNormal;}
		bool GetShowLight()const noexcept{return showLight;}
		
		Object3D& SetLineColor(Color color)noexcept{lineColor = color; return *this;}
		Object3D& SetNormalColor(Color color)noexcept{normalColor = color; return *this;}
		Object3D& SetLineOpacity(float opacity)noexcept{lineOpacity = opacity; if(lineOpacity < 0) lineOpacity =0; if(lineOpacity> 1.0f) lineOpacity = 1.0f; return *this;}
		Object3D& SetNormalOpacity(float opacity)noexcept{normalOpacity = opacity; if(normalOpacity < 0) normalOpacity =0; if(normalOpacity> 1.0f) normalOpacity = 1.0f; return *this;}
		Upp::Color GetLineColor()const noexcept{return lineColor;}
		Upp::Color GetNormalColor()const noexcept{return normalColor;}
		float GetNormalOpcaity()const noexcept{return normalOpacity;}
		float GetLineOpcaity()const noexcept{return lineOpacity;}
				
		Transform& GetTransform()noexcept{return transform;}
		unsigned int GetNumberOfSurface()noexcept{return SurfaceCount;}
		GLuint GetVAO()noexcept{return VAO;}
		unsigned int GetSurfaceCount()noexcept{return SurfaceCount;}
		
		VolumeEnvelope& GetVolumeEnvelope(){ASSERT_(env,"GetVolumeEnvelope return nullptr !"); return *env;}
		Object3D& SetVolumeEnvelope(VolumeEnvelope& envelope){env = &envelope;return *this;}
		/*
			Write into OpenGL buffer return true if operation have been done correctly.
			/!\ True do not mean OpenGL will provide good things, it just mean buffer have been
			    writted  correctly
		*/
		bool UpdateColor(unsigned int SurfaceNumber, int r, int g, int b)noexcept;
		bool UpdateColor(unsigned int SurfaceNumber, float r, float g, float b)noexcept;
		bool UpdateColor(unsigned int SurfaceNumber, glm::vec3 color)noexcept;
		bool UpdateColor(unsigned int SurfaceNumber, Upp::Color color)noexcept;
		bool UpdateColors(unsigned int SurfaceNumber,int Count, const float * data)noexcept;
		
		bool UpdateNormal(unsigned int SurfaceNumber, float x, float y, float z)noexcept;
		bool UpdateNormal(unsigned int SurfaceNumber, glm::vec3 normal)noexcept;
		bool UpdateNormals(unsigned int SurfaceNumber,int Count, const float * data)noexcept;
		
		bool UpdateVertice(unsigned int SurfaceNumber, float x, float y, float z)noexcept;
		bool UpdateVertice(unsigned int SurfaceNumber, glm::vec3 vertice)noexcept;
		bool UpdateVertices(unsigned int SurfaceNumber,int Count, const float * data)noexcept;
		/*
			Read OpenGL buffer and return data in Upp::Vector
		*/
		Vector<float> ReadColors(unsigned int SurfaceNumber, int count)noexcept;
		Vector<float> ReadNormals(unsigned int SurfaceNumber, int count)noexcept;
		Vector<float> ReadVertices(unsigned int SurfaceNumber, int count)noexcept;
		
		void Draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::vec3 viewPosition, OpenGLProgram& noLight, OpenGLProgram& light, OpenGLProgram& line,OpenGLProgram& normal)noexcept;
};
	
}

#endif
