#include "TrackBall.h"
#include "trkball.h"

#ifdef PLATFORM_POSIX
#include <GL/glut.h>
#elif defined(WIN32)
#include <gl/glu.h>
#endif

TrackBall::TrackBall() {
	owner = NULL;
	buttonRot = Ctrl::LEFT;
	tracking = false;
	zoomFactor = 10;
	//trackball(curquat, 0, -.5, -1, 1);
	curquat[0] = 0.15f;	curquat[1] = 0.35f;	curquat[2] = 0.85f;	curquat[3] = 0.35f;
}

void TrackBall::Init(Ctrl *ow) {
	owner = ow;
}

void TrackBall::Matrix() {
	GLdouble m[4][4];

	build_rotmatrix(m, curquat);
	glMultMatrixd(&m[0][0]);
}

void TrackBall::Reshape(int w, int h) {
	width  = w;
	height = h;
}

Image TrackBall::MouseEvent(int event, Point p, int zdelta, dword keyflags) {
	if ((event & Ctrl::ACTION) == Ctrl::MOUSEWHEEL) {
		zoomFactor = min<double>(180., zoomFactor * (1 - zdelta/1000.));
		owner->Refresh();
	} else if ((event & Ctrl::BUTTON) == buttonRot && (event & Ctrl::ACTION) == Ctrl::DOWN) {
		tracking = true;
		beginx = p.x;
		beginy = p.y;
	} else {
		if ((event & Ctrl::BUTTON) == buttonRot && (event & Ctrl::ACTION) == Ctrl::UP)
			tracking = false;
		else if((event & Ctrl::ACTION) == Ctrl::MOUSEMOVE) {
			if (tracking) {
				trackball(
					lastquat,
					float((2.0 * beginx - width) / width),
					float((height - 2.0 * beginy) / height),
					float((2.0 * p.x - width) / width),
					float((height - 2.0 * p.y) / height)
				);
				beginx = p.x;
				beginy = p.y;
				add_quats(lastquat, curquat, curquat);
				owner->Refresh();
			}
		}
	}
	return Image::Hand();
}
