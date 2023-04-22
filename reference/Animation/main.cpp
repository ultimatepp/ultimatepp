#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Animation/Chameleon.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS MyButtonImg
#define IMAGEFILE <Animation/Chameleon.iml>
#include <Draw/iml.h>

struct ChEllipse {
	int   status;
	int   width;
	Color pen;
	Color color;

	ChEllipse() {}
};

Value EllipseLook(int status, int width, Color pen, Color color)
{
	ChEllipse e;
	e.status = status;
	e.width = width;
	e.pen = pen;
	e.color = color;
	return RawToValue(e);
}

Value AnimatedEllipse(Draw& w, Ctrl *button, const Rect& rect, const Value& v, int op, Color)
{
	static ArrayMap<Ctrl *, int> ani;

	if(IsTypeRaw<ChEllipse>(v)) {
		const ChEllipse& e = ValueTo<ChEllipse>(v);
		
		switch(op) {
		case LOOK_MARGINS:
			return Rect(e.width, e.width, e.width, e.width);
		case LOOK_PAINT:
			if(e.status == CTRL_PRESSED) {
				int tm = msecs();
				int animation_start = ani.GetPut(button, tm);
				int animation_time = tm - animation_start;
				w.DrawEllipse(rect.Deflated(e.width), e.color, e.width + min(animation_time / 30, 10), e.pen);
				if(animation_time < 1000) {
					Ptr<Ctrl> h = button;
					SetTimeCallback(20, [=] { if(h) h->Refresh(); });
				}
			}
			else {
				ani.UnlinkKey(button);
				w.DrawEllipse(rect.Deflated(e.width), e.color, e.width, e.pen);
			}
			return 1;
		}
	}
	return Null;
}

INITBLOCK {
	ChLookFn(AnimatedEllipse);
}

Button::Style AnimationStyle()
{
	Button::Style s = Button::StyleNormal();
	s.look[0] = EllipseLook(0, 2, Black, LtGray);
	s.look[1] = EllipseLook(1, 3, Red, WhiteGray);
	s.look[2] = EllipseLook(2, 4, Blue, White);
	s.look[3] = EllipseLook(3, 1, Black, Gray);
	s.focus_use_ok = false; // otherwise Button would use OK style for focused button
	return s;
}

void AnimationSkin()
{
	Button::StyleNormal().Write() = AnimationStyle();
}

struct MyApp : WithChameleonLayout<TopWindow> {
	Button::Style scolor, simage, sfn;

	MyApp();
};

MyApp::MyApp()
{
	CtrlLayout(*this, "Animation example");
	SetSkin(AnimationSkin);
}

GUI_APP_MAIN
{
	MyApp().Run();
}
