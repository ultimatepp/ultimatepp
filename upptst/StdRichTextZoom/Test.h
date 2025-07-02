#ifndef _Test_Test_h
#define _Test_Test_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <StdRichTextZoom/Test.lay>
#include <CtrlCore/lay.h>

class Test : public WithTestLayout<TopWindow> {
public:
	Test();
};

#endif
