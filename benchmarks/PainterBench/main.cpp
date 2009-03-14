#include <GLCtrl/GLCtrl.h>
#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

#define LINE

struct PainterExample : Ctrl {
public:
	double delta;
	virtual void Paint(Draw &draw) {
		delta=0;
		
		ImageBuffer ib(draw.GetPagePixels());
		BufferPainter pntr(ib);
		
		dword begin=GetTickCount();
		
		int reps=0;
		for(reps=0;reps<5;reps++) for(int i=0;i<500;i++){
			#ifdef LINE
				pntr.Move(i,0).Line(499,i).Line(499-i,499).Line(0,499-i).Close().Stroke(4,Red());
			#else
				pntr.Move(i,0).Line(499,i).Line(499-i,499).Line(0,499-i).Close().Fill(Red());
			#endif
		}
		dword end=GetTickCount();
		delta=end-begin;
		delta/=reps;
		draw.DrawImage(0,0,ib);
	}
};

struct DrawExample : Ctrl {
public:
	double delta;
	virtual void Paint(Draw &draw) {
		delta=0;
		
		dword begin=GetTickCount();
		Rect rect(0,0,500,500);
		
		int reps=0;
		for(reps=0;reps<5;reps++) for(int i=0;i<500;i++){
			Point v[5]={Point(i,0),Point(499,i),Point(499-i,499),Point(0,499-i),Point(i,0)};
			#ifdef LINE
				draw.DrawPolyline(v,5,4,Green());
			#else
				draw.DrawPolygon(v,5,Green());
			#endif
		}
		dword end=GetTickCount();
		delta=end-begin;
		delta/=reps;
	}

};

struct OpenGLExample : GLCtrl {
public:
	double delta;
	virtual void GLPaint() {
		delta=0;
		Size sz=GetSize();
		dword begin=GetTickCount();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0,0,sz.cx,sz.cy);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glPushMatrix();
		glOrtho(0,sz.cx,sz.cy,0,-1,1);
		
		
		int reps=0;
		for(reps=0;reps<10;reps++) for(int i=0;i<500;i++){
		#ifdef LINE
			glBegin(GL_LINE_STRIP); // Polyline
		#else
			glBegin(GL_POLYGON); // Polygon
		#endif
			glColor3f(0.0f,0.0f,1.0f); // Blue
			glVertex2f((float)i,(float)0);
			glVertex2f((float)499,(float)i);
			glVertex2f((float)499-i,(float)499);
			glVertex2f((float)0,(float)499-i);
			glVertex2f((float)i,(float)0);
			glEnd();
			glFlush();
		}
		
		glPopMatrix();
		glFlush();

		dword end=GetTickCount();
		delta=end-begin;
		delta/=reps;
	}
};

class ExampleTopWindow: public TopWindow{
public:
	PainterExample pr;
	DrawExample dw;
	OpenGLExample gl;
		
	ExampleTopWindow(){
		pr.SetRect(0,0,500,500);
		dw.SetRect(500,0,500,500);
		gl.SetRect(1000,0,500,500);
		
		pr.BackPaint();
		dw.BackPaint();
		gl.BackPaint();
		
		pr.delta=0;
		dw.delta=0;
		gl.delta=0;
		
		Add(pr);
		Add(dw);
		Add(gl);
	}

	virtual void LeftDown(Point p,dword keyflags){
		Title(Format("Painter/Draw/OpenGL: %.3f/%.3f/%.3f ms",pr.delta,dw.delta,gl.delta));
		Refresh();
	}

};

GUI_APP_MAIN
{
	ExampleTopWindow win;
	win.Sizeable().Zoomable();
	win.Open();
	win.Run();
}
