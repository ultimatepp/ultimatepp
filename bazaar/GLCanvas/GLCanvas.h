#ifndef _GLCanvas_GLCanvas_h_
#define _GLCanvas_GLCanvas_h_

#include <GLCtrl/GLCtrl.h>
#include "trackball.h"
#include <Surface/Surface.h>

namespace Upp {
	
class GLCanvas : public GLCtrl {
public:
	typedef GLCanvas CLASSNAME;

	GLCanvas();

private:
	TrackBall trackBall;

	void SetUpLighting();
	void SetCamera();
	
	void ContextMenu(Bar& bar);
	int buttonContext;
	void ExportViewAsImage();
	
	VolumeEnvelope *env = nullptr;
	String defaultFileName;
	
	int jpgQuality = 90;
		
protected:
	virtual void Layout();
	virtual Image MouseEvent(int event, Point p, int zdelta, dword keyflags);
	virtual bool Key(dword key, int count);
	
	template <class T>
	void Ize(T& io) { 
		io
			("defaultFileName", defaultFileName)
			("jpgQuality", jpgQuality)
		;
	}

public:
	void Xmlize(XmlIO& xml) 	{Ize(xml);}
	void Jsonize(JsonIO& json) 	{Ize(json);}
	
	void Serialize(Stream& s) {
		s % defaultFileName
		  % jpgQuality
		;
	}
	
	void PaintLine(double x0, double y0, double z0, double x1, double y1, double z1, const Color &color);
	void PaintLine(const Point3D &p0, const Point3D &p1, const Color &color);
	void PaintLine(const Segment3D &p, const Color &color);
	void PaintQuad(const Point3D &p0, const Point3D &p1, const Point3D &p2, const Point3D &p3, const Color &color, double alpha = 1);
	void PaintCuboid(const Point3D &p0, const Point3D &p1, const Color &color);	
	void PaintSurface(const Surface &surf, const Color &linCol, bool showMesh, bool showNormals);
	void PaintSegments(const Vector<Segment3D>& segs, const Color &color);
	void PaintAxis(double x, double y, double z, double len);
	void PaintAxis(const Point3D &p, double len);
	void PaintDoubleAxis(double x, double y, double z, double len, const Color &color);
	void PaintDoubleAxis(const Point3D &p, double len, const Color &color);
	void PaintCube(const Point3D &p, double side, const Color &color);
	void PaintCube(double x, double y, double z, double side, const Color &color);
	void PaintArrow(double x0, double y0, double z0, double x1, double y1, double z1, const Color &color); 
	void PaintArrow(const Point3D &p0, const Point3D &p1, const Color &color);
			
	void SetZoomDistance(double distance);
	void View(bool x, bool y, bool z);
	void ZoomToFit();
	void Zoom(int zdelta); 
	
	Image GetImage();
	void SaveToClipboard();
	void SaveToFile();
	
	Function <void()> WhenPaint;	
	
	void SetEnv(VolumeEnvelope &_env) 	 {env = &_env;}
	void OnPaint();	
	
	GLCanvas &SetJPGQuality(int quality) {jpgQuality = quality; return *this;}
	int GetJPGQuality() 				 {return jpgQuality;}
	
	virtual void GLResize(int w, int h) {
		glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
	}
	
private:
	void PaintSurface0(const Vector<Point3D> &nodes, const Vector<Panel> &panels, bool showMesh, bool showNormals, 
							double len, const Color &linCol);
	void PaintMesh(const Point3D &p0, const Point3D &p1, const Point3D &p2, const Point3D &p3, const Color &linCol);
	void OnTypeImage(FileSel *_fs);
};

}

#endif
