#ifndef _SurfaceCtrl_STLLoader_h_
#define _SurfaceCtrl_STLLoader_h_
#include <Core/Core.h>
namespace Upp{

#define LLOG(x) LOG(x)

//Code taken from Surface package
//Author : Iñaki Koldo Zabala

class STLLoader{
	private:
		
	static int IsTabSpace(int c)noexcept{
		if (c == '\t' || c == ' ' || c == '!')
			return true;
		return false;
	}

	static bool LoadSTLText(const String& fileName, Vector<float>& vertices, Vector<float>& normals,bool ReadNormals = false){
		int lineCpt = 0;
		FileIn in(fileName);
		if (!in.IsOpen()){
			LLOG(Format(t_("Impossible to open file '%s'"), fileName));
			return false;
		}
		
		String line = ToLower(TrimBoth(in.GetLine()));
		lineCpt++;
		if (!line.StartsWith("solid")){
			LLOG(AsString(AsString(lineCpt)) + ". "  + t_("'solid' field not found"));
			return false;
		}
	    while (!in.IsEof()) {
			line = ToLower(TrimBoth(in.GetLine()));
			lineCpt++;
			if (line.IsEmpty())
				continue;
			
			if (line.StartsWith("endsolid"))
				break;
			
			if (!line.StartsWith("facet normal")){
				LLOG(AsString(AsString(lineCpt)) + ". "  + t_("'facet normal' field not found"));
				return false;
			}else{
				if(ReadNormals){
					Vector<String> data = Split(line,' ');
					normals.Add(Atof(data[2].ToStd().c_str()));
					normals.Add(Atof(data[3].ToStd().c_str()));
					normals.Add(Atof(data[4].ToStd().c_str()));
				}
			}
			
			line = ToLower(TrimBoth(in.GetLine()));
			lineCpt++;
			if (!line.StartsWith("outer loop")){
				LLOG(AsString(lineCpt) + ". "  + t_("'outer loop' field not found"));
				return false;
			}
	
			int ids[5];
			for (int i = 0; i < 6; ++i) {
				line = ToLower(TrimBoth(in.GetLine()));
				lineCpt++;
				Vector<String> data = Split(line,' ');
				//f.Load(in.GetLine());
				if(data.GetCount() == 0){
					LLOG(AsString(lineCpt) + ". " +t_("Empty line in outer loop"));
					return false;
				}
				String label = data[0];
				if (label == "vertex") {
					if (i == 4){
						LLOG(AsString(lineCpt) + ". "  + t_("Too much vertex in facet"));
						return false;
					}
					Vector<String> data = Split(line,' ');
					vertices.Add(Atof(data[1].ToStd().c_str()));
					vertices.Add(Atof(data[2].ToStd().c_str()));
					vertices.Add(Atof(data[3].ToStd().c_str()));
					if(!ReadNormals){
						if(i ==3){
							glm::vec3 p1(vertices[vertices.GetCount() - 9],vertices[vertices.GetCount() - 8],vertices[vertices.GetCount() - 7]);
							glm::vec3 p2(vertices[vertices.GetCount() - 6],vertices[vertices.GetCount() - 5],vertices[vertices.GetCount() - 4]);
							glm::vec3 p3(vertices[vertices.GetCount() - 3],vertices[vertices.GetCount() - 2],vertices[vertices.GetCount() - 1]);
							glm::vec3 normal = glm::abs(glm::normalize(glm::cross((p1 - p2),(p2 - p3)))) * -1.0f; //pro trick (in reality it's a bad trick...)
							normals << normal.x << normal.y << normal.z;
							normals << normal.x << normal.y << normal.z;
							normals << normal.x << normal.y << normal.z;
						}
					}
				}else if (label == "endloop") {
					if (i < 3){
						LLOG(AsString(lineCpt) + ". "  + t_("Too few vertex in facet"));
						return false;
					}
				}else if (label == "endfacet")
					break;
				else{
					LLOG(AsString(lineCpt) + ". "  + Format(t_("Label '%s' not handled in facet"), label));
					return false;
				}
			}
		}
		return true;
	}
	
	static bool LoadSTLBin(const String& fileName, Vector<float>& vertices, Vector<float>& normals,bool ReadNormals = false){
		FileIn in(fileName);
		if (!in.IsOpen()){
			LLOG(Format(t_("Impossible to open file '%s'"), fileName));
			return false;
		}
		
		String headerB = in.Get(80);
		if (headerB.StartsWith("solid")){
			LLOG(t_("Binary stl must not begin with 'solid' text"));
			return false;
		}
		int numFacets = in.Get32();
		while (!in.IsEof()) {
			if(ReadNormals){
				normals.Add((*(float*)in.Get(4).ToStd().c_str()));
				normals.Add((*(float*)in.Get(4).ToStd().c_str()));
				normals.Add((*(float*)in.Get(4).ToStd().c_str()));
			}else{
				in.Get(12);
			}
			
			vertices.Add((*(float*)in.Get(4).ToStd().c_str()));
			vertices.Add((*(float*)in.Get(4).ToStd().c_str()));
			vertices.Add((*(float*)in.Get(4).ToStd().c_str()));
		
			vertices.Add((*(float*)in.Get(4).ToStd().c_str()));
			vertices.Add((*(float*)in.Get(4).ToStd().c_str()));
			vertices.Add((*(float*)in.Get(4).ToStd().c_str()));
		
			vertices.Add((*(float*)in.Get(4).ToStd().c_str()));
			vertices.Add((*(float*)in.Get(4).ToStd().c_str()));
			vertices.Add((*(float*)in.Get(4).ToStd().c_str()));
			if(!ReadNormals){
				glm::vec3 p1(vertices[vertices.GetCount() - 9],vertices[vertices.GetCount() - 8],vertices[vertices.GetCount() - 7]);
				glm::vec3 p2(vertices[vertices.GetCount() - 6],vertices[vertices.GetCount() - 5],vertices[vertices.GetCount() - 4]);
				glm::vec3 p3(vertices[vertices.GetCount() - 3],vertices[vertices.GetCount() - 2],vertices[vertices.GetCount() - 1]);
				glm::vec3 normal = glm::abs(glm::normalize(glm::cross((p1 - p2),(p2 - p3)))) * -1.0f; //pro trick (in reality it's a bad trick...)
			
				normals << normal.x << normal.y << normal.z;
				normals << normal.x << normal.y << normal.z;
				normals << normal.x << normal.y << normal.z;
			}

			in.Get16();
		}
		return true;
	}
	
	public:
		
	static bool LoadSTL(const String& fileName, Vector<float>& vertices, Vector<float>& normals, bool ReadNormals = false){
		if(LoadSTLText(fileName,  vertices, normals ,ReadNormals)){
			return true;
		}else{
			return LoadSTLBin(fileName, vertices, normals ,ReadNormals);
		}
	}
	
	static bool SaveSTL(const String& fileName,const Vector<float>& vertices, const Vector<float>& normals){
		//TODO
		return false;
	}

};
}



#endif
