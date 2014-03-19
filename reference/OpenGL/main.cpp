#include <CtrlLib/CtrlLib.h>
#include <GLCtrl/GLCtrl.h>

using namespace Upp;

struct OpenGLExample : GLCtrl {
	Point point;

	virtual void GLPaint() {
		StdView();

		double t = 13 * point.x;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glLoadIdentity();
		glPushMatrix();
			glTranslatef(0, 0, -6);
			glRotated(t / 30.0, 0, 1, 0);
			glRotated(t / 40.0, 1, 0, 0);
			glRotated(t / 5.0, 0, 0, 1);
			glBegin(GL_TRIANGLE_FAN);
				glColor4d(0.8, 0.4, 0.2, 1);
				glVertex3f(0.0f, 0.0f, 0.0f);
				for(int i = 0; i < 50; i++) {
					double u = M_2PI * i / 49;
					glColor4d(i / 50.0, 0.5 + i / 100.0, 0.7 - i / 150.0, 1);
					glVertex3d(sin(u), cos(u), 1.0f);
				}
			glEnd();
			glBegin(GL_TRIANGLE_FAN);
				glColor4d(0.8, 0.4, 0.2, 1);
				glVertex3f(0.0f, 0.0f, 3.0f);
				for(int i = 0; i < 50; i++) {
					double u = M_2PI * i / 49;
					glColor4d(0.7, 0.5 - i / 100.0, 0.7 + i / 150.0, 1);
					glVertex3d(sin(u), cos(u), 1.0f);
				}
			glEnd();
		glPopMatrix();
		t = 13 * point.y;
		float sn = (float)sin(t / 300);
		float cs = (float)cos(t / 300);
		glTranslatef(sn + cs, sn, -6 - cs);
		glRotated(t / 12.0, 0, 0, 1);
		glRotated(t / 15.0, 0, 1, 0);
		glRotated(t / 17.0, 1, 0, 0);
		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glColor4d(1, 0, 0, 0.5);
			glVertex3d(-1, 0, 0);
			glVertex3d(1, 0, 0);
			glVertex3d(0, 0.87, 0);
			glColor4d(0, 1, 0, 0.5);
			glVertex3d(0, 0, 1.67);
		glEnd();
	}
	
	virtual void GLResize(int w, int h) {
		glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	}

	virtual void MouseMove(Point p, dword) {
		point = p;
		Refresh();
	}
};

GUI_APP_MAIN
{
	Ctrl::GlobalBackPaint();
	TopWindow win;
	OpenGLExample gl;
	gl.SetFrame(InsetFrame());
	win.Add(gl.HSizePos(10, 10).VSizePos(10, 10));
	win.Sizeable().Zoomable();
	win.Open();
	win.Run();
}
