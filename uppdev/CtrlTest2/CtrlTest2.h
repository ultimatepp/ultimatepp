#ifndef _CtrlTest2_CtrlTest2_h
#define _CtrlTest2_CtrlTest2_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <CtrlTest2/CtrlTest2.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS MyImg
#define IMAGEFILE <CtrlTest2/CtrlTest2.iml>
#include <Draw/iml_header.h>

class CtrlTest2 : public WithCtrlTest2Layout<TopWindow> {
public:
	typedef CtrlTest2 CLASSNAME;
	CtrlTest2();
};

#endif

