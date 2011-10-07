#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	String pict = AsQTF(CreateImageObject(StreamRaster::LoadFileAny("d:/msg_about.gif"), 400));
	String qtf;
	qtf << "[A6 This is some drawing in QTF: " << pict << "&";
	qtf << "[C3*@B And now in the table: {{1:1 " << pict << ":: Another cell " << pict << "}}";
	PromptOK(qtf);
}
