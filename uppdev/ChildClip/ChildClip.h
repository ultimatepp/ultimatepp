#ifndef _ChildClip_ChildClip_h
#define _ChildClip_ChildClip_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <ChildClip/ChildClip.lay>
#include <CtrlCore/lay.h>



class ChildClip : public WithChildClipLayout<TopWindow> {
public:
	typedef ChildClip CLASSNAME;
	ChildClip();
};

#endif
