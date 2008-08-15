#ifndef _RichView_RichView_h
#define _RichView_RichView_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <RichView/RichView.lay>
#include <CtrlCore/lay.h>

class RichView : public WithRichViewLayout<TopWindow> {
public:
	void Add();
	void Clear();

	typedef RichView CLASSNAME;
	RichView();
};

#endif
