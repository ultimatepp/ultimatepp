#include "GLCanvas_demo.h"

GLCanvas_demo::GLCanvas_demo()
{
	CtrlLayout(*this, "GLCanvas demo. STL viewer");
	
	opShowAxis.WhenAction = [&] {canvas.Refresh();};
	opShowNormals.WhenAction = [&] {canvas.Refresh();};
	
	butOpen.WhenAction = [&] {
		try {
			WaitCursor waitcursor;
			
			bool isText;
			String header;
			LoadStl(~filename, surf, isText, header);
			surf.GetPanelParams();
			surf.GetLimits();
			surf.GetSurface();
			surf.GetVolume();
			canvas.SetEnv(surf.env);
		} catch (Exc e) {
			Exclamation(DeQtf(e));
		}
	};
	
	canvas.WhenPaint = [&] {
		if (opShowAxis)
			canvas.PaintAxis(0, 0, 0, surf.env.LenRef()/4.);
		canvas.PaintSurface(surf, Green(), true, ~opShowNormals);
	};
	
	Sizeable().Zoomable();
}

GUI_APP_MAIN
{
	GLCanvas_demo().Run();
}
