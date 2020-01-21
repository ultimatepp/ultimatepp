#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Chameleon/Chameleon.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS MyButtonImg
#define IMAGEFILE <Chameleon/Chameleon.iml>
#include <Draw/iml.h>

struct ChEllipse {
	int   width;
	Color pen;
	Color color;

	ChEllipse() {}
};

Value EllipseLook(int width, Color pen, Color color)
{
	ChEllipse e;
	e.width = width;
	e.pen = pen;
	e.color = color;
	return RawToValue(e);
}

INITBLOCK {
	ChLookFn(
		[](Draw& w, const Rect& rect, const Value& v, int op, Color) -> Value {
			if(IsTypeRaw<ChEllipse>(v)) {
				const ChEllipse& e = ValueTo<ChEllipse>(v);
				switch(op) {
				case LOOK_MARGINS:
					return Rect(e.width, e.width, e.width, e.width);
				case LOOK_PAINT:
					w.DrawEllipse(rect.Deflated(e.width), e.color, e.width, e.pen);
					return 1;
				}
			}
			return Null;
		}
	);
}

Button::Style ColorStyle()
{
	Button::Style s = Button::StyleNormal();
	s.look[0] = LtCyan();
	s.look[1] = Yellow();
	s.look[2] = Brown();
	s.look[3] = Gray();
	s.focus_use_ok = false; // otherwise Button would use OK style for focused button
	return s;
}

Button::Style ImageStyle()
{
	Button::Style s = Button::StyleNormal();
	for(int i = 0; i < 4; i++)
		s.look[i] = MyButtonImg::Get(i);
	s.pressoffset = Point(1, -1);
	s.focus_use_ok = false; // otherwise Button would use OK style for focused button
	return s;
}

Button::Style FnStyle()
{
	Button::Style s = Button::StyleNormal().Write();
	s.look[0] = EllipseLook(2, Black, LtGray);
	s.look[1] = EllipseLook(3, Red, WhiteGray);
	s.look[2] = EllipseLook(4, Blue, White);
	s.look[3] = EllipseLook(1, Black, Gray);
	s.focus_use_ok = false; // otherwise Button would use OK style for focused button
	return s;
}

void ColorSkin()
{
	Button::StyleNormal().Write() = ColorStyle();
}

void ImageSkin()
{
	Button::StyleNormal().Write() = ImageStyle();
}

void FnSkin()
{
	Button::StyleNormal().Write() = FnStyle();
}

struct MyApp : WithChameleonLayout<TopWindow> {
	void Set(void (*skin)());
	Button::Style scolor, simage, sfn;

	MyApp();
};

void MyApp::Set(void (*skin)())
{
	SetSkin(skin);
}

MyApp::MyApp()
{
	CtrlLayout(*this, "Chameleon example");
	host << [=] { Set(ChHostSkin); };
	std << [=] { Set(ChStdSkin); };
	classic << [=] { Set(ChClassicSkin); };
	color << [=] { Set(ColorSkin); };
	scolor = ColorStyle();
	color.SetStyle(scolor);
	img << [=] { Set(ImageSkin); };
	simage = ImageStyle();
	img.SetStyle(simage);
	fn << [=] { Set(FnSkin); };
	sfn = FnStyle();
	fn.SetStyle(sfn);
}

GUI_APP_MAIN
{
	MyApp().Run();
}
