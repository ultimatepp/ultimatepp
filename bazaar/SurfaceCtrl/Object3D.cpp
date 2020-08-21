#include "Object3D.h"
#include <Surface/Surface.h>
namespace Upp{
int Object3D::GlobalID = 0;

Object3D& Object3D::operator=(Object3D&& obj){
	VerticesVBO = obj.VerticesVBO;
	ColorVBO = obj.ColorVBO;
	NormalVBO = obj.NormalVBO;
	VAO = obj.VAO;
	
	material = obj.material; //The material object is a representation of material property of the object (it change how light affect it)
	lineColor = obj.lineColor;
	lineOpacity = obj.lineOpacity;
	lineWidth = obj.lineWidth;
	normalColor = obj.normalColor;
	normalOpacity = obj.normalOpacity;
	normalLenght = obj.normalLenght;
	SurfaceCount = obj.SurfaceCount;
	showMesh = obj.showMesh;
	showMeshLine = obj.showMeshLine;
	showMeshNormal = obj.showMeshNormal;
	showLight = obj.showLight;
	boundingBox = obj.boundingBox;
	showBoundingBox = obj.showBoundingBox;
	transform = obj.transform;
	DrawType = obj.DrawType;
	
	verticesData.Append(obj.verticesData);
	normalsData.Append(obj.normalsData);
	colorsData.Append(obj.colorsData);
	texturesData.Append(obj.texturesData);
	
	obj.VerticesVBO = 0;
	obj.ColorVBO = 0;
	obj.NormalVBO = 0;
	obj.VAO = 0;
	obj.verticesData.Clear();
	obj.normalsData.Clear();
	obj.colorsData.Clear();
	obj.texturesData.Clear();
	obj.SurfaceCount = 0;
	obj.loaded = false;
	return *this;
}

Object3D& Object3D::operator=(const Object3D& obj){
	VerticesVBO = obj.VerticesVBO;
	ColorVBO = obj.ColorVBO;
	NormalVBO = obj.NormalVBO;
	VAO = obj.VAO;
	
	material = obj.material; //The material object is a representation of material property of the object (it change how light affect it)
	lineColor = obj.lineColor;
	lineOpacity = obj.lineOpacity;
	lineWidth = obj.lineWidth;
	normalColor = obj.normalColor;
	normalOpacity = obj.normalOpacity;
	normalLenght = obj.normalLenght;
	SurfaceCount = obj.SurfaceCount;
	showMesh = obj.showMesh;
	showMeshLine = obj.showMeshLine;
	showMeshNormal = obj.showMeshNormal;
	showLight = obj.showLight;
	boundingBox = obj.boundingBox;
	showBoundingBox = obj.showBoundingBox;
	transform = obj.transform;
	DrawType = obj.DrawType;
	
	verticesData.Append(obj.verticesData);
	normalsData.Append(obj.normalsData);
	colorsData.Append(obj.colorsData);
	texturesData.Append(obj.texturesData);
	
	return *this;
}

Object3D::~Object3D(){
	Unload();
}

bool Object3D::LoadObj(const String& FileObj){
	return false;
}
bool Object3D::LoadStl(const String& StlFile, Color color){
	verticesData.Clear();
	normalsData.Clear();
	colorsData.Clear();
	texturesData.Clear();
	if(STLLoader::LoadSTL(StlFile,verticesData,normalsData)){
		float r = color.GetR()/255.0f;
		float g = color.GetG()/255.0f;
		float b = color.GetB()/255.0f;
		for(int e = 0; e < (verticesData.GetCount() -1) ; e = e + 3){
			colorsData << r << g << b ;
		}
		SurfaceCount = verticesData.GetCount() / 3;
		Load(MT_COLOR);
		return true;
	}
	return false;
}
bool Object3D::LoadSurface(Surface& surface, Color color){
	glm::vec3 col(color.GetR()/255.0f,color.GetG()/255.0f,color.GetB()/255.0f);
	int indice =0;
	SurfaceCount =0;
	for (int ip = 0; ip < surface.panels.GetCount(); ++ip) {
		const Panel &panel = surface.panels[ip];
		//ADDING :
		if(panel.IsTriangle()){
			for(int e = 0; e < 3; e++){
				Point3D p = surface.nodes[panel.id[e]];
				verticesData.Add(p.x);
				verticesData.Add(p.y);
				verticesData.Add(p.z);
				normalsData.Add(panel.normalPaint.x);
				normalsData.Add(panel.normalPaint.y);
				normalsData.Add(panel.normalPaint.z);
				colorsData.Add(col.x);
				colorsData.Add(col.y);
				colorsData.Add(col.z);
				SurfaceCount++;
			}
		}else{//A quad is just 2 triangles
			Point3D SecondTriangle[3];
			for(int e = 0; e < 3; e++){
				Point3D p = surface.nodes[panel.id[e]];
				if(e == 0) SecondTriangle[2] = p;
				if(e == 2) SecondTriangle[0] = p;
				verticesData.Add(p.x);
				verticesData.Add(p.y);
				verticesData.Add(p.z);
				
				normalsData.Add(panel.normalPaint.x);
				normalsData.Add(panel.normalPaint.y);
				normalsData.Add(panel.normalPaint.z);
				
				colorsData.Add(col.x);
				colorsData.Add(col.y);
				colorsData.Add(col.z);
				SurfaceCount++;
			}
			SecondTriangle[1]  = surface.nodes[panel.id[3]];
			for(int e = 0; e < 3; e++){
				Point3D p = SecondTriangle[e];
				verticesData.Add(p.x);
				verticesData.Add(p.y);
				verticesData.Add(p.z);
				
				normalsData.Add(panel.normalPaint.x);
				normalsData.Add(panel.normalPaint.y);
				normalsData.Add(panel.normalPaint.z);
				
				colorsData.Add(col.x);
				colorsData.Add(col.y);
				colorsData.Add(col.z);
				SurfaceCount++;
			}
		}
	}
	Load(MT_COLOR);
	return true;
}

Surface Object3D::GetSurface(){ //return a surface Objectd
	Surface surf;
	for(int e = 0; e < verticesData.GetCount(); e = e+9){
		Panel &panel = surf.panels.Add();
		
		panel.normalPaint.x = double(normalsData[e]);
		panel.normalPaint.y = double(normalsData[e+1]);
		panel.normalPaint.z = double(normalsData[e+2]);
		
		Point3D &node0 = surf.nodes.Add();
		node0.x = double(verticesData[e]);
		node0.y = double(verticesData[e+1]);
		node0.z = double(verticesData[e+2]);
		panel.id[0] = surf.nodes.GetCount()-1;
		
		Point3D &node1 = surf.nodes.Add();
		node1.x = double(verticesData[e+3]);
		node1.y = double(verticesData[e+4]);
		node1.z = double(verticesData[e+5]);
		panel.id[1] = surf.nodes.GetCount()-1;
		
		Point3D &node2 = surf.nodes.Add();
		node2.x = double(verticesData[e+6]);
		node2.y = double(verticesData[e+7]);
		node2.z = double(verticesData[e+8]);
		panel.id[2] = surf.nodes.GetCount()-1;
		panel.id[3] = panel.id[0];
	}
	return pick(surf);
}

bool Object3D::Load(const MaterialType& mt){ //Load all data in graphic memory It's called automaticly by using Load function, but you must call it if you set manually all data
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
	glBufferData(GL_ARRAY_BUFFER,verticesData.GetCount() * sizeof(float),&(verticesData[0]),GL_DYNAMIC_READ);
	
	glBindBuffer(GL_ARRAY_BUFFER,NormalVBO);
	glBufferData(GL_ARRAY_BUFFER,normalsData.GetCount() * sizeof(float),&(normalsData[0]),GL_DYNAMIC_READ);
	
	glBindBuffer(GL_ARRAY_BUFFER,VerticesVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER,NormalVBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	
	if(mt == MT_COLOR){
		glBindBuffer(GL_ARRAY_BUFFER,ColorVBO);
		glBufferData(GL_ARRAY_BUFFER,colorsData.GetCount() * sizeof(float),&(colorsData[0]),GL_DYNAMIC_READ);
	
		glBindBuffer(GL_ARRAY_BUFFER,ColorVBO);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(2);
	}else{
		//Texturing
	}
	CreateBoundingBox(verticesData);
	loaded = true;
	return true;
}
Object3D& Object3D::Unload(){
	if(SurfaceCount > 0){
		if(VAO > 0) glDeleteVertexArrays(1,&VAO);
		if(VerticesVBO > 0)glDeleteBuffers(1,&VerticesVBO);
		if(NormalVBO > 0)glDeleteBuffers(1,&NormalVBO);
		if(ColorVBO > 0)glDeleteBuffers(1,&ColorVBO);
		ColorVBO = 0;
		NormalVBO = 0;
		VerticesVBO = 0;
		VAO = 0;
		
		SurfaceCount = 0;
		
		RemoveBoundingBox();
		loaded = false;
	}
	return *this;
}

void Object3D::CreateBoundingBox(Upp::Vector<float>& surface){
	float minX,minY,minZ;
	minX = minY = minZ = FLT_MAX;
	float maxX,maxY,maxZ;
	maxX = maxY = maxZ = -FLT_MAX;
	for (int i = 0; i < (surface.GetCount() -1); i = i + 3) {
		maxX = max(maxX, surface[i]);
		minX = min(minX, surface[i]);
		maxY = max(maxY, surface[i+1]);
		minY = min(minY, surface[i+1]);
		maxZ = max(maxZ, surface[i+2]);
		minZ = min(minZ, surface[i+2]);
	}
	boundingBox.SetBoundingBox(minX,minY,minZ,maxX,maxY,maxZ);
}

void Object3D::RemoveBoundingBox(){
	boundingBox = BoundingBox();
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
void Object3D::Draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix,glm::vec3 viewPosition)noexcept{
	if(SurfaceCount){
		glBindVertexArray(VAO);
		if(showMesh){
			if(NoLight.IsLinked() && Light.IsLinked()){
				OpenGLProgram&  prog = (showLight)? Light : NoLight;
				prog.Bind();
				if(showLight){
					prog.SetVec3("viewPos",viewPosition.x,viewPosition.y,viewPosition.z);
					if(material.ShouldBeUpdated()){
						prog.SetVec3("mat.Diffuse", material.GetDiffuse().x,material.GetDiffuse().y,material.GetDiffuse().z);
						prog.SetVec3("mat.Specular", material.GetSpecular().x,material.GetSpecular().y,material.GetSpecular().z);
						prog.SetFloat("mat.Shininess", material.GetShininess());
						material.HaveBeenUpdated();
					}
				}
				
				prog.SetMat4("ViewMatrix", viewMatrix);
				prog.SetMat4("ProjectionMatrix", projectionMatrix);
				prog.SetMat4("ModelMatrix", transform.GetModelMatrix());
				glDrawArrays(((prog.ContainTCS()) ? GL_PATCHES : DrawType), 0, SurfaceCount);
			}else{
				ONCELOCK{
					LOG("no Light/NoLight OpenGL Program have been provided, Object3D No " + AsString(ID) +" Can't be light/nolight draw");
				}
			}
		}
		if(showMeshLine){
			if(Line.IsLinked()){
				Line.Bind();
				glLineWidth(lineWidth);
				Line.SetMat4("ViewMatrix",viewMatrix);
				Line.SetMat4("ProjectionMatrix",projectionMatrix);
				Line.SetMat4("ModelMatrix",transform.GetModelMatrix());
				Line.SetVec4("CustomColor", lineColor.GetR() / 255.0f, lineColor.GetG() / 255.0f, lineColor.GetB() / 255.0f, lineOpacity );
				glDrawArrays(((Line.ContainTCS()) ? GL_PATCHES : GL_TRIANGLES), 0, SurfaceCount);
			}else{
				ONCELOCK{
					LOG("no Line OpenGL Program have been provided, Object3D No " + AsString(ID) +" Can't be line draw");
				}
			}
		}
		if(showMeshNormal){
			if(Normal.IsLinked()){
				Normal.Bind();
				Normal.SetMat4("ViewMatrix", viewMatrix);
				Normal.SetMat4("ProjectionMatrix", projectionMatrix);
				Normal.SetMat4("ModelMatrix", transform.GetModelMatrix());
				Normal.SetVec4("CustomColor",normalColor.GetR() / 255.0f, normalColor.GetG() / 255.0f, normalColor.GetB() / 255.0f, normalOpacity);
				Normal.SetFloat("normal_length",normalLenght );
				glDrawArrays(GL_TRIANGLES, 0, SurfaceCount);
			}else{
				ONCELOCK{
					LOG("no Normal OpenGL Program have been provided, Object3D No " + AsString(ID) +" Can't be normal draw");
				}
			}
		}
		if(showBoundingBox){
			if(Line.IsLinked()){
				boundingBox.Draw(transform.GetModelMatrix(),viewMatrix,projectionMatrix,Line);
			}else{
				ONCELOCK{
					LOG("no Line OpenGL Program have been provided, Object3D No " + AsString(ID) +" Can't have is bounding box draw");
				}
			}
		}
	}
}
}