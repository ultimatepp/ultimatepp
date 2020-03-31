#ifndef _TextToSvgPath_TextToSvgPath_h
#define _TextToSvgPath_TextToSvgPath_h

#include <CtrlLib/CtrlLib.h>
#include <RichEdit/RichEdit.h>

using namespace Upp;

#define LAYOUTFILE <TextToSvgPath/TextToSvgPath.lay>
#include <CtrlCore/lay.h>

String TextToSvgPath(double x, double y, const char *text, Font fnt, bool singleline);

struct Preview : Ctrl {
	String svgpath;
	
	virtual void Paint(Draw& w);
};

struct TextToSvgPathDlg : public WithTextToSvgPathLayout<TopWindow> {
	Preview preview;
	
	TextToSvgPathDlg();
};

#endif
