#ifndef _FTGL_Demo_TrackBall_h_
#define _FTGL_Demo_TrackBall_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class TrackBall
{
	private:
	
		Ctrl *owner;
	
		Time lastTime;
		
		float curquat[4];
		float lastquat[4];
		int beginx, beginy;
		
		int width;
		int height;
		
		int button;
		bool tracking;
		bool animate;

		void _Animate(void);
		void _StartMotion(int x, int y, Time time);
		void _StopMotion(Time time);
		
		bool enableAnimCb;

	protected:
	
	public:
	
		typedef TrackBall CLASSNAME;

		void Matrix(void);
		void Reshape(int width, int height);
		void Animate(bool animate);
		
		void SetButton(int but = Ctrl::LEFT) { button = but; }
		
		virtual Image MouseEvent(int event, Point p, int zdelta, dword keyflags);
		
		TrackBall(Ctrl *owner);
};

#endif
