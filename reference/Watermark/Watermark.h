#ifndef _Watermark_Watermark_h
#define _Watermark_Watermark_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Watermark/Watermark.lay>
#include <CtrlCore/lay.h>

class Watermark : public WithWatermarkLayout<TopWindow> {
public:
	typedef Watermark CLASSNAME;
	Watermark();
};

#endif
