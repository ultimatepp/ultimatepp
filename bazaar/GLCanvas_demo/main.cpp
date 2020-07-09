#include "GLCanvas_demo.h"

GLCanvas_demo::GLCanvas_demo()
{
	CtrlLayout(*this, "GLCanvas demo. STL viewer");
	
	butOpen.WhenAction = [&] {
		try {
			bool isText;
			String header;
			LoadStl(~filename, surf, isText, header);
			surf.GetLimits();
			canvas.SetEnv(surf.env);
		} catch (Exc e) {
			Exclamation(DeQtf(e));
		}
	};
	
	canvas.WhenPaint = [&] {
		canvas.PaintSurface(surf, Green(), true, false);
	};
	
	Sizeable().Zoomable();
}

GUI_APP_MAIN
{
	GLCanvas_demo().Run();
}
