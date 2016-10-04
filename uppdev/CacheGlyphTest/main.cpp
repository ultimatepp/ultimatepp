#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

struct Test : Painter {
	Pointf current;

	virtual void   ClearOp(const RGBA& color) {}

	virtual void   MoveOp(const Pointf& p, bool rel);
	virtual void   LineOp(const Pointf& p, bool rel);
	virtual void   QuadraticOp(const Pointf& p1, const Pointf& p, bool rel);
	virtual void   QuadraticOp(const Pointf& p, bool rel) {}
	virtual void   CubicOp(const Pointf& p1, const Pointf& p2, const Pointf& p, bool rel) {}
	virtual void   CubicOp(const Pointf& p2, const Pointf& p, bool rel) {}
	virtual void   ArcOp(const Pointf& c, const Pointf& r, double angle, double sweep, bool rel) {}
	virtual void   SvgArcOp(const Pointf& r, double xangle, bool large, bool sweep,
	                        const Pointf& p, bool rel) {}
	virtual void   CloseOp() {}
	virtual void   DivOp() {}

	virtual void   FillOp(const RGBA& color) {}
	virtual void   FillOp(const Image& image, const Xform2D& transsrc, dword flags) {}
	virtual void   FillOp(const Pointf& p1, const RGBA& color1,
	                      const Pointf& p2, const RGBA& color2,
	                      int style) {}
	virtual void   FillOp(const Pointf& f, const RGBA& color1, 
	                      const Pointf& c, double r, const RGBA& color2,
	                      int style) {}

	virtual void   StrokeOp(double width, const RGBA& rgba) {}
	virtual void   StrokeOp(double width, const Image& image, const Xform2D& transsrc,
	                        dword flags) {}
	virtual void   StrokeOp(double width, const Pointf& p1, const RGBA& color1,
	                        const Pointf& p2, const RGBA& color2,
	                        int style) {}
	virtual void   StrokeOp(double width, const Pointf& f, const RGBA& color1, 
	                        const Pointf& c, double r, const RGBA& color2,
	                        int style) {}

	virtual void   ClipOp() {}


	virtual void   ColorStopOp(double pos, const RGBA& color) {}
	virtual void   ClearStopsOp() {}
	
	virtual void   OpacityOp(double o) {}
	virtual void   LineCapOp(int linecap) {}
	virtual void   LineJoinOp(int linejoin) {}
	virtual void   MiterLimitOp(double l) {}
	virtual void   EvenOddOp(bool evenodd) {}
	virtual void   DashOp(const Vector<double>& dash, double start) {}

	virtual void   TransformOp(const Xform2D& m) {}

	virtual void   BeginOp() {}
	virtual void   EndOp() {}

	virtual void   BeginMaskOp() {}
};

int quad;
int move;
int line;
int aline;
int amove;

struct TestConsumer : LinearPathConsumer {
	virtual void Line(const Pointf& p) { LOG("Line " << p); aline++; }
	virtual void Move(const Pointf& p) { LOG("Move " << p); amove++; }
};

void Test::MoveOp(const Pointf& p, bool rel)
{
	LOG("MoveOp " << p);
	move++;
	current = p;
}

void Test::LineOp(const Pointf& p, bool rel)
{
	LOG("LineOp " << p);
	line++;
	current = p;
}

void Test::QuadraticOp(const Pointf& p1, const Pointf& p, bool rel)
{
	LOG("QuadraticOp " << p1 << ' ' << p);
	quad++;
	ApproximateQuadratic(TestConsumer(), current, p1, p, 0.3);
	current = p;
}

GUI_APP_MAIN
{
	Test test;
	test.Character(0, 0, 'Q', Roman(20).Italic());
	DDUMP(quad);
	DDUMP(move);
	DDUMP(line);
	DDUMP(aline);
	DDUMP(amove);
}
