#include "Color.h"

ColorWindow::ColorWindow()
{
	Sizeable().Zoomable();
}

void ColorWindow::Paint(Draw& draw)
{
	draw.DrawRect(GetSize(), White());
	for(int i = 0; i < 16; i++) {
		Color c(i & 1 ? 255 : 0, i & 2 ? 255 : 0, i & 4 ? 255 : 0);
		draw.DrawText(0, i * 30, AsString(c), Arial(30).Underline(), c);
	}
}

GUI_APP_MAIN
{
	ColorWindow().Run();
}
