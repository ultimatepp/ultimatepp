#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Animation/Chameleon.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS MyButtonImg
#define IMAGEFILE <Animation/Chameleon.iml>
#include <Draw/iml.h>

struct ChAnimatedButtonLook {
	int   status;
	int   width;
	Color pen;
	Color color;

	ChAnimatedButtonLook() {}
};

Value AnimatedEllipse(Ctrl *ctrl, Draw& draw, const Rect& rect, const Value& v, int op, Color)
{
	struct Rec {
		Ptr<Ctrl> ctrl;
		int       start = 0;
	};
	
	static ArrayMap<Ctrl *, Rec> rec;
	
	rec.RemoveIf([](int i) { return !rec[i].ctrl; });

	if(IsTypeRaw<ChAnimatedButtonLook>(v)) {
		const auto& e = ValueTo<ChAnimatedButtonLook>(v);
		
		switch(op) {
		case LOOK_MARGINS:
			return Rect(e.width, e.width, e.width, e.width);
		case LOOK_PAINT:
			draw.Clipoff(rect);
			{
				Size sz = rect.GetSize();
				DrawPainter w(draw, rect.GetSize());
				w.Clear();
				w.RoundedRectangle(Rect(sz).Deflated(DPI(2)), DPI(3)).Fill(e.color);
				w.Stroke(DPI(e.width), e.pen);
				
				if(ctrl) {
					if(e.status == CTRL_PRESSED) {
						Rec& r = rec.GetAdd(ctrl);
						int tm = msecs();
						if(!r.ctrl) {
							r.ctrl = ctrl;
							r.start = tm;
						}

						double ani = (msecs() - r.start) / 3;

						if(ani < 300) {
							w.Circle(Rect(sz).CenterPoint(), ani).Fill((int)max(255 - ani, 0.0) * SLtGray());
							Ptr<Ctrl> h = ctrl;
							SetTimeCallback(20, [=] { if(h) h->Refresh(); });
						}
					}
					else {
						int q = rec.Find(ctrl);
						if(q >= 0 && rec[q].ctrl)
							rec[q].ctrl = nullptr;
					}
				}
			}
			draw.End();
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

	auto Look = [](int status, int width, Color pen, Color color) {
		ChAnimatedButtonLook e;
		e.status = status;
		e.width = width;
		e.pen = pen;
		e.color = color;
		return RawToValue(e);
	};

	s.look[0] = Look(0, 1, Black, LtGray);
	s.look[1] = Look(1, 2, Blue, WhiteGray);
	s.look[2] = Look(2, 2, Blue, White);
	s.look[3] = Look(3, 1, Black, Gray);
	s.focus_use_ok = false; // otherwise Button would use OK style for focused button
	return s;
}

void AnimationSkin()
{
	ChStdSkin();
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
