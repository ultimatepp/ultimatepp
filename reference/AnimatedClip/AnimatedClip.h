#ifndef _AnimatedClipDemo_AnimatedClipDemo_h
#define _AnimatedClipDemo_AnimatedClipDemo_h

#define LAYOUTFILE <AnimatedClip/AnimatedClip.lay>
#include <CtrlCore/lay.h>

class AnimatedClip : public WithAnimatedClipLayout<TopWindow> {
public:
	typedef AnimatedClip CLASSNAME;
	AnimatedClip();
	
	void Browse();
	void Play();
	void Stop();	
	void OpenNext();
	void ChangeThreads();
	
	void OnShown();
};

#endif

