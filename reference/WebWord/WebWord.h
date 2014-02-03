#ifndef _WebWord_WebWord_h_
#define _WebWord_WebWord_h_

#include <RichEdit/RichEdit.h>
#include <PdfDraw/PdfDraw.h>

using namespace Upp;

struct WebWord : public TopWindow {
public:
	RichEditWithToolBar   editor;
	StatusBar  statusbar;
	
	void ShowInfo();

public:
	typedef WebWord CLASSNAME;

	WebWord();
};

#endif
