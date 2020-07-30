#ifndef _SurfaceCtrl_GLDrawFunction_h_
#define _SurfaceCtrl_GLDrawFunction_h_
#include <Surface/Surface.h>

/*
	All this code belong to Iñaki Zabala
*/

namespace Upp{
class GLDrawFunction {
	public:
		static void PaintLine(double x0, double y0, double z0, double x1, double y1, double z1, const Color &color){
			glBegin(GL_LINES);
			glColor4d(color.GetR()/255., color.GetG()/255., color.GetB()/255., 1);
			glVertex3d(x0, y0, z0);
			glVertex3d(x1, y1, z1);
			glEnd();
		}
		static void PaintLine(const Point3D &p0, const Point3D &p1, const Color &color){
			PaintLine(p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, color);
		}
		static void PaintLine(const Segment3D &p, const Color &color){
			PaintLine(p.from, p.to, color);
		}
		static void PaintQuad(const Point3D &p0, const Point3D &p1, const Point3D &p2, const Point3D &p3, const Color &color, double alpha = 1){
			glBegin(GL_QUADS);
				glColor4d(color.GetR()/255., color.GetG()/255., color.GetB()/255., alpha);
				glVertex3d(p0.x, p0.y, p0.z);
				glVertex3d(p1.x, p1.y, p1.z);
				glVertex3d(p2.x, p2.y, p2.z);
				glVertex3d(p3.x, p3.y, p3.z);
			glEnd();
		}
		static void PaintCuboid(const Point3D &p0, const Point3D &p1, const Color &color){
			glBegin(GL_LINE_STRIP);
				glColor4d(color.GetR()/255., color.GetG()/255., color.GetB()/255., 1);
				glLineWidth(2);
				glVertex3d(p0.x, p0.y, p0.z);
				glVertex3d(p0.x, p0.y, p1.z);
				glVertex3d(p0.x, p1.y, p1.z);
				glVertex3d(p0.x, p1.y, p0.z);
				glVertex3d(p0.x, p0.y, p0.z);
				glVertex3d(p1.x, p0.y, p0.z);
				glVertex3d(p1.x, p0.y, p1.z);
				glVertex3d(p1.x, p1.y, p1.z);
				glVertex3d(p1.x, p1.y, p0.z);
				glVertex3d(p1.x, p0.y, p0.z);
				glVertex3d(p1.x, p0.y, p1.z);
				glVertex3d(p0.x, p0.y, p1.z);
				glVertex3d(p0.x, p1.y, p1.z);
				glVertex3d(p1.x, p1.y, p1.z);
				glVertex3d(p1.x, p1.y, p0.z);
				glVertex3d(p0.x, p1.y, p0.z);
			glEnd();
		}
		static void PaintSurface(const Surface &surf, const Color &linCol, bool showMesh, bool showNormals){
			PaintSurface0(surf.nodes, surf.panels, showMesh, showNormals, surf.avgFacetSideLen, linCol);
		}
		static void PaintSegments(const Vector<Segment3D>& segs, const Color &color){
			for (int i = 0; i < segs.GetCount(); ++i)
				PaintLine(segs[i], color);
		}
		static void PaintAxis(double x, double y, double z, double len){
			PaintArrow(x, y, z, x+len, y	 , z,     LtRed());
			PaintArrow(x, y, z, x	 , y+len , z,     LtGreen());
			PaintArrow(x, y, z, x	 , y	 , z+len, LtBlue());
		}
		static void PaintAxis(const Point3D &p, double len){
			PaintAxis(p.x, p.y, p.z, len);
		}
		static void PaintDoubleAxis(double x, double y, double z, double len, const Color &color){
			PaintLine(x-len/2, y	  , z	   , x+len/2, y	     , z	  , color);
			PaintLine(x		 , y-len/2, z	   , x		, y+len/2, z	  , color);
			PaintLine(x		 , y	  , z-len/2, x		, y	     , z+len/2, color);
		}
		static void PaintDoubleAxis(const Point3D &p, double len, const Color &color){
			PaintDoubleAxis(p.x, p.y, p.z, len, color);
		}
		static void PaintCube(const Point3D &p, double side, const Color &color){
			PaintCube(p.x, p.y, p.z, side, color);
		}
		static void PaintCube(double x, double y, double z, double side, const Color &color){
			PaintCuboid(Point3D(x-side/2., y-side/2., z-side/2.), Point3D(x+side/2., y+side/2., z+side/2.), color);
		}
		static void PaintArrow(double x0, double y0, double z0, double x1, double y1, double z1, const Color &color){
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
			PaintLine(seg.from, pointTri, color);
			PaintLine(seg.to,   parr1,	  color);
			PaintLine(seg.to,   parr2,    color);
			PaintLine(parr1,    parr2,    color);
		}
		static void PaintArrow(const Point3D &p0, const Point3D &p1, const Color &color){
			PaintArrow(p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, color);
		}
		static void PaintMesh(const Point3D &p0, const Point3D &p1, const Point3D &p2, const Point3D &p3, const Color &linCol){
			PaintLine(p0, p1, linCol);
			PaintLine(p1, p2, linCol);
			PaintLine(p2, p3, linCol);
			PaintLine(p3, p0, linCol);
		}
		static void PaintSurface0(const Vector<Point3D> &nodes, const Vector<Panel> &panels, bool showMesh, bool showNormals,double len, const Color &linCol){
			for (int ip = 0; ip < panels.GetCount(); ++ip) {
				const Panel &panel = panels[ip];
				if (showMesh) {
					Point3D p0 = nodes[panel.id[0]];
					Point3D p1 = nodes[panel.id[1]];
					Point3D p2 = nodes[panel.id[2]];
					Point3D p3 = nodes[panel.id[3]];
					PaintMesh(p0, p1, p2, p3, linCol);
				}
				if (showNormals) {
					Segment3D seg(panel.centroidPaint, panel.normalPaint, len);
					PaintArrow(seg.from, seg.to, Blue());
				}
			}
		}
};
}


#endif
