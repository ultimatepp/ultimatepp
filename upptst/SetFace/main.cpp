#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	int ndx = Font::FindFaceNameIndex("Arial");
	if(ndx >= 0) {
		Font::SetFace(Font::SERIF, "Arial", Font::GetFaceInfo(ndx));
	}
	PromptOK("[A5 Is this Arial?&[R Is this Arial?");
}
