#include <GLCanvas/GLCanvas.h>

using namespace Upp;


GLCanvas::GLCanvas() {
	WhenGLPaint = THISBACK(OnPaint);
	
	WantFocus();
	
	trackBall.Init(this);
	
	SetCamera();
}

Image GLCanvas::MouseEvent(int event, Point p, int zdelta, dword keyflags) {
	Image img = trackBall.MouseEvent(event, p, zdelta, keyflags);
	Refresh();
	return img;
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

void GLCanvas::SetCamera(void) {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(trackBall.GetZoomFactor(), (float)GetSize().cx / (float)GetSize().cy, 1, 10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, (float)GetSize().cy/2., 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
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
	MemoryIgnoreLeaksBlock __; 
	
	glClearColor(1, 1, 1, 0);
	glClearDepth(0); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	SetCamera();
	glTranslatef(0, 0, 0);
	trackBall.Matrix();
	
	glPushMatrix();
	glDisable(GL_BLEND);
	SetUpLighting();
	WhenPaint();
	glPopMatrix();
}