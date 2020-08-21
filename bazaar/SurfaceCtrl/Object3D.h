#ifndef _SurfaceCtrl_Object3D_h_
#define _SurfaceCtrl_Object3D_h_
#include <Core/Core.h>
#include <GLCtrl_glad/GLCtrl_glad.h>
#include <Surface/Surface.h>
#include "Transform.h"
#include "Shader.h"
#include "BoundingBox.h"

#include "STLLoader.h"

namespace Upp{
enum DrawType { DT_TRIANGLE, DT_QUAD };
enum MaterialType { MT_COLOR, MT_TEXTURE };
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



struct Object3DMaterial{
	private:
		glm::vec3 Diffuse = glm::vec3(0.30f, 0.30f, 0.30f);
		glm::vec3 Specular = glm::vec3(0.1f, 0.1f, 0.1f);
		float Shininess = 12.0f;
		bool update = true;
	public:
		Object3DMaterial(){}
		Object3DMaterial(const glm::vec3& diffuse_, const glm::vec3& speculare_, float shininess_){Diffuse = diffuse_;Specular = speculare_;Shininess = shininess_;}
		
		glm::vec3 GetDiffuse()const noexcept{return Diffuse;}
		glm::vec3 GetSpecular()const noexcept{return Specular;}
		float GetShininess()const noexcept{return Shininess;}
		
		Object3DMaterial& SetDiffuse(const glm::vec3& diffuse_)noexcept{Diffuse = diffuse_; update = true; return *this;}
		Object3DMaterial& SetSpecular(const glm::vec3& speculare_)noexcept{Specular = speculare_; update = true; return *this;}
		Object3DMaterial& SetShininess(float shininess_)noexcept{Shininess = shininess_; update = true; return *this;}
		
		bool ShouldBeUpdated()const noexcept{return update;}
		void HaveBeenUpdated()noexcept{update = false;}
};
class Surface;

class Object3D : public Upp::Moveable<Object3D>{
	private:
		static int GlobalID;
		
		int ID;
		
		GLuint VerticesVBO = 0;
		GLuint ColorVBO = 0;
		GLuint NormalVBO = 0;
		
		GLuint VAO = 0;
		
		bool loaded = false;
		
		Upp::Vector<float> verticesData;
		Upp::Vector<float> normalsData;
		Upp::Vector<float> colorsData;
		Upp::Vector<float> texturesData; //Texture coordinate
		
		Object3DMaterial material; //The material object is a representation of material property of the object (it change how light affect it)
		
		Color lineColor = Black();
		float lineOpacity = 0.5f;
		float lineWidth = 1.0f;
		
		Color normalColor = Red();
		float normalOpacity = 0.5f;
		float normalLenght = 1.0f;
		
		int SurfaceCount = 0;

		bool showMesh = true;
		bool showMeshLine = false;
		bool showMeshNormal = false;
		bool showLight = true;
		
		
		BoundingBox boundingBox;
		bool showBoundingBox = false;
			
		Transform transform;
		
		GLenum DrawType = GL_TRIANGLES;
	
		bool UpdateBuffer(GLuint buffer, int SurfaceNumber,int count , const float * data)noexcept;
		Vector<float> ReadBuffer(GLuint buffer, int SurfaceNumber,int count)noexcept;
	public:
		Object3D():ID(GlobalID++){}
		Object3D(Object3D&& obj):ID(GlobalID++){*this = pick(obj);}
		Object3D& operator=(Object3D&& obj);
		
		Object3D(const Object3D& obj):ID(GlobalID++){*this = obj;}
		Object3D& operator=(const Object3D& obj);
		~Object3D();
	
		//copy also VBO and VAO , wich mean when object is destroyed , every copy will
		//lost their 3D representation in OpenGL
		
		
		bool LoadObj(const String& FileObj);
		bool LoadStl(const String& StlFile, Upp::Color = Green());
		bool LoadSurface(Surface& surface, Upp::Color = Green());
		Surface GetSurface();
		
		int GetID()const {return ID;}
		
		Upp::Vector<float>& GetVerticesData()noexcept{return verticesData;}
		Upp::Vector<float>& GetNormalsData()noexcept{return normalsData;}
		Upp::Vector<float>& GetColorsData()noexcept{return colorsData;}
		Upp::Vector<float>& GetTexturesData()noexcept{return texturesData;}
		
		Object3D& AddVerticesData(const Upp::Vector<float>& data)noexcept{verticesData.Append(data); SurfaceCount = verticesData.GetCount() / 3; return *this;}
		Object3D& AddNormalsData(const Upp::Vector<float>& data)noexcept{normalsData.Append(data); return *this;}
		Object3D& AddColorsData(const Upp::Vector<float>& data)noexcept{colorsData.Append(data); return *this;}
		Object3D& AddTexturesData(const Upp::Vector<float>& data)noexcept{texturesData.Append(data); return *this;}
		
		bool Load(const MaterialType& mt); //Load all data in graphic memory It's called automaticly by using Load function, but you must call it if you set manually all data
		Object3D& Unload();
		
		Object3D& SetDrawType(GLenum dt)noexcept{DrawType = dt; return *this;}
		Object3D& ShowMesh(bool b = true)noexcept{showMesh = b; return *this;}
		Object3D& ShowMeshLine(bool b = true)noexcept{showMeshLine = b; return *this;}
		Object3D& ShowMeshNormal(bool b = true)noexcept{showMeshNormal = b; return *this;}
		Object3D& ShowLight(bool b = true)noexcept{showLight = b; return *this;}
		Object3D& ShowBoundingBox(bool b = true)noexcept{showBoundingBox = b; return *this;}
		
		bool GetShowMesh()const noexcept{return showMesh;}
		bool GetShowMeshLine()const noexcept{return showMeshLine;}
		bool GetShowMeshNormal()const noexcept{return showMeshNormal;}
		bool GetShowLight()const noexcept{return showLight;}
		bool GetShowBoundingBox()const noexcept{return showBoundingBox;}
		
		Object3D& SetLineColor(Color color)noexcept{lineColor = color; return *this;}
		Object3D& SetNormalColor(Color color)noexcept{normalColor = color; return *this;}
		Object3D& SetLineOpacity(float opacity)noexcept{lineOpacity = opacity; if(lineOpacity < 0) lineOpacity =0; if(lineOpacity> 1.0f) lineOpacity = 1.0f; return *this;}
		Object3D& SetLineWidth(float width)noexcept{lineWidth = width; if(lineWidth < 1) lineWidth = 1.0f; return *this;}
		Object3D& SetNormalOpacity(float opacity)noexcept{normalOpacity = opacity; if(normalOpacity < 0) normalOpacity =0; if(normalOpacity> 1.0f) normalOpacity = 1.0f; return *this;}
		Upp::Color GetLineColor()const noexcept{return lineColor;}
		Upp::Color GetNormalColor()const noexcept{return normalColor;}
		float GetNormalOpcaity()const noexcept{return normalOpacity;}
		float GetLineOpcaity()const noexcept{return lineOpacity;}
		float GetLineWidth()const noexcept{return lineWidth;}
		GLenum GetDrawType()const noexcept{return DrawType;}
		
		Object3DMaterial& GetMaterial(){return material;}
		
		Transform& GetTransform()noexcept{return transform;}
		const Transform& GetTransform()const noexcept{return transform;}
		
		unsigned int GetNumberOfSurface()noexcept{return SurfaceCount;}
		GLuint GetVAO()noexcept{return VAO;}
		unsigned int GetSurfaceCount()noexcept{return SurfaceCount;}
		
		void CreateBoundingBox(Upp::Vector<float>& surface); // Create Bounding box
		void RemoveBoundingBox();
		
		BoundingBox& GetBoundingBox(){return boundingBox;}
		BoundingBox GetBoundingBoxTransformed()const noexcept{
			BoundingBox box(boundingBox);
			return box.TransformBy(transform.GetModelMatrix());
		}
		
		bool TestLineIntersection(const glm::vec3 & start, const glm::vec3 & end)const{
			BoundingBox box  = Upp::pick(GetBoundingBoxTransformed());
			return box.LineIntersection(start,end);
		}
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
