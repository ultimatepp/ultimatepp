#ifndef _SkinSensitive_SkinSensitive_h
#define _SkinSensitive_SkinSensitive_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <SkinSensitive/SkinSensitive.lay>
#include <CtrlCore/lay.h>

class SkinSensitive : public WithSkinSensitiveLayout<TopWindow> {
public:
	SkinSensitive();
};

#endif
