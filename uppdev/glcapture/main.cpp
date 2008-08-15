#include <CtrlLib/CtrlLib.h>
#include <GLCtrl/GLCtrl.h>
#include <stdlib.h>
#include <math.h>
//7#include <glut.h>

using namespace Upp;

#define LAYOUTFILE <SphereSkin/SphereSkin.lay>
#include <CtrlCore/lay.h>

class Ball
{
public:
	float x,y,z,r;
};

double rand01() { return rand()/(double)(RAND_MAX+1); }
double rand10() { return -10+20*rand01(); }

//double randn() { return (drand48()+drand48()+drand48())/3.0-0.5; }


class MyGLArea : public GLCtrl {
public:
	Vector<Ball*> balls;
	float x_angle_deg;
	float y_angle_deg;
	bool mousedown;
	Point pprev;

	MyGLArea() {
		mousedown=false;
		x_angle_deg=0;
		y_angle_deg=0;

		int i;
		for(i=0;i<200;i++) {
			Ball* b = new Ball;
			b->x = rand10();
			b->y = rand10();
			b->z = rand10();
			b->r = exp(log(0.5)+rand01()*(log(2)-log(0.5)));
			balls.Add(b);
		}


	}


	virtual void GLPaint() {
		StdView();

		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		float s=15;
		glOrtho(-s,s, -s,s, -s,s);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glRotatef(x_angle_deg,0,1,0);
		glRotatef(y_angle_deg,1,0,0);

		#if 0
		glColor3f(1,1,1);
		glBegin(GL_QUADS);
			glVertex2f(-9,-9);
			glVertex2f( 9,-9);
			glVertex2f( 9, 9);
			glVertex2f(-9, 9);
		glEnd();
		#else

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_NORMALIZE);
		int i;
		for(i=0;i<balls.GetCount();i++) {
			Ball* b = balls.At(i);
			glPushMatrix();
			glTranslatef(b->x,b->y,b->z);
			glutSolidSphere(b->r, 20,20);
			glPopMatrix();
		}

		#endif

		glFlush();
	}

    virtual void LeftDown(Point p, dword keyflags) {
        SetCapture();
    	pprev = p;
    }

    virtual void MouseMove(Point p, dword keyflags) {
       if(IsCapture()) {
           int dx=p.x-pprev.x;
           int dy=p.y-pprev.y;
           x_angle_deg += dx;
           y_angle_deg += dy;
           pprev = p;
           this->Refresh();
       }
    }

    virtual void LeftUp(Point p, dword keyflags) {
		mousedown=false;
    }

	virtual bool Key(dword key, int count) {
		switch(key) {
			case 'a':
				::PromptOK("'a' key was pressed");
				break;
		}
	}
};

class SphereSkin : public WithSphereSkinLayout<TopWindow> {
public:
	MyGLArea gl_area;
	StatusBar status;

	typedef SphereSkin CLASSNAME;

	SphereSkin() {
		CtrlLayout(*this, "SphereSkin");
		this->SetFrameRect(50,50, 500,500);
		Sizeable().Zoomable();
		Add(gl_area.SizePos());
		AddFrame(status);
	}
};


GUI_APP_MAIN
{
	SphereSkin().Run();
}
