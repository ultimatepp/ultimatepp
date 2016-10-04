#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class Vera : public TopWindow {
public:
	virtual void Paint(Draw& draw);

	typedef Vera CLASSNAME;
};

void DrawIt(Draw& w)
{
	w.DrawRect(0, 0, 10000, 10000, White());
	w.DrawText(10, 10, "Vera::Paint", Font().FaceName("Bitstream Vera Sans Mono").Height(13));
	w.DrawText(10, 30, "Arial", Arial(18));
	w.DrawText(10, 50, "Roman", Roman(18));
}

void Vera::Paint(Draw& w)
{
	DrawIt(w);
	ImageDraw iw(200, 200);
	DrawIt(iw);
	w.DrawImage(100, 0, iw);
}

GUI_APP_MAIN
{
	Vera().Run();
}

