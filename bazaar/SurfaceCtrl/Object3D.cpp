#include "Object3D.h"
#include <Surface/Surface.h>
#define STRINGIFY(...) #__VA_ARGS__
#define SHADER(version, shader) "#version " #version "\n" STRINGIFY(shader)

namespace Upp{
	
#define IMAGECLASS TexturesImg
#define IMAGEFILE <SurfaceCtrl/textures.iml>
#include <Draw/iml.h>


int Object3D::GlobalID = 0;

Object3D::Object3D():ID(++GlobalID){
	lineColor = Black();
	lineOpacity = 0.5f;
	lineWidth = 1.0f;
	
	normalColor = Red();
	normalOpacity = 1.0f;
	normalLength = 1.0f;
	
	showMesh = true;
	showMeshLine = false;
	showMeshNormal = false;
	showLight = true;
	
	WhenInit = [&](Object3D& obj){ DefaultInit(obj); };
	WhenDraw = [&](const glm::mat4& projectionMatrix,const glm::mat4& viewMatrix,const glm::vec3& viewPosition, Object3D& obj){ DefaultDraw(projectionMatrix,viewMatrix,viewPosition,obj); };
	WhenClear = [&](Object3D& obj){ DefaultClear(obj); };
	WhenReload = [&](Object3D& obj){ DefaultReload(obj); };
}

Object3D& Object3D::operator=(Object3D&& obj){
	transform = pick(obj.transform);
	boundingBox = pick(obj.boundingBox);
	material = pick(obj.material);

	meshes = pick(obj.meshes);
	textures = pick(obj.textures);
	program = pick(obj.program);
	objectValues = pick(obj.objectValues);
	
	lineColor = obj.lineColor;
	lineOpacity = obj.lineOpacity;
	lineWidth = obj.lineWidth;
	
	normalColor = obj.normalColor;
	normalOpacity = obj.normalOpacity;
	normalLength = obj.normalLength;
	
	showMesh = obj.showMesh;
	showMeshLine = obj.showMeshLine;
	showMeshNormal = obj.showMeshNormal;
	showLight = obj.showLight;
	
	WhenInit = obj.WhenInit;
	WhenDraw = obj.WhenDraw;
	WhenClear = obj.WhenClear;

	loaded = obj.loaded;
	moved = obj.moved;
	visible = obj.visible;
	showBoundingBox = obj.showBoundingBox;

	programNoLight = obj.programNoLight;
	programLine = obj.programLine;
	programNormal = obj.programNormal;
	programLight = obj.programLight;
	
	drawType = obj.drawType;
	
	obj.moved = true;
	obj.loaded = false;
	return *this;
}

Object3D& Object3D::operator=(const Object3D& obj){
	transform = clone(obj.transform);
	boundingBox = clone(obj.boundingBox);
	material = clone(obj.material);

	meshes = clone(obj.meshes);
	textures = clone(obj.textures);
	program = clone(obj.program);
	objectValues = clone(obj.objectValues);
	
	lineColor = obj.lineColor;
	lineOpacity = obj.lineOpacity;
	lineWidth = obj.lineWidth;
	
	normalColor = obj.normalColor;
	normalOpacity = obj.normalOpacity;
	normalLength = obj.normalLength;
	
	showMesh = obj.showMesh;
	showMeshLine = obj.showMeshLine;
	showMeshNormal = obj.showMeshNormal;
	showLight = obj.showLight;
	
	WhenInit = obj.WhenInit;
	WhenDraw = obj.WhenDraw;
	WhenClear = obj.WhenClear;

	loaded = obj.loaded;
	moved = obj.moved;
	visible = obj.visible;
	showBoundingBox = obj.showBoundingBox;

	programNoLight = obj.programNoLight;
	programLine = obj.programLine;
	programNormal = obj.programNormal;
	programLight = obj.programLight;
	
	drawType = obj.drawType;
	return *this;
}

Object3D::~Object3D(){
	for(const Texture& t  : textures){
		glDeleteTextures(1,&(t.id));
	}
	Clear();
}
/**
	LoadModel load multiple kind of object file using Assimp lib, you can specify some custom
	load routine by editing pFlags, if pFlags = 0 then SurfaceCtrl default behavior about
	assimp will be used
**/
Object3D& Object3D::LoadModel(const String& Filename, Color color, int alpha , unsigned int pFlags ){
	Clear();
    if( pFlags == 0){
		pFlags = aiProcess_JoinIdenticalVertices |// join identical vertices/ optimize indexing
		aiProcess_ValidateDataStructure |	// perform a full validation of the loader's output
		aiProcess_ImproveCacheLocality |	// improve the cache locality of the output vertices
		aiProcess_RemoveRedundantMaterials |// remove redundant materials
		aiProcess_GenUVCoords |				// convert spherical, cylindrical, box and planar mapping to proper UVs
		aiProcess_TransformUVCoords |		// pre-process UV transformations (scaling, translation ...)
		aiProcess_FindInstances |			// search for instanced meshes and remove them by references to one master
		aiProcess_LimitBoneWeights |		// limit bone weights to 4 per vertex
		aiProcess_OptimizeMeshes |			// join small meshes, if possible;
		aiProcess_PreTransformVertices |
		aiProcess_GenSmoothNormals |		// generate smooth normal vectors if not existing
		aiProcess_SplitLargeMeshes |		// split large, unrenderable meshes into sub-meshes
		aiProcess_Triangulate |				// triangulate polygons with more than 3 edges
		aiProcess_ConvertToLeftHanded |		// convert everything to D3D left handed space
		aiProcess_SortByPType;
    }
    
    Assimp::Importer Importer;
	const aiScene* pScene = Importer.ReadFile(Filename.ToStd().c_str(),pFlags);
	if(pScene){
        if(InitFromScene(pScene, Filename)){
            glm::vec4 col(color.GetR()/255.0f, color.GetG()/255.0f, color.GetB()/255.0f, alpha/255.0f);
            for(Mesh& m : meshes){
				Vector<float>& colors = m.GetColors();
				for(int e = 0; e < (m.GetVertices().GetCount()/3) ; e++){
					colors << col.x << col.y << col.z << col.w;
				}
            }
        }else{
			throw Exc(Format("Error initing data from '%s'\n", Filename));
        }
    }else {
        throw Exc(Format("Error parsing '%s': '%s'\n", Filename, String(Importer.GetErrorString())));
    }
    return *this;
}
int Object3D::LoadTexture(const Image& img , const String& name, int indiceWhereInsert){
	Image image = clone(img);
	String trueName = name;
	if(IsNull(image)){
		image = clone(TexturesImg::empty());
		trueName = AsString(image.GetSerialId());
	}
	for(int e = 0; e < textures.GetCount(); e++){
		if(textures[e].name.IsEqual(trueName)){
			return e;
		}
	}
	
	Size size = image.GetSize();
	int indice;

	Texture& t = [&]()-> Texture&{
		if(indiceWhereInsert == -1){
			indice = textures.GetCount();
			return textures.Add();
		}else{
			if(indiceWhereInsert < textures.GetCount()){
				indice = indiceWhereInsert;
				return textures[indiceWhereInsert];
			}else{
				indice = textures.GetCount();
				return textures.Add();
			}
		}
	}();
	
	t.name = trueName;
	glGenTextures(1, &(t.id));
	glBindTexture(GL_TEXTURE_2D, t.id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.cx , size.cy , 0, GL_BGRA, GL_UNSIGNED_BYTE, ~image);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.cy , size.cx, 0, GL_RGBA, GL_UNSIGNED_BYTE, ~image);
	glGenerateMipmap(GL_TEXTURE_2D);

	return indice;
}
/*
	Assimp loading function
*/
bool Object3D::InitFromScene(const aiScene* pScene, const String& Filename){
	meshes.AddN(pScene->mNumMeshes);
	textures.AddN(pScene->mNumMaterials);
	//Add Texture vector init here
	
	// Initialises the scene meshes, one by one
    for (int i = 0 ; i < meshes.GetCount() ; i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        InitMesh(i, paiMesh);
    }
	return InitMaterials(pScene, Filename);
}
void Object3D::InitMesh(unsigned int Index, const aiMesh* paiMesh){
	//For texture / material data
	meshes[Index].SetTextureIndice(paiMesh->mMaterialIndex);
	
	Vector<float>& vertices = meshes[Index].GetVertices();
	Vector<float>& normals = meshes[Index].GetNormals();
	Vector<float>& textCoords = meshes[Index].GetTexCoords();
	
	struct Vertex : public Moveable<Vertex>{
		float pos[3];
		float normals[3];
		float texCoords[2];
		
		Vertex(float x, float y, float z, float n1 , float n2, float n3, float tc1 , float tc2){
			pos[0] = x; pos[1] = y; pos[2] = z;
			normals[0] = n1; normals[1] = n2; normals[2] = n3;
			texCoords[0] = tc1; texCoords[1] = tc2;
		}
	};
	
	Vector<Vertex> dummyVertices;
	Vector<unsigned int> dummyIndices;
	
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
        const aiVector3D* pPos = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal = (&(paiMesh->mNormals[i]))? &(paiMesh->mNormals[i]) : &Zero3D;
        
        glm::vec3 norm(pNormal->x , pNormal->y , pNormal->z);
        norm = glm::abs(norm) * -1.0f;
        
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
		
		//dummyVertices << Vertex( pPos->x , pPos->y , pPos->z , pNormal->x , pNormal->y , pNormal->z , pTexCoord->x , pTexCoord->y);
		dummyVertices << Vertex( pPos->x , pPos->y , pPos->z , norm.x , norm.y , norm.z , pTexCoord->x , pTexCoord->y);
    }
	
	for (int i = 0 ; i < paiMesh->mNumFaces ; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        ASSERT_(Face.mNumIndices == 3, "Face in Assimp strucure do not contain 3 points !");
        dummyIndices << Face.mIndices[0] << Face.mIndices[1] << Face.mIndices[2];
    }
	for(unsigned int i = 0 ; i < dummyIndices.GetCount(); i++){
		Vertex& vertex = dummyVertices[dummyIndices[i]];
		vertices <<  vertex.pos[0] << vertex.pos[1] << vertex.pos[2];
		normals << vertex.normals[0] << vertex.normals[1] << vertex.normals[2];
		textCoords << vertex.texCoords[0] << vertex.texCoords[1];
	}
}
bool Object3D::InitMaterials(const aiScene* pScene, const String& Filename){
	bool Ret = false;
	for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
        const aiMaterial* pMaterial = pScene->mMaterials[i];
        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0){
            aiString Path;
            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                String FullPath =AppendFileName(GetFileFolder(Filename), String(Path.data));
                InsertTexture(FullPath,i);
            }
        }
    }
    return true;
}

Object3D& Object3D::LoadSurface(Surface& surface, Color color, int alpha){
	Mesh& mesh = meshes.Create();
	glm::vec4 col( color.GetR()/255.0f, color.GetG()/255.0f, color.GetB()/255.0f , alpha /255.0f);
	
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
				vertices.Add(float(p.x));
				vertices.Add(float(p.y));
				vertices.Add(float(p.z));
				normals.Add(float(panel.normalPaint.x));
				normals.Add(float(panel.normalPaint.y));
				normals.Add(float(panel.normalPaint.z));
				colors.Add(col.x);
				colors.Add(col.y);
				colors.Add(col.z);
				colors.Add(col.w);
			}
		}else{//A quad is just 2 triangles
			Point3D SecondTriangle[3];
			for(int e = 0; e < 3; e++){
				Point3D p = surface.nodes[panel.id[e]];
				if(e == 0) SecondTriangle[2] = p;
				if(e == 2) SecondTriangle[0] = p;
				vertices.Add(float(p.x));
				vertices.Add(float(p.y));
				vertices.Add(float(p.z));
				
				normals.Add(float(panel.normalPaint.x));
				normals.Add(float(panel.normalPaint.y));
				normals.Add(float(panel.normalPaint.z));
				
				colors.Add(col.x);
				colors.Add(col.y);
				colors.Add(col.z);
				colors.Add(col.w);
			}
			SecondTriangle[1]  = surface.nodes[panel.id[3]];
			for(int e = 0; e < 3; e++){
				Point3D p = SecondTriangle[e];
				vertices.Add(float(p.x));
				vertices.Add(float(p.y));
				vertices.Add(float(p.z));
				
				normals.Add(float(panel.normalPaint.x));
				normals.Add(float(panel.normalPaint.y));
				normals.Add(float(panel.normalPaint.z));
				
				colors.Add(col.x);
				colors.Add(col.y);
				colors.Add(col.z);
				colors.Add(col.w);
			}
		}
	}
	return *this;
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

//Check if texture provided is already load, then return the iterator of the texture
//object
int Object3D::InsertTexture(const String& filename, int indice, FlipMode flipmode)noexcept{ //insert texture in object3D
	Image m = pick(StreamRaster::LoadFileAny( (IsFullPath(filename))? filename : AppendFileName(GetCurrentDirectory(),filename)));
	return InsertTexture(m,indice,flipmode);
}
int Object3D::InsertTexture(const Image& m, int indice, FlipMode flipmode)noexcept{ //insert texture in object3D
	return LoadTexture(FlipImage(m,flipmode),AsString(m.GetSerialId()),indice);
}

int Object3D::InsertTexture(const TexturesMaterial& tm, int indice, FlipMode flipmode)noexcept{ //Insert one of SurfaceCtrl provided texture
	Image m;
	switch(tm){
		case TM_BRICK:
			m = clone(TexturesImg::brick());
		break;
		case TM_METAL:
			m = clone(TexturesImg::metal());
		break;
		case TM_STONE:
			m = clone(TexturesImg::stone());
		break;
		case TM_WATER:
			m = clone(TexturesImg::water());
		break;
		case TM_WOOD:
			m = clone(TexturesImg::wood());
		break;
	}
	return InsertTexture(m,indice,flipmode);
}
Object3D& Object3D::AttachTexture(int numTexture,int MeshNo, int count){//Attach a texture to the range of mes
	int textNo = numTexture;
	if(textNo > textures.GetCount()) textNo = 0;
	if(MeshNo < meshes.GetCount() && count > 0 &&  (MeshNo + count) <= meshes.GetCount()){
		for(int e = MeshNo; e < (MeshNo + count); e ++){
			meshes[e].SetTextureIndice(textNo);
		}
	}
	return *this;
}
Object3D& Object3D::GenerateTextureCoordinate(int MeshNo, int count , bool CustomTextureCoordinate, const Vector<float>& tc){// Generate new Texture coordinate for the selected range of mesh
	if(MeshNo < meshes.GetCount() && count > 0 && (MeshNo + count) <= meshes.GetCount()){
		for(int e = MeshNo; e < (MeshNo + count); e++){
			Mesh& m  = meshes[e];
			Vector<float>& tex = m.GetTexCoords();
			tex.Clear();
			int triangleCpt = (m.GetVertices().GetCount()/3) -1;
			for(int i = 0; i < triangleCpt; i++){
				if(CustomTextureCoordinate) tex.Append(tc);
				else tex << 0.0f << 0.0f << 1.0f << 0.0f << 0.5f << 1.0f;
			}
		}
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

bool Object3D::UpdateBuffer(GLuint buffer, int SurfaceCount ,int SurfaceNumber, int count, int numberOfElement, const float * data)noexcept{
	glBindBuffer(GL_ARRAY_BUFFER,buffer);
	float* ptr = (float*) glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
	//float* ptr = (float*) glMapNamedBuffer(buffer,GL_WRITE_ONLY);
	if(ptr){
		if(SurfaceNumber < SurfaceCount){
			ptr += SurfaceNumber;
			for(int i = 0; i < count; i++){
				for(int e = 0; e < numberOfElement; e++){
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
Vector<float> Object3D::ReadBuffer(GLuint buffer, int SurfaceCount , int SurfaceNumber, int count, int numberOfElement)noexcept{
//	float* ptr = (float*) glMapNamedBuffer(buffer,GL_READ_ONLY);
	glBindBuffer(GL_ARRAY_BUFFER,buffer);
	float* ptr = (float*) glMapBuffer(GL_ARRAY_BUFFER,GL_READ_ONLY);
	Vector<float> data;
	if(ptr){
		if(SurfaceNumber < SurfaceCount){
			ptr += SurfaceNumber;
			for(int i = 0; i < count; i++){
				for(int e = 0; e < numberOfElement; e++){
					data.Add(*ptr);
				}
			}
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	return data;
}
bool Object3D::UpdateColor(int MeshNo, int SurfaceNumber, int r, int g, int b, int alpha)noexcept{
	float data[]= {r/255.0f,g/255.0f,b/255.0f, alpha/255.0f};
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetColorsVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber, 1 ,4,data);
	}else{
		return false;
	}
}
bool Object3D::UpdateColor(int MeshNo, int SurfaceNumber, float r, float g, float b, float alpha)noexcept{
	float data[] = {r,g,b, alpha};
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetColorsVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber, 1 ,4,data);
	}else{
		return false;
	}
}
bool Object3D::UpdateColor(int MeshNo, int SurfaceNumber, glm::vec3 color, float alpha)noexcept{
	if(MeshNo < meshes.GetCount()){
		float data[]= {color.x,color.y,color.z, alpha};
		return UpdateBuffer(meshes[MeshNo].GetColorsVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber, 1 ,4,data);
	}else{
		return false;
	}
}
bool Object3D::UpdateColor(int MeshNo, int SurfaceNumber, Upp::Color color, int alpha)noexcept{
	float data[] = {color.GetR()/255.0f,color.GetG()/255.0f,color.GetB()/255.0f, alpha/255.0f};
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetColorsVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber, 1 ,4,data);
	}else{
		return false;
	}
}
bool Object3D::UpdateColors(int MeshNo, int SurfaceNumber,int Count, const float * data)noexcept{
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetColorsVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber, Count ,4,data);
	}else{
		return false;
	}
}
bool Object3D::UpdateNormal(int MeshNo, int SurfaceNumber, float x, float y, float z)noexcept{
	float data[] = {x,y,z};
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetNormalsVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber, 1 ,3, data);
	}else{
		return false;
	}
}
bool Object3D::UpdateNormal(int MeshNo, int SurfaceNumber, glm::vec3 normal)noexcept{
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetNormalsVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber, 1 ,3, &(normal.x));
	}else{
		return false;
	}
}
bool Object3D::UpdateNormals(int MeshNo, int SurfaceNumber,int Count, const float * data)noexcept{
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetNormalsVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber, Count ,3, data);
	}else{
		return false;
	}
}
bool Object3D::UpdateVertice(int MeshNo, int SurfaceNumber, float x, float y, float z)noexcept{
	float data[] = {x,y,z};
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetVerticesVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber, 1 ,3, data);
	}else{
		return false;
	}
}
bool Object3D::UpdateVertice(int MeshNo, int SurfaceNumber, glm::vec3 vertice)noexcept{
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetVerticesVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber, 1 ,3, &(vertice.x));
	}else{
		return false;
	}
}
bool Object3D::UpdateVertices(int MeshNo, int SurfaceNumber,int Count, const float * data)noexcept{
	if(MeshNo < meshes.GetCount()){
		return UpdateBuffer(meshes[MeshNo].GetVerticesVBO(), meshes[MeshNo].GetVertices().GetCount()/3 ,SurfaceNumber, Count ,3, data);
	}else{
		return false;
	}
}
Vector<float> Object3D::ReadColors(int MeshNo, int SurfaceNumber, int count){
	if(MeshNo < meshes.GetCount()){
		return ReadBuffer(meshes[MeshNo].GetColorsVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber,count,4);
	}else{
		throw Exc(Format(t_("Object3D '%i' don't have meshes no '%i', colors can't be readed\n"), ID, MeshNo));
	}
}
Vector<float> Object3D::ReadNormals(int MeshNo, int SurfaceNumber, int count){
	if(MeshNo < meshes.GetCount()){
		return ReadBuffer(meshes[MeshNo].GetNormalsVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber,count,3);
	}else{
		throw Exc(Format(t_("Object3D '%i' don't have meshes no '%i', normals can't be readed\n"), ID, MeshNo));
	}
}
Vector<float> Object3D::ReadVertices(int MeshNo, int SurfaceNumber, int count){
	if(MeshNo < meshes.GetCount()){
		return ReadBuffer(meshes[MeshNo].GetVerticesVBO(), meshes[MeshNo].GetVertices().GetCount()/3,SurfaceNumber,count,3);
	}else{
		throw Exc(Format(t_("Object3D '%i' don't have meshes no '%i', vertices can't be readed\n"), ID, MeshNo));
	}
}

void Object3D::DefaultInit(Object3D& obj){
	for(Mesh& m : obj.GetMeshes()){
		m.Init();
	}
	obj.CreateBoundingBox();
	
}
void Object3D::DefaultDraw(const glm::mat4& projectionMatrix,const glm::mat4& viewMatrix,const glm::vec3& viewPosition,Object3D& obj)noexcept{
	if(obj.IsLoaded()){
		if(obj.IsVisible()){
			if(obj.GetShowMesh()){
				if(obj.GetProgramNoLight() != -1 && obj.GetProgramLight() != -1 && obj.GetProgram()[obj.GetProgramNoLight()].IsLinked() && obj.GetProgram()[obj.GetProgramLight()].IsLinked()){
					OpenGLProgram&  prog = (obj.GetShowLight())? obj.GetProgram()[obj.GetProgramLight()] : obj.GetProgram()[obj.GetProgramNoLight()];
					prog.Bind();
					if(obj.GetShowLight()){
						prog.SetVec3("viewPos",viewPosition.x,viewPosition.y,viewPosition.z);
						if(obj.GetMaterial().ShouldBeUpdated()){
							Material& material = obj.GetMaterial();
							prog.SetVec3("mat.Diffuse", material.GetDiffuse().x,material.GetDiffuse().y,material.GetDiffuse().z);
							prog.SetVec3("mat.Specular", material.GetSpecular().x,material.GetSpecular().y,material.GetSpecular().z);
							prog.SetFloat("mat.Shininess", material.GetShininess());
							material.HaveBeenUpdated();
						}
					}
					prog.SetMat4("ViewMatrix", viewMatrix);
					prog.SetMat4("ProjectionMatrix", projectionMatrix);
					prog.SetMat4("ModelMatrix", obj.GetTransform().GetModelMatrix());
					
					for(Mesh& m : obj.GetMeshes()){
						glBindVertexArray(m.GetVAO());
						if(obj.GetTextures().GetCount()> 0){
							glActiveTexture(GL_TEXTURE0);
							glBindTexture(GL_TEXTURE_2D, obj.GetTextures()[m.GetTextureIndice()].id);
							prog.SetInt("tex", 0);
							prog.SetInt("useTexture", obj.GetTextures()[m.GetTextureIndice()].id);
						}else{
							prog.SetInt("useTexture", 0);
						}
						glDrawArrays(((prog.ContainTCS()) ? GL_PATCHES : obj.GetDrawType()), 0, m.GetVertices().GetCount()/3);
					}
				}else{
					ONCELOCK{
						LOG("no Light/NoLight OpenGL Program have been provided, Object3D No " + AsString(ID) +" Can't be light/nolight draw");
					}
				}
			}
			if(obj.GetShowMeshLine()){
				if(obj.GetProgramLine() != -1 && obj.GetProgram()[obj.GetProgramLine()].IsLinked()){
					OpenGLProgram& prog =  obj.GetProgram()[obj.GetProgramLine()];
					prog.Bind();
					
					glLineWidth(obj.GetLineWidth());
					prog.SetMat4("ViewMatrix",viewMatrix);
					prog.SetMat4("ProjectionMatrix",projectionMatrix);
					prog.SetMat4("ModelMatrix",obj.GetTransform().GetModelMatrix());
					Color lineColor = obj.GetLineColor();
					prog.SetVec4("CustomColor", lineColor.GetR() / 255.0f, lineColor.GetG() / 255.0f, lineColor.GetB() / 255.0f,obj.GetLineOpacity());
					
					for(Mesh& m : obj.GetMeshes()){
						glBindVertexArray(m.GetVAO());
						glDrawArrays(((prog.ContainTCS()) ? GL_PATCHES : GL_TRIANGLES), 0, m.GetVertices().GetCount()/3);
					}
				}else{
					ONCELOCK{
						LOG("no Line OpenGL Program have been provided, Object3D No " + AsString(ID) +" Can't be line draw");
					}
				}
			}
			if(obj.GetShowMeshNormal()){
				if(obj.GetProgramNormal() != -1 && obj.GetProgram()[obj.GetProgramNormal()].IsLinked() ){
					OpenGLProgram& prog =  obj.GetProgram()[obj.GetProgramNormal()];
					prog.Bind();
					prog.SetMat4("ViewMatrix", viewMatrix);
					prog.SetMat4("ProjectionMatrix", projectionMatrix);
					prog.SetMat4("ModelMatrix", obj.GetTransform().GetModelMatrix());
					Color normalColor = obj.GetNormalColor();
					prog.SetVec4("CustomColor",normalColor.GetR() / 255.0f, normalColor.GetG() / 255.0f, normalColor.GetB() / 255.0f, obj.GetNormalOpacity());
					prog.SetFloat("normal_length",obj.GetNormalLength());
					for(Mesh& m : obj.GetMeshes()){
						glBindVertexArray(m.GetVAO());
						glDrawArrays(GL_TRIANGLES, 0, m.GetVertices().GetCount()/3);
					}
				}else{
					ONCELOCK{
						LOG("no Normal OpenGL Program have been provided, Object3D No " + AsString(ID) +" Can't be normal draw");
					}
				}
			}
			if(obj.GetShowBoundingBox()){
				if(obj.GetProgramLine() != -1 && obj.GetProgram()[obj.GetProgramLine()].IsLinked()){
					boundingBox.Draw(obj.GetTransform().GetModelMatrix(),viewMatrix,projectionMatrix,obj.GetProgram()[obj.GetProgramLine()]);
				}else{
					ONCELOCK{
						LOG("no Line OpenGL Program have been provided, Object3D No " + AsString(ID) +" Can't have is bounding box draw");
					}
				}
			}
		}
	}
}
void Object3D::DefaultClear(Object3D& obj){
	if(!moved){
		for(Mesh& m : obj.GetMeshes()){
			m.Clear(true);
		}
		obj.RemoveBoundingBox();
	}
}
void Object3D::DefaultReload(Object3D& obj){
	obj.Clear();
	obj.Init();
}

Skybox& Skybox::Init(const Image& skybox_right,const Image& skybox_left,const Image& skybox_top,const Image& skybox_bottom,const Image& skybox_front,const Image& skybox_back){
	auto TestImage = [&](const Image& img) -> Image{
		if(img){
			return pick(img);
		}
		return TexturesImg::empty();
	};
	return Init(Vector<Image>{ TestImage(skybox_right), TestImage(skybox_left), TestImage(skybox_top), TestImage(skybox_bottom),TestImage(skybox_front), TestImage(skybox_back)});
}
Skybox& Skybox::Init(const Vector<Image>& images){
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
	
	int i = 0;
	for(const Image& img : images){
		if(img){
			Size size = img.GetSize();
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, size.cx , size.cy, 0, GL_BGRA, GL_UNSIGNED_BYTE, ~img);
		}else{
			Size size = TexturesImg::empty().GetSize();
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, size.cx , size.cy, 0, GL_BGRA, GL_UNSIGNED_BYTE, ~(TexturesImg::empty()));
		}
		i++;
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    Vector<float> skyboxData;
    skyboxData << -1.0f <<  1.0f << -1.0f << -1.0f << -1.0f << -1.0f << 1.0f << -1.0f << -1.0f << 1.0f << -1.0f << -1.0f << 1.0f <<  1.0f << -1.0f << -1.0f <<  1.0f << -1.0f <<
	-1.0f << -1.0f <<  1.0f << -1.0f << -1.0f << -1.0f << -1.0f <<  1.0f << -1.0f << -1.0f <<  1.0f << -1.0f << -1.0f <<  1.0f <<  1.0f << -1.0f << -1.0f <<  1.0f <<
	 1.0f << -1.0f << -1.0f << 1.0f << -1.0f <<  1.0f << 1.0f <<  1.0f <<  1.0f << 1.0f <<  1.0f <<  1.0f << 1.0f <<  1.0f << -1.0f << 1.0f << -1.0f << -1.0f <<
	-1.0f << -1.0f <<  1.0f << -1.0f <<  1.0f <<  1.0f << 1.0f <<  1.0f <<  1.0f << 1.0f <<  1.0f <<  1.0f << 1.0f << -1.0f <<  1.0f << -1.0f << -1.0f <<  1.0f <<
	-1.0f <<  1.0f << -1.0f << 1.0f <<  1.0f << -1.0f << 1.0f <<  1.0f <<  1.0f << 1.0f <<  1.0f <<  1.0f << -1.0f <<  1.0f <<  1.0f << -1.0f <<  1.0f << -1.0f <<
	-1.0f << -1.0f << -1.0f << -1.0f << -1.0f <<  1.0f << 1.0f << -1.0f << -1.0f << 1.0f << -1.0f << -1.0f << -1.0f << -1.0f <<  1.0f << 1.0f << -1.0f <<  1.0f;
    

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, skyboxData.GetCount() * sizeof(float), &(skyboxData[0]), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
 
	program.AttachShader(OpenGLShader(GL_VERTEX_SHADER,
		#include "shaders/VertexCubeMap.glsl"
	)).AttachShader(OpenGLShader(GL_FRAGMENT_SHADER,
		#include "shaders/FragmentCubeMap.glsl"
	)).Link();
	return *this;
}
Skybox& Skybox::Clear(){if(ID)glDeleteTextures(1,&ID); return *this;} //Clear the skybox

Skybox& Skybox::Draw(const glm::mat4& projectionMatrix,const glm::mat4& viewMatrix){ //Draw the skybox
	if(show && ID > 0){
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		program.Bind();
		
		program.SetMat4("ViewMatrix", glm::mat4(glm::mat3(viewMatrix)));// remove translation from the view matrix
		program.SetMat4("ProjectionMatrix", projectionMatrix);
		program.SetInt("skybox", 1);
		
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS); // set depth function back to default
	}
	return *this;
}

}