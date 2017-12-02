#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define TOPICFILE <Topic/app.tpp/all.i>
#include <Core/topic_group.h>

GUI_APP_MAIN
{
	HelpWindow help;
	help.GoTo("topic://Topic/app/main_en-us");
	help.Execute();
}
