#include "Transparent.h"



App::App()
{
	CtrlLayout(*this);

	list.HeaderObject().Absolute().Track();

	const int fx = 10;
	const int fy = 50;

	for (int i = 0; i < fx; i++)
		list.AddColumn("Column " + AsString(i), 100).HeaderTab().Min(50);

    for (int i = 0; i < fx; i++)
        for (int j = 0; j < fy; j++)
            list.Set(j, i, "Value " + AsString(j) + ":" + AsString(i));

	Sizeable().Zoomable();

//	static PaintRect pr(ColorDisplay(), White());
//	Background(pr);
}

GUI_APP_MAIN
{
	App().Run();
}
