#include "Object3D.h"
#include <Surface/Surface.h>
namespace Upp{
Object3D::Object3D(Surface& surface, Color color) : transform(){
	glm::vec3 col(color.GetR()/255.0f,color.GetG()/255.0f,color.GetB()/255.0f);
	Upp::Vector<float> vertexData;
	Upp::Vector<float> normalData;
	Upp::Vector<float> colorData;
	int indice =0;
	SurfaceCount =0;
	for (int ip = 0; ip < surface.panels.GetCount(); ++ip) {
		const Panel &panel = surface.panels[ip];
		//ADDING :
		if(panel.IsTriangle()){
			for(int e = 0; e < 3; e++){
				Point3D p = surface.nodes[panel.id[e]];
				vertexData.Add(p.x);
				vertexData.Add(p.y);
				vertexData.Add(p.z);
				normalData.Add(panel.normalPaint.x);
				normalData.Add(panel.normalPaint.y);
				normalData.Add(panel.normalPaint.z);
				colorData.Add(col.x);
				colorData.Add(col.y);
				colorData.Add(col.z);
				SurfaceCount++;
			}
		}else{//A quad is just 2 triangles
			Point3D SecondTriangle[3];
			for(int e = 0; e < 3; e++){
				
				Point3D p = surface.nodes[panel.id[e]];
				if(e == 0) SecondTriangle[2] = p;
				if(e == 2) SecondTriangle[0] = p;
				vertexData.Add(p.x);
				vertexData.Add(p.y);
				vertexData.Add(p.z);
				
				normalData.Add(panel.normalPaint.x);
				normalData.Add(panel.normalPaint.y);
				normalData.Add(panel.normalPaint.z);
				
				colorData.Add(col.x);
				colorData.Add(col.y);
				colorData.Add(col.z);
				SurfaceCount++;
			}
			SecondTriangle[1]  = surface.nodes[panel.id[3]];
			for(int e = 0; e < 3; e++){
				Point3D p = SecondTriangle[e];
				vertexData.Add(p.x);
				vertexData.Add(p.y);
				vertexData.Add(p.z);
				
				normalData.Add(panel.normalPaint.x);
				normalData.Add(panel.normalPaint.y);
				normalData.Add(panel.normalPaint.z);
				
				colorData.Add(col.x);
				colorData.Add(col.y);
				colorData.Add(col.z);
				SurfaceCount++;
			}
		}
	}
	BuildOpenGLData(vertexData,normalData,colorData);
}

Object3D::Object3D(Upp::Vector<float>& surface, Upp::Vector<float>& normal, Upp::Vector<float>& color){
	SurfaceCount = surface.GetCount() / 3;
	BuildOpenGLData(surface,normal,color);
}

void Object3D::BuildOpenGLData(Upp::Vector<float>& surface, Upp::Vector<float>& normal, Upp::Vector<float>& color){
	if(VAO > 0) glDeleteVertexArrays(1,&VAO);
	if(VerticesVBO > 0)glDeleteBuffers(1,&VerticesVBO);
	if(NormalVBO > 0)glDeleteBuffers(1,&NormalVBO);
	if(ColorVBO > 0)glDeleteBuffers(1,&ColorVBO);
	
	// OpenGL 4.0 compatibility :
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VerticesVBO);
	glGenBuffers(1, &NormalVBO);
	glGenBuffers(1, &ColorVBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VerticesVBO);
	glBufferData(GL_ARRAY_BUFFER,surface.GetCount() * sizeof(float),&(surface[0]),GL_DYNAMIC_READ);
	glBindBuffer(GL_ARRAY_BUFFER,NormalVBO);
	glBufferData(GL_ARRAY_BUFFER,normal.GetCount() * sizeof(float),&(normal[0]),GL_DYNAMIC_READ);
	glBindBuffer(GL_ARRAY_BUFFER,ColorVBO);
	glBufferData(GL_ARRAY_BUFFER,color.GetCount() * sizeof(float),&(color[0]),GL_DYNAMIC_READ);
	
	glBindBuffer(GL_ARRAY_BUFFER,VerticesVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER,NormalVBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER,ColorVBO);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	
	//OpenGL 4.5 compatibility :
	/*
	glGenVertexArrays(1, &VAO);
	
	glCreateBuffers(1, &VerticesVBO);
	glCreateBuffers(1, &NormalVBO);
	glCreateBuffers(1, &ColorVBO);

	glBindVertexArray(VAO);
	glNamedBufferStorage(VerticesVBO,vertexData.GetCount() * sizeof(float),&(vertexData[0]), GL_MAP_READ_BIT  | GL_MAP_WRITE_BIT);
	glNamedBufferStorage(NormalVBO,normalData.GetCount() * sizeof(float),&(normalData[0]), GL_MAP_READ_BIT  | GL_MAP_WRITE_BIT);
	glNamedBufferStorage(ColorVBO,colorData.GetCount() * sizeof(float),&(colorData[0]), GL_MAP_READ_BIT  | GL_MAP_WRITE_BIT);
	
	glBindBuffer(GL_ARRAY_BUFFER,VerticesVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER,NormalVBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER,ColorVBO);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	*/
}

bool Object3D::UpdateBuffer(GLuint buffer, int SurfaceNumber, int count,const float * data)noexcept{
	glBindBuffer(GL_ARRAY_BUFFER,buffer);
	float* ptr = (float*) glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
	//float* ptr = (float*) glMapNamedBuffer(buffer,GL_WRITE_ONLY);
	if(ptr){
		if(SurfaceNumber < SurfaceCount){
			ptr += SurfaceNumber;
			for(int i = 0; i < count; i++){
				for(int e = 0; e < 3; e++){
					*(ptr++) = data[e];
				}
			}
			glUnmapBuffer(GL_ARRAY_BUFFER);
			return true;
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		return false;
	}
	return false;
}
Vector<float> Object3D::ReadBuffer(GLuint buffer, int SurfaceNumber, int count)noexcept{
//	float* ptr = (float*) glMapNamedBuffer(buffer,GL_READ_ONLY);
	glBindBuffer(GL_ARRAY_BUFFER,buffer);
	float* ptr = (float*) glMapBuffer(GL_ARRAY_BUFFER,GL_READ_ONLY);
	Vector<float> data;
	if(ptr){
		if(SurfaceNumber < SurfaceCount){
			ptr += SurfaceNumber;
			for(int i = 0; i < count; i++){
				for(int e = 0; e < 3; e++){
					data.Add(*ptr);
				}
			}
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	return data;
}
bool Object3D::UpdateColor(unsigned int SurfaceNumber, int r, int g, int b)noexcept{
	float data[]= {r/255.0f,g/255.0f,b/255.0f};
	return UpdateBuffer(ColorVBO,SurfaceNumber, 1 ,data);
}
bool Object3D::UpdateColor(unsigned int SurfaceNumber, float r, float g, float b)noexcept{
	float data[] = {r,g,b};
	return UpdateBuffer(ColorVBO,SurfaceNumber, 1 ,data);
}
bool Object3D::UpdateColor(unsigned int SurfaceNumber, glm::vec3 color)noexcept{
	return UpdateBuffer(ColorVBO,SurfaceNumber, 1 ,&(color.x));
}
bool Object3D::UpdateColor(unsigned int SurfaceNumber, Upp::Color color)noexcept{
	float data[] = {color.GetR()/255.0f,color.GetG()/255.0f,color.GetB()/255.0f};
	return UpdateBuffer(ColorVBO,SurfaceNumber, 1 ,data);
}
bool Object3D::UpdateColors(unsigned int SurfaceNumber,int Count, const float * data)noexcept{
	return UpdateBuffer(ColorVBO,SurfaceNumber, Count ,data);
}
bool Object3D::UpdateNormal(unsigned int SurfaceNumber, float x, float y, float z)noexcept{
	float data[] = {x,y,z};
	return UpdateBuffer(NormalVBO,SurfaceNumber, 1 ,data);
}
bool Object3D::UpdateNormal(unsigned int SurfaceNumber, glm::vec3 normal)noexcept{
	return UpdateBuffer(NormalVBO,SurfaceNumber, 1 ,&(normal.x));
}
bool Object3D::UpdateNormals(unsigned int SurfaceNumber,int Count, const float * data)noexcept{
	return UpdateBuffer(NormalVBO,SurfaceNumber, Count ,data);
}
bool Object3D::UpdateVertice(unsigned int SurfaceNumber, float x, float y, float z)noexcept{
	float data[] = {x,y,z};
	return UpdateBuffer(VerticesVBO,SurfaceNumber, 1 ,data);
}
bool Object3D::UpdateVertice(unsigned int SurfaceNumber, glm::vec3 vertice)noexcept{
	return UpdateBuffer(VerticesVBO,SurfaceNumber, 1 ,&(vertice.x));
}
bool Object3D::UpdateVertices(unsigned int SurfaceNumber,int Count, const float * data)noexcept{
	return UpdateBuffer(VerticesVBO, SurfaceNumber, Count ,data);
}
Vector<float> Object3D::ReadColors(unsigned int SurfaceNumber, int count)noexcept{
	return ReadBuffer(ColorVBO,SurfaceNumber,count);
}
Vector<float> Object3D::ReadNormals(unsigned int SurfaceNumber, int count)noexcept{
	return ReadBuffer(NormalVBO,SurfaceNumber,count);
}
Vector<float> Object3D::ReadVertices(unsigned int SurfaceNumber, int count)noexcept{
	return ReadBuffer(VerticesVBO,SurfaceNumber,count);
}
void Object3D::Draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix,glm::vec3 viewPosition,OpenGLProgram& noLight, OpenGLProgram& light, OpenGLProgram& line,OpenGLProgram& normal)noexcept{
	if(SurfaceCount){
		glBindVertexArray(VAO);
		if(showMesh && noLight.IsLinked() && light.IsLinked()){
			OpenGLProgram&  prog = (showLight)? light : noLight;
			prog.Bind();
			if(showLight)prog.SetVec3("viewPos",viewPosition.x,viewPosition.y,viewPosition.z);
			prog.SetMat4("ViewMatrix", viewMatrix);
			prog.SetMat4("ProjectionMatrix", projectionMatrix);
			prog.SetMat4("ModelMatrix", transform.GetModelMatrice());
			glDrawArrays(((prog.ContainTCS()) ? GL_PATCHES : DrawType), 0, SurfaceCount);
		}
		if(showMeshLine && line.IsLinked()){
			line.Bind();
			line.SetMat4("ViewMatrix",viewMatrix);
			line.SetMat4("ProjectionMatrix",projectionMatrix);
			line.SetMat4("ModelMatrix",transform.GetModelMatrice());
			line.SetVec4("CustomColor", lineColor.GetR() / 255.0f, lineColor.GetG() / 255.0f, lineColor.GetB() / 255.0f, lineOpacity );
			glDrawArrays(((line.ContainTCS()) ? GL_PATCHES : GL_TRIANGLES), 0, SurfaceCount);
		}
		if(showMeshNormal && normal.IsLinked()){
			normal.Bind();
			normal.SetMat4("ViewMatrix", viewMatrix);
			normal.SetMat4("ProjectionMatrix", projectionMatrix);
			normal.SetMat4("ModelMatrix", transform.GetModelMatrice());
			normal.SetVec4("CustomColor",normalColor.GetR() / 255.0f, normalColor.GetG() / 255.0f, normalColor.GetB() / 255.0f, normalOpacity);
			normal.SetFloat("normal_length",normalLenght );
			glDrawArrays(GL_TRIANGLES, 0, SurfaceCount);
		}
	}
}
}