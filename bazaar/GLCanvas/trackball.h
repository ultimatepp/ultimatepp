#ifndef _FTGL_Demo_TrackBall_h_
#define _FTGL_Demo_TrackBall_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class TrackBall {
public:
	typedef TrackBall CLASSNAME;

	TrackBall();
	void Init(Ctrl *owner);
	
	void Matrix(void);
	void Reshape(int width, int height);
	void SetZoomFactor(double factor)		{factor = zoomFactor;}
	double GetZoomFactor()					{return zoomFactor;}
	void SetButtonRot(int but = Ctrl::LEFT) {buttonRot = but; }
	

	virtual Image MouseEvent(int event, Point p, int zdelta, dword keyflags);
	
	float curquat[4];
	
private:
	Ctrl *owner;

//	float curquat[4];
	float lastquat[4];
	int beginx, beginy;
	
	int width, height;
	
	int buttonRot;
	bool tracking;
	
	double zoomFactor;
};

#endif
