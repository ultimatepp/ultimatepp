#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <CtrlRetriever/CtrlRetriever.lay>
#include <CtrlCore/lay.h>

GUI_APP_MAIN
{
	String text = "Some text";
	bool   option = true;

	WithCtrlRetrieverLayout<TopWindow> dlg;
	CtrlLayoutOK(dlg, "CtrlRetriever example");
	CtrlRetriever r;
	r
		(dlg.option, option)
		(dlg.text, text)
	;

	dlg.Run();
	r.Retrieve();

	PromptOK(String().Cat() << "text: [* " << DeQtf(text) << "]&"
	                        << "option: [* " << (option ? "true" : "false"));
}
