#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow w;
	EditString a, b;
	w << a << b;
	a <<= "Hello ";
	b <<= "world!";

	String h;
	for(Ctrl& q : w)
		h << ~q;
	
	DUMP(h);
}
