#ifndef _robot_robot_h
#define _robot_robot_h

#include <CtrlLib/CtrlLib.h>
#include <GLCtrl/GLCtrl.h>
#include <GL/glaux.h>

#define LAYOUTFILE <robot/robot.lay>
#include <CtrlCore/lay.h>


class _przeszkoda:Moveable<_przeszkoda> {
public:
	GLfloat x1,x2,y1,y2;
	_przeszkoda(){x1=0;x2=0;y1=0;y2=0;}
};


struct OpenGL : GLCtrl {
AUX_RGBImageRec *LoadBMP(char *Filename);
int LoadGLTextures();
int InitGL();
void NextMove();
Vector<_przeszkoda> listaP;
GLuint	texture[1];			// Storage For One Texture
GLfloat szerokosc,dlugosc,wysokosc,rx,ry,dx,dy;
GLfloat sceneroty,lookupdown;
int ilP;
GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
virtual void GLPaint();
};

class robot : public WithrobotLayout<TopWindow> {
public:

	typedef robot CLASSNAME;
	EditField arr_edit;
	WithzadanieLayout<StaticRect> zadanie;
	WithterenLayout<StaticRect> teren;
	WithprobotaLayout<StaticRect> probota;

	EditDouble x1, y1, x2, y2;

	OpenGL gl;
	void Change();
	void UstawTeren();
	void ilPrzeszkod();
	robot();
};

#endif
