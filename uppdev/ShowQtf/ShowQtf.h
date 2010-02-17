#ifndef _ShowQtf_ShowQtf_h
#define _ShowQtf_ShowQtf_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class ShowQtf : public TopWindow {
	DocEdit      text;
	RichTextCtrl qtf;
	Splitter     split;
	
	void Text();
	
public:
	typedef ShowQtf CLASSNAME;
	ShowQtf();
};

#endif

