class ViewDraw : public SystemDraw {
	ImageBuffer ib;
	
	Vector<Rect> dummy_invalid;

public:
	ViewDraw(Ctrl *ctrl) : SystemDraw(ib, dummy_invalid) { _DBG_ }
	~ViewDraw() {}
};

class DHCtrl : Ctrl {};

#include FRAMEBUFFER_INCLUDE
