#include "TrackBall.h"

#include "trkball.h"

#ifdef PLATFORM_POSIX
#include <GL/glut.h>
#elif defined(WIN32)
#include <gl/glu.h>
#endif

void TrackBall::_Animate(void)
{
	add_quats(lastquat, curquat, curquat);
	owner->Refresh();
	owner->RefreshLayoutDeep();
	if(enableAnimCb)
		owner->SetTimeCallback(10, THISBACK(_Animate));
}

void TrackBall::_StartMotion(int x, int y, Time time)
{
	enableAnimCb = false;
	tracking = true;
	lastTime = time;
	beginx = x;
	beginy = y;
}

void TrackBall::_StopMotion(Time time)
{
	tracking = false;

	if (time == lastTime && animate)
	{
		enableAnimCb = true;
		_Animate();
	}
	else
		if (animate)
			enableAnimCb = false;
}

void TrackBall::Animate(bool a)
{
	animate = a;
}

void TrackBall::Matrix(void)
{
	GLfloat m[4][4];

	build_rotmatrix(m, curquat);
	glMultMatrixf(&m[0][0]);
}

void TrackBall::Reshape(int w, int h)
{
	width  = w;
	height = h;
}

// mouse event handler
Image TrackBall::MouseEvent(int event, Point p, int zdelta, dword keyflags)
{
	// mouse up/down starts and stop motion
	if ((event & Ctrl::BUTTON) == button && (event & Ctrl::ACTION) == Ctrl::DOWN)
		_StartMotion(p.x, p.y, GetSysTime());
	else
		if ((event & Ctrl::BUTTON) == button && (event & Ctrl::ACTION) == Ctrl::UP)
			_StopMotion(GetSysTime());
	
	// mouse move do the animation, if we're tracking
	else if((event & Ctrl::ACTION) == Ctrl::MOUSEMOVE)
	{
		if (tracking)
		{
			trackball(
				lastquat,
				(2.0 * beginx - width) / width,
				(height - 2.0 * beginy) / height,
				(2.0 * p.x - width) / width,
				(height - 2.0 * p.y) / height
			);
			beginx = p.x;
			beginy = p.y;
			animate = true;
			lastTime = GetSysTime();
			_Animate();
		}
	}

	return Image::Hand();
}

TrackBall::TrackBall(Ctrl *ow)
{
	owner = ow;
	button = Ctrl::LEFT;
	tracking = false;
	animate = false;
	trackball(curquat, 0.0, 0.0, 0.0, 0.0);
	enableAnimCb = false;
}
