#include "Object3D.h"
#include <Surface/Surface.h>
namespace Upp{
int Object3D::GlobalID = 0;

Object3D& Object3D::operator=(Object3D&& obj){
	meshes = pick(obj.meshes);
	
	material = obj.material; //The material object is a representation of material property of the object (it change how light affect it)
	lineColor = obj.lineColor;
	lineOpacity = obj.lineOpacity;
	lineWidth = obj.lineWidth;
	normalColor = obj.normalColor;
	normalOpacity = obj.normalOpacity;
	normalLenght = obj.normalLenght;
	showMesh = obj.showMesh;
	showMeshLine = obj.showMeshLine;
	showMeshNormal = obj.showMeshNormal;
	showLight = obj.showLight;
	boundingBox = obj.boundingBox;
	showBoundingBox = obj.showBoundingBox;
	transform = obj.transform;
	DrawType = obj.DrawType;
	obj.moved = true;
	obj.loaded = false;
	return *this;
}

Object3D& Object3D::operator=(const Object3D& obj){
	meshes.Append(obj.meshes);
	
	material = obj.material; //The material object is a representation of material property of the object (it change how light affect it)
	lineColor = obj.lineColor;
	lineOpacity = obj.lineOpacity;
	lineWidth = obj.lineWidth;
	normalColor = obj.normalColor;
	normalOpacity = obj.normalOpacity;
	normalLenght = obj.normalLenght;
	showMesh = obj.showMesh;
	showMeshLine = obj.showMeshLine;
	showMeshNormal = obj.showMeshNormal;
	showLight = obj.showLight;
	boundingBox = obj.boundingBox;
	showBoundingBox = obj.showBoundingBox;
	transform = obj.transform;
	DrawType = obj.DrawType;
	
	return *this;
}

Object3D::~Object3D(){
	Unload();
}

bool Object3D::LoadObj(const String& FileObj){
	Color color = Gray();
	if(OBJLoader::LoadOBJ(FileObj,meshes)){
		float r = color.GetR()/255.0f;
		float g = color.GetG()/255.0f;
		float b = color.GetB()/255.0f;
		for(Mesh& m : meshes){
			int cpt = m.GetVertices().GetCount() -1;
			for(int e = 0; e < cpt ; e = e + 3){
				m.GetColors() << r << g << b ;
			}
		}
		Load();
		return true;
	}
	return false;
}

bool Object3D::LoadStl(const String& StlFile, Color color){
	Mesh& mesh = meshes.Create();
	glm::vec3 col( color.GetR()/255.0f, color.GetG()/255.0f, color.GetB()/255.0f);
	if(STLLoader::LoadSTL(StlFile,mesh.GetVertices(),mesh.GetNormals())){
		int cpt = mesh.GetVertices().GetCount() -1;
		for(int e = 0; e < cpt ; e = e + 3){
			mesh.GetColors() << col.x << col.y << col.z ;
		}
		Load();
		return true;
	}
	return false;
}
bool Object3D::LoadSurface(Surface& surface, Color color){
	Mesh& mesh = meshes.Create();
	glm::vec3 col( color.GetR()/255.0f, color.GetG()/255.0f, color.GetB()/255.0f);
	
	auto& vertices = mesh.GetVertices();
	auto& normals = mesh.GetNormals();
	auto& colors = mesh.GetColors();
	
	int indice =0;
	for (int ip = 0; ip < surface.panels.GetCount(); ++ip) {
		const Panel &panel = surface.panels[ip];
		//ADDING :
		if(panel.IsTriangle()){
			for(int e = 0; e < 3; e++){
				Point3D p = surface.nodes[panel.id[e]];
				vertices.Add(p.x);
				vertices.Add(p.y);
				vertices.Add(p.z);
				normals.Add(panel.normalPaint.x);
				normals.Add(panel.normalPaint.y);
				normals.Add(panel.normalPaint.z);
				colors.Add(col.x);
				colors.Add(col.y);
				colors.Add(col.z);
			}
		}else{//A quad is just 2 triangles
			Point3D SecondTriangle[3];
			for(int e = 0; e < 3; e++){
				Point3D p = surface.nodes[panel.id[e]];
				if(e == 0) SecondTriangle[2] = p;
				if(e == 2) SecondTriangle[0] = p;
				vertices.Add(p.x);
				vertices.Add(p.y);
				vertices.Add(p.z);
				
				normals.Add(panel.normalPaint.x);
				normals.Add(panel.normalPaint.y);
				normals.Add(panel.normalPaint.z);
				
				colors.Add(col.x);
				colors.Add(col.y);
				colors.Add(col.z);
			}
			SecondTriangle[1]  = surface.nodes[panel.id[3]];
			for(int e = 0; e < 3; e++){
				Point3D p = SecondTriangle[e];
				vertices.Add(p.x);
				vertices.Add(p.y);
				vertices.Add(p.z);
				
				normals.Add(panel.normalPaint.x);
				normals.Add(panel.normalPaint.y);
				normals.Add(panel.normalPaint.z);
				
				colors.Add(col.x);
				colors.Add(col.y);
				colors.Add(col.z);
			}
		}
	}
	Load();
	return true;
}

Surface Object3D::GetSurface(){ //return a surface Objectd
	Surface surf;
	for(Mesh& m : meshes){
		auto& vertices = m.GetVertices();
		auto& normals = m.GetNormals();
		for(int e = 0; e < vertices.GetCount(); e = e+9){
			Panel &panel = surf.panels.Add();
			
			panel.normalPaint.x = double(normals[e]);
			panel.normalPaint.y = double(normals[e+1]);
			panel.normalPaint.z = double(normals[e+2]);
			
			Point3D &node0 = surf.nodes.Add();
			node0.x = double(vertices[e]);
			node0.y = double(vertices[e+1]);
			node0.z = double(vertices[e+2]);
			panel.id[0] = surf.nodes.GetCount()-1;
			
			Point3D &node1 = surf.nodes.Add();
			node1.x = double(vertices[e+3]);
			node1.y = double(vertices[e+4]);
			node1.z = double(vertices[e+5]);
			panel.id[1] = surf.nodes.GetCount()-1;
			
			Point3D &node2 = surf.nodes.Add();
			node2.x = double(vertices[e+6]);
			node2.y = double(vertices[e+7]);
			node2.z = double(vertices[e+8]);
			panel.id[2] = surf.nodes.GetCount()-1;
			panel.id[3] = panel.id[0];
		}
	}
	return pick(surf);
}

bool Object3D::Load(){ //Load all data in graphic memory It's called automaticly by using Load function, but you must call it if you set manually all data
	for(Mesh& m : meshes){
		m.Init();
	}
	CreateBoundingBox();
	loaded = true;
	return true;
}
Object3D& Object3D::Unload(){
	if(!moved){
		for(Mesh& m : meshes){
			m.Clear(true);
		}
		RemoveBoundingBox();
		loaded = false;
	}
	return *this;
}

void Object3D::CreateBoundingBox(){
	float minX,minY,minZ;
	minX = minY = minZ = FLT_MAX;
	float maxX,maxY,maxZ;
	maxX = maxY = maxZ = -FLT_MAX;
	for(Mesh& m : meshes){
		auto& vertices = m.GetVertices();
		int cpt = vertices.GetCount() -1;
		for (int i = 0; i < cpt; i = i + 3) {
			maxX = max(maxX, vertices[i]);
			minX = min(minX, vertices[i]);
			maxY = max(maxY, vertices[i+1]);
			minY = min(minY, vertices[i+1]);
			maxZ = max(maxZ, vertices[i+2]);
			minZ = min(minZ, vertices[i+2]);
		}
	}
	boundingBox.SetBoundingBox(minX,minY,minZ,maxX,maxY,maxZ);
}

void Object3D::RemoveBoundingBox(){
	boundingBox = BoundingBox();
}

bool Object3D::UpdateBuffer(GLuint buffer, int SurfaceCount ,int SurfaceNumber, int count,const float * data)noexcept{
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
Vector<float> Object3D::ReadBuffer(GLuint buffer, int SurfaceCount , int SurfaceNumber, int count)noexcept{
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
bool Object3D::UpdateColor(unsigned int MeshNo, unsigned int SurfaceNumber, int r, int g, int b)noexcept{
	float data[]= {r/255.0f,g/255.0f,b/255.0f};
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetColorsVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber, 1 ,data);
	}else{
		return false;
	}
}
bool Object3D::UpdateColor(unsigned int MeshNo, unsigned int SurfaceNumber, float r, float g, float b)noexcept{
	float data[] = {r,g,b};
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetColorsVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber, 1 ,data);
	}else{
		return false;
	}
}
bool Object3D::UpdateColor(unsigned int MeshNo, unsigned int SurfaceNumber, glm::vec3 color)noexcept{
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetColorsVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber, 1 ,&(color.x));
	}else{
		return false;
	}
}
bool Object3D::UpdateColor(unsigned int MeshNo, unsigned int SurfaceNumber, Upp::Color color)noexcept{
	float data[] = {color.GetR()/255.0f,color.GetG()/255.0f,color.GetB()/255.0f};
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetColorsVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber, 1 ,data);
	}else{
		return false;
	}
}
bool Object3D::UpdateColors(unsigned int MeshNo, unsigned int SurfaceNumber,int Count, const float * data)noexcept{
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetColorsVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber, Count ,data);
	}else{
		return false;
	}
}
bool Object3D::UpdateNormal(unsigned int MeshNo, unsigned int SurfaceNumber, float x, float y, float z)noexcept{
	float data[] = {x,y,z};
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetNormalsVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber, 1 ,data);
	}else{
		return false;
	}
}
bool Object3D::UpdateNormal(unsigned int MeshNo, unsigned int SurfaceNumber, glm::vec3 normal)noexcept{
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetNormalsVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber, 1 ,&(normal.x));
	}else{
		return false;
	}
}
bool Object3D::UpdateNormals(unsigned int MeshNo, unsigned int SurfaceNumber,int Count, const float * data)noexcept{
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetNormalsVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber, Count ,data);
	}else{
		return false;
	}
}
bool Object3D::UpdateVertice(unsigned int MeshNo, unsigned int SurfaceNumber, float x, float y, float z)noexcept{
	float data[] = {x,y,z};
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetVerticesVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber, 1 ,data);
	}else{
		return false;
	}
}
bool Object3D::UpdateVertice(unsigned int MeshNo, unsigned int SurfaceNumber, glm::vec3 vertice)noexcept{
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetVerticesVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber, 1 ,&(vertice.x));
	}else{
		return false;
	}
}
bool Object3D::UpdateVertices(unsigned int MeshNo, unsigned int SurfaceNumber,int Count, const float * data)noexcept{
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetVerticesVBO(), meshes[MeshNo].GetVertices().GetCount()/3 ,SurfaceNumber, Count ,data);
	}else{
		return false;
	}
}
Vector<float> Object3D::ReadColors(int MeshNo, unsigned int SurfaceNumber, int count){
	if(MeshNo < meshes.GetCount()){
		return ReadBuffer(meshes[MeshNo].GetColorsVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber,count);
	}else{
		throw Exc(Format(t_("Object3D '%i' don't have meshes no '%i', colors can't be readed\n"), ID, MeshNo));
	}
}
Vector<float> Object3D::ReadNormals(int MeshNo, unsigned int SurfaceNumber, int count){
	if(MeshNo < meshes.GetCount()){
		return ReadBuffer(meshes[MeshNo].GetNormalsVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber,count);
	}else{
		throw Exc(Format(t_("Object3D '%i' don't have meshes no '%i', normals can't be readed\n"), ID, MeshNo));
	}
}
Vector<float> Object3D::ReadVertices(int MeshNo, unsigned int SurfaceNumber, int count){
	if(MeshNo < meshes.GetCount()){
		return ReadBuffer(meshes[MeshNo].GetVerticesVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber,count);
	}else{
		throw Exc(Format(t_("Object3D '%i' don't have meshes no '%i', vertices can't be readed\n"), ID, MeshNo));
	}
}
void Object3D::Draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix,glm::vec3 viewPosition)noexcept{
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
			
			
			for(Mesh& m : meshes){
				glBindVertexArray(m.GetVAO());
				glDrawArrays(((prog.ContainTCS()) ? GL_PATCHES : DrawType), 0, m.GetVertices().GetCount()/3);
			}
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
			
			for(Mesh& m : meshes){
				glBindVertexArray(m.GetVAO());
				glDrawArrays(((Line.ContainTCS()) ? GL_PATCHES : GL_TRIANGLES), 0, m.GetVertices().GetCount()/3);
			}
			//glDrawArrays(((Line.ContainTCS()) ? GL_PATCHES : GL_TRIANGLES), 0, SurfaceCount);
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
			for(Mesh& m : meshes){
				glBindVertexArray(m.GetVAO());
				glDrawArrays(GL_TRIANGLES, 0, m.GetVertices().GetCount()/3);
			}
			//glDrawArrays(GL_TRIANGLES, 0, SurfaceCount);
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