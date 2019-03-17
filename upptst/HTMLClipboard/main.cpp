#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	String html =
	"<p style = \"margin:0px 0px 2px 0px;text-indent:0px;text-align:left;color:#be4e45;font-family:serif;font-size:24pt;font-weight:bold;font-style:normal;text-decoration:none;\">\
	Ultimate++ is a C++ cross-platform rapid application development framework</p>";
	
	WriteClipboardHTML( html);
	// then do paste in a word processor, libreoffice writer or winword.
}
