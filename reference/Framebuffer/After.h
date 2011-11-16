class ViewDraw : public SystemDraw {
public:
	ViewDraw(Ctrl *ctrl);
	~ViewDraw();
};


/*
class ViewDraw : public SystemDraw {
	Vector<Rect> dummy;
public:
	ViewDraw(Ctrl *) : SystemDraw(Ctrl::framebuffer, dummy) { dummy.Add(Rect(10, 10, 100, 100)); }
};
*/
class DHCtrl : Ctrl {};

#include FRAMEBUFFER_INCLUDE
