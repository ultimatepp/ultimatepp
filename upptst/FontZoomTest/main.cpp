#include <CtrlLib/CtrlLib.h>

using namespace Upp;

namespace Upp {
template<>
String AsString(const Zoom& m)
{
	return String().Cat() << m.m << '/' << m.d;
}
};

GUI_APP_MAIN
{
	PromptYesNoCancel("This is an example of prompt Yes No Cancel!");
	DUMP(GetRichTextStdScreenZoom());
	SetStdFont(SansSerif(8));
	PromptYesNoCancel("This is an example of prompt  Yes No Cancel after changing font!");
	DUMP(GetRichTextStdScreenZoom());
}
