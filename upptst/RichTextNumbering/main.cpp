#include <RichEdit/RichEdit.h>

using namespace Upp;

GUI_APP_MAIN
{
//	PromptOK("[NI; Number 1&][NIA; Number 1a&][NIa; Number 1b&][NIa; &][NI; Number 2&][N1; Last 3]");
	RichEditWithToolBar edit;
	TopWindow win;
	edit.SetQTF(
		"[ $$0,0#00000000000000000000000000000000:Default]\r\n[{_} \r\n[s0;NI; Number 1&]\r\n[s0;NIA; Number 1a&]\r\n[s0;NIa; Number 1b&]\r\n[ {{5000:5000 [s0;NIa; ]\r\n:: [s0;NIa; ssasdf]}}&]\r\n[s0;r3264;NIa; asdf&]\r\n[s0;r3264;NIa; asdf&]\r\n[s0;r3264;NIa; &]\r\n[s0; &]\r\n[s0; &]\r\n[s0; &]\r\n[s0;r3264;NIa; &]\r\n[s0;r3264;NIa; asdfasdfasdf&]\r\n[s0;r3264;NIa; &]\r\n[s0;r3264;NIa;%% asdfasdfasdf&]\r\n[s0;r3264;NIa; &]\r\n[s0;r3264;NI; Number 2&]\r\n[s0;r3264;N1; Last 3]]"
	);

	edit.WhenSel << [&] { win.Title(AsString(edit.GetCursor())); };
	win.Add(edit.SizePos());
	win.Run();
	
	RDUMP(AsCString(edit.GetQTF()));
}
