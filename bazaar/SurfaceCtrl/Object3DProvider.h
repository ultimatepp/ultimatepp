#ifndef _SurfaceCtrl_GLDrawFunction_h_
#define _SurfaceCtrl_GLDrawFunction_h_
#include "Definition.h"
#include "Object3D.h"

namespace Upp{
class Object3DProvider {
	private:
		bool begin = false;
		Upp::Vector<float> vertexData;
		Upp::Vector<float> normalData;
		Upp::Vector<float> colorData;
		GLenum DrawType;
		
		void InsertFloatColor(int number,const Color& color, int alpha){
			if(number > 0){
				for(int e = 0;e < number; e ++){
					colorData.Add(color.GetR()/255.0f);
					colorData.Add(color.GetG()/255.0f);
					colorData.Add(color.GetB()/255.0f);
					colorData.Add(alpha/255.0f);  //Not used YET
				}
			}
		}
		
		glm::vec3 Point3DToVec3(const Point3D& p){
			return glm::vec3(float(p.x), float(p.y), float(p.z));
		}
		
	public:
		Object3DProvider(){}
		Object3DProvider& Begin(GLenum drawType){
			if(!begin){
				vertexData.Clear();
				normalData.Clear();
				colorData.Clear();
				DrawType = drawType;
				begin = true;
			}else
				LOG("Error Object3DProvider : Call Begin function without ending the last begin function !");
			return *this;
		}
		Object3DProvider& AddLine(float x0, float y0, float z0, float x1, float y1, float z1,const Color& color, int alpha= 255){
			if(begin){
				vertexData<<x0<<y0<<z0<<x1<<y1<<z1;
				InsertFloatColor(2,color,alpha);
			}
			return *this;
		}
		Object3DProvider& AddLine(const glm::vec3& p0, const glm::vec3& p1, const Color &color, int alpha = 255){
			return AddLine(p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, color, alpha);
		}
		Object3DProvider& AddLine(const Segment3D& seg, const Color &color, int alpha = 255){
			return AddLine(float(seg.from.x), float(seg.from.y), float(seg.from.z), float(seg.to.x), float(seg.to.y), float(seg.to.z), color, alpha);
		}
		Object3DProvider& AddQuad(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const Color &color, int alpha= 255){
			if(begin){
				vertexData
				<<p0.x<<p0.y<<p0.z
				<<p1.x<<p1.y<<p1.z
				<<p2.x<<p2.y<<p2.z
				<<p3.x<<p3.y<<p3.z;
				InsertFloatColor(4,color,alpha);
			}
			return *this;
		}
		Object3DProvider& AddCuboid(const glm::vec3& p0, const glm::vec3& p1, const Color &color, int alpha= 255){
			if(begin){
				vertexData
				<<p0.x<< p0.y<< p0.z
				<<p0.x<< p0.y<< p1.z
				<<p0.x<< p1.y<< p1.z
				<<p0.x<< p1.y<< p0.z
				<<p0.x<< p0.y<< p0.z
				<<p1.x<< p0.y<< p0.z
				<<p1.x<< p0.y<< p1.z
				<<p1.x<< p1.y<< p1.z
				<<p1.x<< p1.y<< p0.z
				<<p1.x<< p0.y<< p0.z
				<<p1.x<< p0.y<< p1.z
				<<p0.x<< p0.y<< p1.z
				<<p0.x<< p1.y<< p1.z
				<<p1.x<< p1.y<< p1.z
				<<p1.x<< p1.y<< p0.z
				<<p0.x<< p1.y<< p0.z;
				InsertFloatColor(16,color,alpha);
			}
			return *this;
		}
		Object3DProvider& AddSurface(const Surface &surf, const Color &linCol, int alpha= 255){
			return AddSurface0(surf.nodes, surf.panels, linCol, alpha);
		}
		Object3DProvider& AddSegments(const Vector<Segment3D>& segs, const Color &color, int alpha= 255){
			for (int i = 0; i < segs.GetCount(); ++i)
				AddLine(segs[i], color, alpha);
			return *this;
		}
		Object3DProvider& AddArrow(float x0, float y0, float z0, float x1, float y1, float z1, const Color &color, int alpha= 255){
			if(begin){
				Segment3D seg(Point3D(x0, y0, z0), Point3D(x1, y1, z1));
				Vector3D vector = seg.Vector().Normalize();
				double len = seg.Length();
				double lenArr = 0.8*len;
				Point3D pointTri(x0, y0, z0);
				pointTri = pointTri + vector*lenArr;
				double zangle = atan2(vector.x, vector.y);
				double nangle = zangle + M_PI/2;
				Point3D parr1(pointTri.x + 0.1*len*sin(nangle), pointTri.y + 0.1*len*cos(nangle), pointTri.z);
				Point3D parr2(pointTri.x - 0.1*len*sin(nangle), pointTri.y - 0.1*len*cos(nangle), pointTri.z);
				
				AddLine(Point3DToVec3(seg.from), Point3DToVec3(pointTri), color, alpha);
				AddLine(Point3DToVec3(seg.to),   Point3DToVec3(parr1),	  color, alpha);
				AddLine(Point3DToVec3(seg.to),   Point3DToVec3(parr2),    color, alpha);
				AddLine(Point3DToVec3(parr1),    Point3DToVec3(parr2),    color, alpha);
			}
			return *this;
		}
		Object3DProvider& AddArrow(const glm::vec3 &p0, const glm::vec3 &p1, const Color &color, int alpha= 255){
			return AddArrow(p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, color);
		}
		Object3DProvider& AddAxis(float x, float y, float z, float len, int alpha= 255){
			AddArrow(x, y, z, x+len, y	 , z,     LtRed() , alpha);
			AddArrow(x, y, z, x	 , y+len , z,     LtGreen() , alpha);
			AddArrow(x, y, z, x	 , y	 , z+len, LtBlue() , alpha);
			return *this;
		}
		Object3DProvider& AddAxis(const glm::vec3 &p, float len , int alpha= 255){
			return AddAxis(p.x, p.y, p.z, len , alpha);
		}
		
		Object3DProvider&  CreateAxis(float len){
			if(begin){
				//vertexData is a vector of float
				//Len is the length of my axis (20 000)
				vertexData << 0.0f << 0.0f << 0.0f
						   << len  << 0.0f << 0.0f
						   << 0.0f << 0.0f << 0.0f
						   << 0.0f << len  << 0.0f
						   << 0.0f << 0.0f << 0.0f
						   << 0.0f << 0.0f << len;
			
			   InsertFloatColor(2,LtRed(), 255);
			   InsertFloatColor(2,LtGreen(), 255);
			   InsertFloatColor(2,LtBlue(), 255);
			}
			return *this;
		}
		
		Object3DProvider& AddDoubleAxis(float x, float y, float z, float len, const Color &color , int alpha= 255){
			AddLine(x-len/2, y	  , z	   , x+len/2, y	     , z	  , color, alpha);
			AddLine(x		 , y-len/2, z	   , x		, y+len/2, z	  , color, alpha);
			AddLine(x		 , y	  , z-len/2, x		, y	     , z+len/2, color, alpha);
			return *this;
		}
		Object3DProvider& AddDoubleAxis(const glm::vec3 &p, float len, const Color &color , int alpha= 255){
			return AddDoubleAxis(p.x, p.y, p.z, len, color, alpha);
		}
		Object3DProvider& AddCube(float x, float y, float z, float side, const Color &color , int alpha= 255){
			return AddCuboid(glm::vec3(x-side/2., y-side/2., z-side/2.), glm::vec3(x+side/2., y+side/2., z+side/2.), color, alpha);
		}
		Object3DProvider& AddCube(const Point3D &p, float side, const Color &color, int alpha= 255){
			return AddCube(float(p.x), float(p.y), float(p.z), side, color, alpha);
		}
		Object3DProvider& AddMesh(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const Color &linCol , int alpha= 255){
			AddLine(p0, p1, linCol, alpha);
			AddLine(p1, p2, linCol, alpha);
			AddLine(p2, p3, linCol, alpha);
			AddLine(p3, p0, linCol, alpha);
			return *this;
		}
		Object3DProvider& AddSurface0(const Vector<Point3D> &nodes, const Vector<Panel> &panels, const Color &linCol , int alpha= 255){
			for (int ip = 0; ip < panels.GetCount(); ++ip) {
				const Panel &panel = panels[ip];
				glm::vec3 p0 = Point3DToVec3(nodes[panel.id[0]]);
				glm::vec3 p1 = Point3DToVec3(nodes[panel.id[1]]);
				glm::vec3 p2 = Point3DToVec3(nodes[panel.id[2]]);
				glm::vec3 p3 = Point3DToVec3(nodes[panel.id[3]]);
				AddMesh(p0, p1, p2, p3, linCol, alpha);
			}
			return *this;
		}
		Object3D End(){
			normalData.Append(Vector<float>(vertexData.GetCount()));
			Object3D buffered;
			Mesh& m = buffered.CreateMesh();
			
			m.GetVertices().Append(vertexData);
			m.GetNormals().Append(normalData);
			m.GetColors().Append(colorData);
			
			buffered.Init();
			buffered.SetDrawType(DrawType);
			buffered.ShowLight(false);
			buffered.ShowMeshLine(false);
			buffered.ShowMeshNormal(false);
			
			begin = false;
			return pick(buffered);
		}
};
}
#endif