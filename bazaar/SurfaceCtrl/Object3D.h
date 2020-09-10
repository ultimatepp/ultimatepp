#ifndef _SurfaceCtrl_Object3D_h_
#define _SurfaceCtrl_Object3D_h_
#include "Definition.h"
#include "BoundingBox.h"
#include "Mesh.h"
#include "Transform.h"

namespace Upp{
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
struct Texture : public Moveable<Texture>{
	Upp::String name="";
	unsigned int id =0;
	
	String ToString()const{return "name=" + name + " | " + "id=" + AsString(id);}
	
	Texture& operator=(const Texture& t){
		name = t.name;
		id = t.id;
		return *this;
	}
	
};

class Object3D : public Upp::Moveable<Object3D>{
	private:
		static int GlobalID;
		int ID;

		Transform transform;
		BoundingBox boundingBox;
		Material material; //The material object is a representation of material property of the object (it change how light affect it)

		Vector<Mesh> meshes;
		Vector<Texture> textures; //Vector carrying all texture of the object, every meshes refer to it via one iterator
		Vector<OpenGLProgram> program;
		VectorMap<String,Value> objectValues; //In case data need to be used in Init() Clear() and Draw();

		bool loaded = false;
		bool moved = false;
		bool visible = true;
		bool showBoundingBox = false;

		int programNoLight = 0;//The program will draw figure without light
		int programLine =    0;//THe program will draw figure line
		int programNormal =  0;//The program will draw Normal
		int programLight =   0;//the program will draw figure with light
		
		GLenum drawType = GL_TRIANGLES;
		
		 //All this value are here by default
		Color lineColor = Black();
		float lineOpacity = 0.5f;
		float lineWidth = 1.0f;
		
		Color normalColor = Red();
		float normalOpacity = 1.0f;
		float normalLength = 1.0f;

		bool showMesh = true;
		bool showMeshLine = false;
		bool showMeshNormal = false;
		bool showLight = true;
	public:
		Function <void(Object3D& obj)> WhenInit;
		Function <void(const glm::mat4& projectionMatrix,const glm::mat4& viewMatrix,const glm::vec3& viewPosition,Object3D& obj)> WhenDraw;
		Function <void(Object3D& obj)> WhenClear;
		Function <void(Object3D& obj)> WhenReload;
	private:
		bool UpdateBuffer(GLuint buffer, int SurfaceCount , int SurfaceNumber,int count ,int numberOfElement, const float * data)noexcept;
		Vector<float> ReadBuffer(GLuint buffer, int SurfaceCount , int SurfaceNumber,int count, int numberOfElement)noexcept;
		//Texture loading function
		//Return position of the texture object carrying this texture
		int LoadTexture(const Image& img , const String& name, int indiceWhereInsert = -1);
		
		//Assimp loading function
		bool InitFromScene(const aiScene* pScene, const String& Filename);
	    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	    bool InitMaterials(const aiScene* pScene, const String& Filename);
	public:
		Object3D();
		Object3D(Object3D&& obj):ID(GlobalID++){*this = pick(obj);}
		Object3D& operator=(Object3D&& obj);	//move will prevent your object to be deleted (from OpenGL perspective)
		Object3D(const Object3D& obj):ID(GlobalID++){*this = obj;}
		Object3D& operator=(const Object3D& obj);//copy also VBO and VAO , what mean when object is destroyed , every copy will lost their 3D representation in OpenGL
		~Object3D();

		int GetID()const{return ID;}
		
		bool IsLoaded(){return loaded;}
		Object3D& Show(){visible = true;return *this;}
		Object3D& Hide(){visible = false;return *this;}
		Object3D& SetVisible(bool b = true){visible = b; return *this;}
		bool& IsVisible(){return visible;}
		
		bool GetShowBoundingBox()const noexcept{return showBoundingBox;}
		Object3D& ShowBoundingBox(bool b = true)noexcept{showBoundingBox = b; return *this;}
		
		Transform& GetTransform()noexcept{return transform;}
		const Transform& GetTransform()const noexcept{return transform;}

		BoundingBox& GetBoundingBox(){return boundingBox;}
		BoundingBox GetBoundingBoxTransformed()const noexcept{BoundingBox box(boundingBox);return box.TransformBy(transform.GetModelMatrix());}

		Material& GetMaterial()noexcept{return material;}
		const Material& GetMaterial()const noexcept{return material;}

		const Upp::Vector<Mesh>& GetMeshes() const noexcept{return meshes;}
		Upp::Vector<Mesh>& GetMeshes()noexcept{return meshes;}
		
		const Vector<Texture>& GetTextures()const noexcept{return textures;}
		Vector<Texture>& GetTextures()noexcept{return textures;}

		const Vector<OpenGLProgram>& GetProgram()const noexcept{return program;}
		Vector<OpenGLProgram>& GetProgram()noexcept{return program;}

		const VectorMap<String,Value>& GetObjectValues()const noexcept{return objectValues;}
		VectorMap<String,Value>& GetObjectValues()noexcept{return objectValues;}
		
		Object3D& SetDrawType(GLenum drawtype)noexcept{drawType = drawtype;return *this;}
		GLenum GetDrawType()const noexcept{return drawType;}
		
		/** LoadModel load multiple kind of object file using Assimp lib, you can specify some custom
		load routine by editing pFlags, if pFlags = 0 then SurfaceCtrl default behavior about assimp will be used**/
		Object3D& LoadModel(const String& FileObj, Color color = Gray(), int alpha =255, unsigned int pFlags = 0);
		Object3D& LoadSurface(Surface& surface, Upp::Color = Green(), int alpha =255);
		Surface GetSurface();
		
		//Check if texture provided is already load, then return the iterator of the texture Object3D
		int InsertTexture(const String& filename,int indice = -1, FlipMode flipmode = FLIP_NONE)noexcept; //insert texture in object3D
		int InsertTexture(const Image& m,int indice = -1, FlipMode flipmode = FLIP_NONE)noexcept; //insert texture in object3D
		int InsertTexture(const TexturesMaterial& tm,int indice = -1, FlipMode flipmode = FLIP_NONE)noexcept; //Insert one of SurfaceCtrl provided texture
		
		const Texture& GetTexture(int indice){if(indice < textures.GetCount()) return textures[indice];else throw Exc("int indice higher than textures.getCount()");}//Return the indice, Can throw exception

		Object3D& AttachTexture(int TexNo,int MeshNo, int NumberMeshToAffect =1);//Attach a texture to the range of mesh
		Object3D& GenerateTextureCoordinate(int MeshNo, int NumberMeshToAffect =1, bool CustomTextureCoordinate = false,const Vector<float>& tc = Vector<float>());// Generate new Texture coordinate for the selected range of mesh

		Mesh& CreateMesh()noexcept{return meshes.Add();}
		
		Object3D& SetLineColor(Color color)noexcept{lineColor = color; return *this;}
		Object3D& SetNormalColor(Color color)noexcept{normalColor = color; return *this;}
		Object3D& ShowMesh(bool b = true)noexcept{showMesh = b; return *this;}
		Object3D& ShowMeshLine(bool b = true)noexcept{showMeshLine = b; return *this;}
		Object3D& ShowMeshNormal(bool b = true)noexcept{showMeshNormal = b; return *this;}
		Object3D& ShowLight(bool b = true)noexcept{showLight = b; return *this;}
		Object3D& SetNormalLength(float length)noexcept{normalLength = length; return *this;}
		Object3D& SetLineOpacity(float opacity)noexcept{lineOpacity = opacity; return *this;}
		Object3D& SetLineWidth(float width)noexcept{lineWidth = width; return *this;}
		Object3D& SetNormalOpacity(float opacity)noexcept{normalOpacity = opacity; return *this;}
		Object3D& SetProgramNoLight(const OpenGLProgram& prog)noexcept{programNoLight = program.GetCount(); program.Add(prog); return *this;}
		Object3D& SetProgramLight(const OpenGLProgram& prog)noexcept{programLight = program.GetCount(); program.Add(prog); return *this;}
		Object3D& SetProgramLine(const OpenGLProgram& prog)noexcept{programLine = program.GetCount(); program.Add(prog); return *this;}
		Object3D& SetProgramNormal(const OpenGLProgram& prog)noexcept{programNormal = program.GetCount(); program.Add(prog); return *this;}

		Upp::Color GetLineColor()const noexcept{return lineColor;}
		Upp::Color GetNormalColor()const noexcept{return normalColor;}
		bool GetShowMesh()const noexcept{return showMesh;}
		bool GetShowMeshLine()const noexcept{return showMeshLine;}
		bool GetShowMeshNormal()const noexcept{return showMeshNormal;}
		bool GetShowLight()const noexcept{return showLight;}
		float GetNormalOpacity()const noexcept{return normalOpacity;}
		float GetNormalLength()const noexcept{return normalLength;}
		float GetLineOpacity()const noexcept{return lineOpacity;}
		float GetLineWidth()const noexcept{return lineWidth;}
		int GetProgramNoLight(){return programNoLight;}
		int GetProgramLight(){return programLight;}
		int GetProgramLine(){return programLine;}
		int GetProgramNormal(){return programNormal;}
		
		void CreateBoundingBox(); // Create Bounding box
		void RemoveBoundingBox(); // Delete the bounding box
		bool TestLineIntersection(const glm::vec3 & start, const glm::vec3 & end)const{BoundingBox box  = Upp::pick(GetBoundingBoxTransformed());return box.LineIntersection(start,end);}

		/*Write into OpenGL buffer return true if operation have been done correctly.
		/!\ True do not mean OpenGL will provide good things, it just mean buffer have been
		writted  correctly*/
		bool UpdateColor(unsigned int MeshNo, unsigned int SurfaceNumber, int r, int g, int b,int alpha)noexcept;
		bool UpdateColor(unsigned int MeshNo, unsigned int SurfaceNumber, float r, float g, float b,float alpha)noexcept;
		bool UpdateColor(unsigned int MeshNo, unsigned int SurfaceNumber, glm::vec3 color,float alpha)noexcept;
		bool UpdateColor(unsigned int MeshNo, unsigned int SurfaceNumber, Upp::Color color,int alpha)noexcept;
		bool UpdateColors(unsigned int MeshNo, unsigned int SurfaceNumber,int Count, const float * data)noexcept;
		
		bool UpdateNormal(unsigned int MeshNo, unsigned int SurfaceNumber, float x, float y, float z)noexcept;
		bool UpdateNormal(unsigned int MeshNo, unsigned int SurfaceNumber, glm::vec3 normal)noexcept;
		bool UpdateNormals(unsigned int MeshNo, unsigned int SurfaceNumber,int Count, const float * data)noexcept;
		
		bool UpdateVertice(unsigned int MeshNo, unsigned int SurfaceNumber, float x, float y, float z)noexcept;
		bool UpdateVertice(unsigned int MeshNo, unsigned int SurfaceNumber, glm::vec3 vertice)noexcept;
		bool UpdateVertices(unsigned int MeshNo, unsigned int SurfaceNumber,int Count, const float * data)noexcept;
		
		//Read OpenGL buffer and return data in Upp::Vector
		Vector<float> ReadColors(int MeshNo, unsigned int SurfaceNumber, int count);
		Vector<float> ReadNormals(int MeshNo, unsigned int SurfaceNumber, int count);
		Vector<float> ReadVertices(int MeshNo, unsigned int SurfaceNumber, int count);
		
		void DefaultInit(Object3D& obj);
		void DefaultDraw(const glm::mat4& projectionMatrix,const glm::mat4& viewMatrix,const glm::vec3& viewPosition,Object3D& obj)noexcept;
		void DefaultClear(Object3D& obj);
		void DefaultReload(Object3D& obj);
			
		void Init(){ WhenInit(*this); loaded = true;} //Load all data in graphic memory It's called automaticly by using Load function, but you must call it if you set manually all data
		void Draw(const glm::mat4& projectionMatrix,const glm::mat4& viewMatrix,const glm::vec3& viewPosition)noexcept{WhenDraw(projectionMatrix,viewMatrix,viewPosition, *this);}
		void Clear(){WhenClear(*this); loaded = false;}
		void Reload(){WhenReload(*this);}
};

class Skybox {
	private:
		unsigned int ID = 0;
		GLuint VBO, VAO;
		bool show = true;
		
		OpenGLProgram program;
		Skybox& Init(const Vector<Image>& images); //Load this vector of image as Skybox
	public:
		
		Skybox(){}
		~Skybox(){Clear();}
		
		Skybox& Show(bool b = true){show = b; return *this;}
		bool IsShown(){return show;}

		Skybox& Init(const Image& skybox_right,const Image& skybox_left,const Image& skybox_top,const Image& skybox_bottom,const Image& skybox_front,const Image& skybox_back); //Load all image provided as skybox
		Skybox& Clear();
		Skybox& Draw(const glm::mat4& projectionMatrix,const glm::mat4& viewMatrix);

};
	
}

#endif
