#ifndef _StatusBarTest_StatusBarTest_h
#define _StatusBarTest_StatusBarTest_h

#include <CtrlLib/CtrlLib.h>

#define IMAGECLASS IconImages
#define IMAGEFILE  <StatusBarTest/IconImages.iml>
#include <Draw/iml.h>

class StatusBarTest : public TopWindow {
public:
	typedef StatusBarTest CLASSNAME;
	StatusBarTest();
private:
	StatusBar m_sb;
};

#endif
