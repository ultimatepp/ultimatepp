#ifndef _PullText_PullText_h
#define _PullText_PullText_h

#include <CtrlLib/CtrlLib.h>
#include <RichEdit/RichEdit.h>

using namespace Upp;

#define LAYOUTFILE <PullText/PullText.lay>
#include <CtrlCore/lay.h>



class PullText : public WithPullTextLayout<TopWindow> {
public:
	typedef PullText CLASSNAME;
	PullText();
	
	void OnButton1(void);
	void OnButton2(void);
};

#endif

