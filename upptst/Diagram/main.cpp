#include <RichEdit/RichEdit.h>

using namespace Upp;

GUI_APP_MAIN
{
/*
	int cp;
	Font fnt;
	SelectUnicodeSymbol(cp, fnt);
	return;
//*/	
	TopWindow app;
	app.Sizeable().Zoomable();
	DiagramEditor de;
	de.AllowDarkContent();
	app.Add(de.SizePos());
	String fn = GetHomeDirFile("test.qdf");
	de.Load(LoadFile(fn));
	app.Run();
	SaveFile(fn, de.Save());
}
