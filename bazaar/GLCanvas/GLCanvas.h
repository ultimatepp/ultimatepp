#ifndef _GLCanvas_GLCanvas_h_
#define _GLCanvas_GLCanvas_h_

#include <GLCtrl/GLCtrl.h>
#include "trackball.h"
#include "surface.h"

class GLCanvas : public GLCtrl {
public:
	typedef GLCanvas CLASSNAME;

	GLCanvas();

private:
	TrackBall trackBall;

	void SetUpLighting();
	void SetCamera();
	
protected:
	virtual void Layout();
	virtual Image MouseEvent(int event, Point p, int zdelta, dword keyflags);
	
public:
	void PaintLine(double x0, double y0, double z0, double x1, double y1, double z1, Color color) {
		glBegin(GL_LINES);
			glColor4d(color.GetR()/255., color.GetG()/255., color.GetB()/255., 1);
			glVertex3d(x0, y0, z0);
			glVertex3d(x1, y1, z1); // 
		glEnd();
	}
	
	void PaintAxis(double x, double y, double z) {
		PaintLine(0, 0, 0, x, 0, 0, LtRed());
		PaintLine(0, 0, 0, 0, y, 0, LtGreen());
		PaintLine(0, 0, 0, 0, 0, z, LtBlue());
	}
	
	void PaintSurface(Surface &surf, Color linCol = LtGreen()) {
		PaintSurface0(surf, linCol, false, false);
		if (surf.x0z)
			PaintSurface0(surf, linCol, false, true);
		if (surf.y0z)
			PaintSurface0(surf, linCol, true, false);
	}

	void SetZoomFactor(double factor)	{trackBall.SetZoomFactor(factor);}
		
	Function <void()> WhenPaint;	
	
	void OnPaint();	
	
	virtual void GLResize(int w, int h) {
		glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	}
	
private:
	void PaintSurface0(Surface &surf, Color linCol, bool simX, bool simY) {
		double xsig = simX ? -1 : 1;
		double ysig = simY ? -1 : 1;
		
		for (int ip = 0; ip < surf.panels.GetCount(); ++ip) {
			Panel &panel = surf.panels[ip];
			Point3D &p0 = surf.nodes[panel.id0];
			Point3D &p1 = surf.nodes[panel.id1];
			Point3D &p2 = surf.nodes[panel.id2];
			Point3D &p3 = surf.nodes[panel.id3];
		
			/*glBegin(GL_QUADS);
				glColor4d(1.0, 0.5, 0.0, 1);	
				if (!sim) {
					glVertex3d(p0.x, p0.y*ysig, p0.z);
					glVertex3d(p1.x, p1.y*ysig, p1.z);
					glVertex3d(p2.x, p2.y*ysig, p2.z);
					glVertex3d(p3.x, p3.y*ysig, p3.z);
				} else {
					glVertex3d(p3.x, p3.y*ysig, p3.z);
					glVertex3d(p2.x, p2.y*ysig, p2.z);
					glVertex3d(p1.x, p1.y*ysig, p1.z);
					glVertex3d(p0.x, p0.y*ysig, p0.z);
				}
			glEnd();*/	
			
			PaintLine(p0.x*xsig, p0.y*ysig, p0.z, p1.x*xsig, p1.y*ysig, p1.z, linCol);
			PaintLine(p1.x*xsig, p1.y*ysig, p1.z, p2.x*xsig, p2.y*ysig, p2.z, linCol);
			PaintLine(p2.x*xsig, p2.y*ysig, p2.z, p3.x*xsig, p3.y*ysig, p3.z, linCol);
			PaintLine(p3.x*xsig, p3.y*ysig, p3.z, p0.x*xsig, p0.y*ysig, p0.z, linCol);
		}
	}
};


#endif
