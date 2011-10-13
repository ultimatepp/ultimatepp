#include <CtrlLib/CtrlLib.h>

#include <HelpViewer/HelpViewer.h>

using namespace Upp;

#define TOPICFILE <TestHelpViewer/app.tpp/all.i>
#include <Core/topic_group.h>

GUI_APP_MAIN
{
	HelpViewer helpViewer;
	
	helpViewer.Title("HelpViewer test");
	helpViewer.Clear();
	helpViewer.LoadTOC("topic://TestHelpViewer/app/TOC$it-it");
	helpViewer.Execute();
}

