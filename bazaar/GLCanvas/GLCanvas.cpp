#include <Core/Core.h>

#include <plugin/jpg/jpg.h>
#include <PdfDraw/PdfDraw.h>

#define IMAGECLASS GLCanvasImg
#define IMAGEFILE <GLCanvas/GLCanvas.iml>
#include <Draw/iml.h>

#include <GLCanvas/GLCanvas.h>

namespace Upp {

GLCanvas::GLCanvas() {
	WhenGLPaint = THISBACK(OnPaint);
	
	WantFocus();
	
	trackBall.Init(this, Ctrl::LEFT);
	buttonContext = Ctrl::RIGHT;
	
	SetCamera();
}

Image GLCanvas::MouseEvent(int event, Point p, int zdelta, dword keyflags) {
	if (((event & Ctrl::BUTTON) == buttonContext) && ((event & Ctrl::ACTION) == Ctrl::UP)) 
		MenuBar::Execute(THISBACK(ContextMenu));
	else if ((event & Ctrl::BUTTON) == Ctrl::LEFT && (event & Ctrl::ACTION) == Ctrl::DOWN) 
		SetFocus();
	
	Image img = trackBall.MouseEvent(event, p, zdelta, keyflags);
	Refresh();
	return img;
}

void GLCanvas::ContextMenu(Bar& bar) {
	bar.Add(t_("Fit to data"), GLCanvasImg::ShapeHandles(), [&]{ZoomToFit();}).Key(K_CTRL_F).Help(t_("Zoom to fit visible all data"));
	bar.Add(t_("Zoom +"), 	   GLCanvasImg::ZoomPlus(),  [&]{Zoom(200);}) .Key(K_CTRL|K_ADD).Help(t_("Zoom in (closer)"));
	bar.Add(t_("Zoom -"), 	   GLCanvasImg::ZoomMinus(), [&]{Zoom(-200);}).Key(K_CTRL|K_SUBTRACT).Help(t_("Zoom out (away)"));									
	bar.Add(t_("View X axis"), 			[&]{View(false, true, true);});
	bar.Add(t_("View Y axis"), 			[&]{View(true, false, true);});
	bar.Add(t_("View Z axis"), 			[&]{View(true, true, false);});	
	bar.Add(t_("View isometric XYZ"), 	[&]{View(true, true, true);});
	bar.Separator();
	bar.Add(t_("Copy image"),  GLCanvasImg::Copy(), [&]{ExecuteGL(THISFN(SaveToClipboard), true);}).Key(K_CTRL_C).Help(t_("Copy image to clipboard"));
	bar.Add(t_("Save image"),  GLCanvasImg::Save(), [&]{ExecuteGL(THISFN(SaveToFile), true);}).Key(K_CTRL_S).Help(t_("Save image to file"));
}

bool GLCanvas::Key(dword key, int ) {
	if (key == K_CTRL_F)
		ZoomToFit();
	else if (key == K_CTRL_ADD || key == (K_CTRL|K_PLUS))
		Zoom(200);
	else if (key == K_CTRL_SUBTRACT || key == (K_CTRL|K_MINUS))
		Zoom(-200);
	else if (key == K_CTRL_C)
		SaveToClipboard();
	else if (key == K_CTRL_S)
		SaveToFile();
	else
		return false;
	return true;
}

void GLCanvas::ZoomToFit() {
	if (env == nullptr) {
		Exclamation(t_("No model is loaded"));
		return;
	}
	double mx = max(max(env->maxX, env->maxY), env->maxZ);
	SetZoomDistance(2*mx);
	Refresh();	
}

void GLCanvas::Zoom(int zdelta) {
	trackBall.Zoom(zdelta);
}

Image GLCanvas::GetImage() {
	Size sz = GetSize();
	ImageBuffer ib(sz);

	glGetError();
	glReadPixels(0, 0, sz.cx, sz.cy, GL_BGRA_EXT, GL_UNSIGNED_BYTE, static_cast<GLvoid*>(~ib));
	if (GL_NO_ERROR != glGetError())
		return Null;
	
	Buffer<RGBA> temp(sz.cx);
	for(int i = 0; i < sz.cy/2; i++){
		memcpy(temp,ib[i], sz.cx*sizeof(RGBA));
		memcpy(ib[i], ib[sz.cy-1-i], sz.cx*sizeof(RGBA));
		memcpy(ib[sz.cy-1-i], temp, sz.cx*sizeof(RGBA));
	}
	
	for (int i = 0; i < sz.cy; i++)
		for (int x = 0; x < sz.cx; x++) 
			ib[i][x].a = 255;

	return ib;
}

void GLCanvas::SaveToClipboard() {
	GuiLock __;
	Image image = GetImage();
	if (IsNull(image)) {
		Exclamation(t_("Imposible to get view image"));
		return;
	}
	
	WriteClipboardImage(image);	
}

void GLCanvas::OnTypeImage(FileSel *_fs) {
	FileSel &fs = *_fs;
	int id = fs.type.GetIndex();
	
	if (id == 0)
		fs.file = ForceExt(GetFileName(~fs), ".jpg");
	else if (id == 1)
		fs.file = ForceExt(GetFileName(~fs), ".png");
	else if (id == 2)
		fs.file = ForceExt(GetFileName(~fs), ".pdf");
}

void GLCanvas::SaveToFile() {
	GuiLock __;

	Image image = GetImage();
	if (IsNull(image)) {
		Exclamation(t_("Imposible to get view image"));
		return;
	}
	
	FileSel fs;
	fs.Type(Format(t_("%s bitmap file"), "jpeg"), "*.jpg");
	fs.Type(Format(t_("%s bitmap file"), "png"), "*.png");
	fs.Type(Format(t_("%s vector file"), "pdf"), "*.pdf");
	fs.AllFilesType();
	if (!defaultFileName.IsEmpty())
		fs = defaultFileName;
	else
		fs = String(t_("Mesh view")) + ".jpg";
	
	String ext = GetFileExt(~fs);
	fs.DefaultExt(ext);
	int idt = 0;
	if (ext == ".jpg" || ext == ".jpeg")
		idt = 0;
	else if (ext == ".png")
		idt = 1;
	else if (ext == ".pdf")
		idt = 2;
	fs.ActiveType(idt);

	fs.ActiveDir(GetFileFolder(defaultFileName));
	fs.type.WhenAction = THISBACK1(OnTypeImage, &fs); 
    if(!fs.ExecuteSaveAs(t_("Saving image to file"))) {
        Exclamation(t_("Image has not been saved"));
        return;
    }
    String fileName = defaultFileName = ~fs;
	 
	if (GetFileExt(fileName) == ".png") {
		WaitCursor waitcursor;
		PNGEncoder encoder;
		encoder.SaveFile(fileName, image);
	} else if (GetFileExt(fileName) == ".jpg") {	
		WaitCursor waitcursor;
		JPGEncoder encoder(jpgQuality);
		encoder.SaveFile(fileName, image);
	} else if (GetFileExt(fileName) == ".pdf") {	
		WaitCursor waitcursor;
		PdfDraw pdf(GetSize().cx, GetSize().cy);
		pdf.DrawImage(0, 0, image);
		SaveFile(fileName, pdf.Finish());		
	} else 
		Exclamation(Format(t_("File format \"%s\" not found"), GetFileExt(fileName)));
}

void GLCanvas::SetUpLighting() {
	float light1_ambient[4]  = { 1.0f, 1.0f, 1.0f, 1.0f };
	float light1_diffuse[4]  = { 1.0f, 0.9f, 0.9f, 1.0f };
	float light1_specular[4] = { 1.0f, 0.7f, 0.7f, 1.0f };
	float light1_position[4] = { -1.0, 1.0, 1.0, 0.0f };
	glLightfv(GL_LIGHT1, GL_AMBIENT,  light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glEnable(GL_LIGHT1);
	
	float light2_ambient[4]  = { 0.2f, 0.2f, 0.2f, 1.0f };
	float light2_diffuse[4]  = { 0.9f, 0.9f, 0.9f, 1.0f };
	float light2_specular[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float light2_position[4] = { 1.0, -1.0, -1.0, 0.0f };
	glLightfv(GL_LIGHT2, GL_AMBIENT,  light2_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE,  light2_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	//glEnable(GL_LIGHT2);
	
	float front_emission[4] = { 0.3f, 0.2f, 0.1f, 0.0f };
	float front_ambient[4]  = { 0.2f, 0.2f, 0.2f, 0.0f };
	float front_diffuse[4]  = { 0.95f, 0.95f, 0.8f, 0.0f };
	float front_specular[4] = { 0.6f, 0.6f, 0.6f, 0.0f };
	glMaterialfv(GL_FRONT, GL_EMISSION, front_emission);
	glMaterialfv(GL_FRONT, GL_AMBIENT, front_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, front_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, front_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 16.0);
	glColor4fv(front_diffuse);
	
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glEnable(GL_CULL_FACE);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
}

void GLCanvas::SetCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double angle = trackBall.GetZoomAngle();
	gluPerspective(angle, static_cast<float>(GetSize().cx)/GetSize().cy, 1, 10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, static_cast<float>(GetSize().cy/2.), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void GLCanvas::SetZoomDistance(double distance) {
	double angle = atan2(distance, GetSize().cy/2.)*180/M_PI;
	trackBall.SetZoomAngle(angle);
}

void GLCanvas::View(bool x, bool y, bool z) {
	trackBall.ViewXYZ(x, y, z);	
	Refresh();
}

void GLCanvas::Layout() {
	GLCtrl::Layout();
	glMatrixMode (GL_MODELVIEW);
	glViewport (0, 0, GetSize().cx, GetSize().cy);
	glLoadIdentity();
	SetCamera();
	trackBall.Reshape(GetSize().cx, GetSize().cy);
}

void GLCanvas::OnPaint() {
	if (!IsEnabled())
		return;
	
	MemoryIgnoreLeaksBlock __; 
	
	glClearColor(1, 1, 1, 0);
	glClearDepth(0); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	SetCamera();
	glTranslatef(0, 0, 0);
	trackBall.Matrix();
	
	glPushMatrix();
	glDisable(GL_BLEND);
	//SetUpLighting();
	WhenPaint();
	glPopMatrix();
}

void GLCanvas::PaintLine(double x0, double y0, double z0, double x1, double y1, double z1, const Color &color) {
	glBegin(GL_LINES);
		glColor4d(color.GetR()/255., color.GetG()/255., color.GetB()/255., 1);
		glVertex3d(x0, y0, z0);
		glVertex3d(x1, y1, z1); 
	glEnd();
}

void GLCanvas::PaintLine(const Point3D &p0, const Point3D &p1, const Color &color) {
	PaintLine(p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, color);
}

void GLCanvas::PaintLine(const Segment3D &p, const Color &color) {
	PaintLine(p.from, p.to, color);
}

void GLCanvas::PaintArrow(double x0, double y0, double z0, double x1, double y1, double z1, const Color &color) {
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
	PaintLine(seg.to,   parr1, 	  color);
	PaintLine(seg.to,   parr2,    color);
	PaintLine(parr1,    parr2,    color);
}

void GLCanvas::PaintArrow(const Point3D &p0, const Point3D &p1, const Color &color) {
	PaintArrow(p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, color);
}

void GLCanvas::PaintQuad(const Point3D &p0, const Point3D &p1, const Point3D &p2, const Point3D &p3, const Color &color, double alpha) {
	glBegin(GL_QUADS);
		glColor4d(color.GetR()/255., color.GetG()/255., color.GetB()/255., alpha);
		glVertex3d(p0.x, p0.y, p0.z);
		glVertex3d(p1.x, p1.y, p1.z);
		glVertex3d(p2.x, p2.y, p2.z);
		glVertex3d(p3.x, p3.y, p3.z);
	glEnd();
}

void GLCanvas::PaintAxis(const Point3D &p, double len) {
	PaintAxis(p.x, p.y, p.z, len);
}

void GLCanvas::PaintAxis(double x, double y, double z, double len) {
	PaintArrow(x, y, z, x+len, y	 , z,     LtRed());
	PaintArrow(x, y, z, x	 , y+len , z,     LtGreen());
	PaintArrow(x, y, z, x	 , y	 , z+len, LtBlue());
}

void GLCanvas::PaintDoubleAxis(const Point3D &p, double len, const Color &color) {
	PaintDoubleAxis(p.x, p.y, p.z, len, color);
}

void GLCanvas::PaintDoubleAxis(double x, double y, double z, double len, const Color &color) {
	PaintLine(x-len/2, y	  , z	   , x+len/2, y	     , z	  , color);
	PaintLine(x		 , y-len/2, z	   , x		, y+len/2, z	  , color);
	PaintLine(x		 , y	  , z-len/2, x		, y	     , z+len/2, color);
}

void GLCanvas::PaintCube(const Point3D &p, double side, const Color &color) {
	PaintCube(p.x, p.y, p.z, side, color);
}

void GLCanvas::PaintCube(double x, double y, double z, double side, const Color &color) {
	PaintCuboid(Point3D(x-side/2., y-side/2., z-side/2.), Point3D(x+side/2., y+side/2., z+side/2.), color);
}

void GLCanvas::PaintCuboid(const Point3D &p0, const Point3D &p1, const Color &color) {
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

void GLCanvas::PaintSegments(const Vector<Segment3D>& segs, const Color &color) {
	for (int i = 0; i < segs.GetCount(); ++i) 
		PaintLine(segs[i], color);
}

void GLCanvas::PaintMesh(const Point3D &p0, const Point3D &p1, const Point3D &p2, const Point3D &p3, const Color &linCol) {
	PaintLine(p0, p1, linCol);
	PaintLine(p1, p2, linCol);
	PaintLine(p2, p3, linCol);
	PaintLine(p3, p0, linCol);
}

void GLCanvas::PaintSurface0(const Vector<Point3D> &nodes, const Vector<Panel> &panels, bool showMesh, bool showNormals, 
							double len, const Color &linCol) {
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

void GLCanvas::PaintSurface(const Surface &surf, const Color &linCol, bool showMesh, bool showNormals) {
	PaintSurface0(surf.nodes, surf.panels, showMesh, showNormals, surf.avgFacetSideLen, linCol);
}

}
