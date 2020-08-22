#ifndef _SurfaceCtrl_GLDrawFunction_h_
#define _SurfaceCtrl_GLDrawFunction_h_
#include <Surface/Surface.h>
#include "Object3D.h"
/*
	All this code belong to Iñaki Zabala
*/
namespace Upp{
class Object3DProvider {
	private:
		bool begin = false;
		Upp::Vector<float> vertexData;
		Upp::Vector<float> normalData;
		Upp::Vector<float> colorData;
		GLenum DrawType;
		
		void InsertFloatColor(int number,const Color& color){
			if(number > 0){
				for(int e = 0;e < number; e ++){
					colorData.Add(color.GetR()/255.0f);
					colorData.Add(color.GetG()/255.0f);
					colorData.Add(color.GetB()/255.0f);
				}
			}
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
		Object3DProvider& AddLine(double x0, double y0, double z0, double x1, double y1, double z1,const Color& color){
			if(begin){
				vertexData
				<<x0<<y0<<z0
				<<x1<<y1<<z1;
				
				InsertFloatColor(2,color);
			}
			return *this;
		}
		Object3DProvider& AddLine(const Point3D &p0, const Point3D &p1, const Color &color){
			return AddLine(p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, color);
		}
		Object3DProvider& AddLine(const Segment3D &p, const Color &color){
			return AddLine(p.from, p.to, color);
		}
		Object3DProvider& AddQuad(const Point3D &p0, const Point3D &p1, const Point3D &p2, const Point3D &p3, const Color &color){
			if(begin){
				vertexData
				<<p0.x<<p0.y<<p0.z
				<<p1.x<<p1.y<<p1.z
				<<p2.x<<p2.y<<p2.z
				<<p3.x<<p3.y<<p3.z;
				InsertFloatColor(4,color);
			}
			return *this;
		}
		Object3DProvider& AddCuboid(const Point3D &p0, const Point3D &p1, const Color &color){
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
				
				InsertFloatColor(16,color);
			}
			return *this;
		}
		Object3DProvider& AddSurface(const Surface &surf, const Color &linCol){
			return AddSurface0(surf.nodes, surf.panels, linCol);
		}
		Object3DProvider& AddSegments(const Vector<Segment3D>& segs, const Color &color){
			for (int i = 0; i < segs.GetCount(); ++i)
				AddLine(segs[i], color);
			return *this;
		}
		Object3DProvider& AddArrow(double x0, double y0, double z0, double x1, double y1, double z1, const Color &color){
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
				
				AddLine(seg.from, pointTri, color);
				AddLine(seg.to,   parr1,	  color);
				AddLine(seg.to,   parr2,    color);
				AddLine(parr1,    parr2,    color);
			}
			return *this;
		}
		Object3DProvider& AddArrow(const Point3D &p0, const Point3D &p1, const Color &color){
			return AddArrow(p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, color);
		}
		Object3DProvider& AddAxis(double x, double y, double z, double len){
			AddArrow(x, y, z, x+len, y	 , z,     LtRed());
			AddArrow(x, y, z, x	 , y+len , z,     LtGreen());
			AddArrow(x, y, z, x	 , y	 , z+len, LtBlue());
			return *this;
		}
		Object3DProvider& AddAxis(const Point3D &p, double len){
			return AddAxis(p.x, p.y, p.z, len);
		}
		
		Object3DProvider&  CreateAxis(double len){
			if(begin){
				//vertexData is a vector of float
				//Len is the length of my axis (20 000)
				vertexData << 0.0f << 0.0f << 0.0f
						   << len  << 0.0f << 0.0f
						   << 0.0f << 0.0f << 0.0f
						   << 0.0f << len  << 0.0f
						   << 0.0f << 0.0f << 0.0f
						   << 0.0f << 0.0f << len;
			
			   InsertFloatColor(2,LtRed());
			   InsertFloatColor(2,LtGreen());
			   InsertFloatColor(2,LtBlue());
			}
			return *this;
		}
		
		Object3DProvider& AddDoubleAxis(double x, double y, double z, double len, const Color &color){
			AddLine(x-len/2, y	  , z	   , x+len/2, y	     , z	  , color);
			AddLine(x		 , y-len/2, z	   , x		, y+len/2, z	  , color);
			AddLine(x		 , y	  , z-len/2, x		, y	     , z+len/2, color);
			return *this;
		}
		Object3DProvider& AddDoubleAxis(const Point3D &p, double len, const Color &color){
			return AddDoubleAxis(p.x, p.y, p.z, len, color);
		}
		Object3DProvider& AddCube(double x, double y, double z, double side, const Color &color){
			return AddCuboid(Point3D(x-side/2., y-side/2., z-side/2.), Point3D(x+side/2., y+side/2., z+side/2.), color);
		}
		Object3DProvider& AddCube(const Point3D &p, double side, const Color &color){
			return AddCube(p.x, p.y, p.z, side, color);
		}
		Object3DProvider& AddMesh(const Point3D &p0, const Point3D &p1, const Point3D &p2, const Point3D &p3, const Color &linCol){
			AddLine(p0, p1, linCol);
			AddLine(p1, p2, linCol);
			AddLine(p2, p3, linCol);
			AddLine(p3, p0, linCol);
			return *this;
		}
		Object3DProvider& AddSurface0(const Vector<Point3D> &nodes, const Vector<Panel> &panels, const Color &linCol){
			for (int ip = 0; ip < panels.GetCount(); ++ip) {
				const Panel &panel = panels[ip];
				Point3D p0 = nodes[panel.id[0]];
				Point3D p1 = nodes[panel.id[1]];
				Point3D p2 = nodes[panel.id[2]];
				Point3D p3 = nodes[panel.id[3]];
				AddMesh(p0, p1, p2, p3, linCol);
			}
			return *this;
		}
		Object3D End(){
			normalData.Append(Vector<float>(vertexData.GetCount()));
			Object3D buffered;
			buffered.AddVerticesData(vertexData);
			buffered.AddNormalsData(normalData);
			buffered.AddColorsData(colorData);
			buffered.Load();
			
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