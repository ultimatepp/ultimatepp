#ifndef _SurfaceCtrl_Mesh_h_
#define _SurfaceCtrl_Mesh_h_

namespace Upp{
	
struct Material{
	private:
		glm::vec3 Diffuse = glm::vec3(0.30f, 0.30f, 0.30f);
		glm::vec3 Specular = glm::vec3(0.1f, 0.1f, 0.1f);
		float Shininess = 12.0f;
		bool update = true;
	public:
		Material(){}
		Material(const glm::vec3& diffuse_, const glm::vec3& speculare_, float shininess_){Diffuse = diffuse_;Specular = speculare_;Shininess = shininess_;}
		
		glm::vec3 GetDiffuse()const noexcept{return Diffuse;}
		glm::vec3 GetSpecular()const noexcept{return Specular;}
		float GetShininess()const noexcept{return Shininess;}
		
		Material& SetDiffuse(const glm::vec3& diffuse_)noexcept{Diffuse = diffuse_; update = true; return *this;}
		Material& SetSpecular(const glm::vec3& speculare_)noexcept{Specular = speculare_; update = true; return *this;}
		Material& SetShininess(float shininess_)noexcept{Shininess = shininess_; update = true; return *this;}
		
		bool ShouldBeUpdated()const noexcept{return update;}
		void HaveBeenUpdated()noexcept{update = false;}
};

	
//Mesh contain vertex  and how it's read
class Mesh : public Moveable<Mesh>{
	private:
		Upp::String name =""; //Possible name of Mesh
		bool loaded = false;
		
		Upp::Vector<GLfloat> vertices;
		Upp::Vector<GLfloat> normals;
		Upp::Vector<GLfloat> colors;
		Upp::Vector<GLfloat> texCoords;
		
		GLuint vao = 0;
		GLuint verticesVBO = 0;
		GLuint normalsVBO = 0;
		GLuint colorsVBO = 0;
		GLuint texCoordsVBO = 0;
		
		
		/**
			All stuff about texture
		**/
	public:
		Mesh(){}
		Mesh(Mesh&& obj){*this = pick(obj);}
		Mesh& operator=(Mesh&& obj){
			name = obj.name;
			loaded = obj.loaded;
			
			vertices = pick(obj.vertices);
			normals = pick(obj.normals);
			colors = pick(obj.colors);
			texCoords = pick(obj.texCoords);
			
			vao = obj.vao;
			verticesVBO = obj.verticesVBO;
			normalsVBO = obj.normalsVBO;
			colorsVBO = obj.colorsVBO;
			texCoordsVBO = obj.texCoordsVBO;
			
			obj.vertices.Clear();
			obj.normals.Clear();
			obj.colors.Clear();
			obj.texCoords.Clear();
			return *this;
		}
		
		//copy also VBO and VAO , wich mean when object is destroyed , every copy will
		//lost their 3D representation in OpenGL
		Mesh(const Mesh& obj){*this = obj;}
		Mesh& operator=(const Mesh& obj){
			name = obj.name;
			loaded = obj.loaded;
			
			vertices.Append(obj.vertices);
			normals.Append(obj.normals);
			colors.Append(obj.colors);
			texCoords.Append(obj.texCoords);
			
			vao = obj.vao;
			verticesVBO = obj.verticesVBO;
			normalsVBO = obj.normalsVBO;
			colorsVBO = obj.colorsVBO;
			texCoordsVBO = obj.texCoordsVBO;
			return *this;
		}
		~Mesh(){
			if(vertices.GetCount()){
				Clear(true);
			}
		}
		
		void SetName(const Upp::String& n){name = n;}
		Upp::String GetName()const noexcept{return name;}
		
		/*** Retireve all vector, to fille data ***/
		Upp::Vector<float>& GetVertices(){return vertices;}
		Upp::Vector<float>& GetNormals(){return normals;}
		Upp::Vector<float>& GetColors(){return colors;}
		Upp::Vector<float>& GetTexCoords(){return texCoords;}
		
		const Upp::Vector<float>& GetVertices()const noexcept{return vertices;}
		const Upp::Vector<float>& GetNormals()const noexcept{return normals;}
		const Upp::Vector<float>& GetColors()const noexcept{return colors;}
		const Upp::Vector<float>& GetTexCoords()const noexcept{return texCoords;}
		
		GLuint GetVAO()const noexcept{return vao;}
		GLuint GetVerticesVBO()const noexcept{return verticesVBO;}
		GLuint GetNormalsVBO()const noexcept{return normalsVBO;}
		GLuint GetColorsVBO()const noexcept{return colorsVBO;}
		GLuint GetTexCoordsVBO()const noexcept{return texCoordsVBO;}
		
		void Init()noexcept{
			if(!loaded){
				glGenVertexArrays(1, &vao);
				glGenBuffers(1,&verticesVBO);
				glGenBuffers(1,&normalsVBO);
				glGenBuffers(1,&colorsVBO);
				glGenBuffers(1,&texCoordsVBO);
			
				glBindVertexArray(vao);
				glBindBuffer(GL_ARRAY_BUFFER,verticesVBO);
				glBufferData(GL_ARRAY_BUFFER,vertices.GetCount() * sizeof(float),(vertices.GetCount() > 0)? &(vertices[0]): NULL,GL_DYNAMIC_READ);
				
				glBindBuffer(GL_ARRAY_BUFFER,normalsVBO);
				glBufferData(GL_ARRAY_BUFFER,normals.GetCount() * sizeof(float),(normals.GetCount() > 0)? &(normals[0]): NULL,GL_DYNAMIC_READ);
				
				glBindBuffer(GL_ARRAY_BUFFER,colorsVBO);
				glBufferData(GL_ARRAY_BUFFER,colors.GetCount() * sizeof(float),(colors.GetCount() > 0)? &(colors[0]): NULL,GL_DYNAMIC_READ);
				
				glBindBuffer(GL_ARRAY_BUFFER,texCoordsVBO);
				glBufferData(GL_ARRAY_BUFFER,texCoords.GetCount() * sizeof(float),(texCoords.GetCount() > 0)? &(texCoords[0]): NULL ,GL_DYNAMIC_READ);
				
				if(vertices.GetCount()){
					glBindBuffer(GL_ARRAY_BUFFER,verticesVBO);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
					glEnableVertexAttribArray(0);
				}
				if(normals.GetCount()){
					glBindBuffer(GL_ARRAY_BUFFER,normalsVBO);
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
					glEnableVertexAttribArray(1);
				}
				if(colors.GetCount()){
					glBindBuffer(GL_ARRAY_BUFFER,colorsVBO);
					glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
					glEnableVertexAttribArray(2);
				}
				if(texCoords.GetCount()){
					glBindBuffer(GL_ARRAY_BUFFER,texCoordsVBO);
					glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
					glEnableVertexAttribArray(3);
				}
					
				loaded = true;
			}
		}
		
		
		void Clear(bool ClearVector = false)noexcept{ //Clear openGL data
			if(vao > 0) glDeleteVertexArrays(1,&vao);
			if(verticesVBO > 0)glDeleteBuffers(1,&verticesVBO);
			if(normalsVBO > 0)glDeleteBuffers(1,&normalsVBO);
			if(colorsVBO > 0)glDeleteBuffers(1,&colorsVBO);
			if(texCoordsVBO > 0)glDeleteBuffers(1,&texCoordsVBO);
			vao = 0;
			verticesVBO = 0;
			normalsVBO = 0;
			colorsVBO = 0;
			texCoordsVBO = 0;
			if(ClearVector){
				vertices.Clear();
				normals.Clear();
				colors.Clear();
				texCoords.Clear();
			}
			loaded = false;
		}
		
	

	

};

}



#endif
