#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	if(AcceptFiles(Ctrl::Clipboard())) {
		Vector<String> f = GetFiles(Ctrl::Clipboard());
		for(int i = 0; i < f.GetCount(); i++)
			LOG("#include \"lib/" + GetFileName(f[i]) + "\"");
	}
}

