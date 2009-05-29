#ifndef _GoogleTranslatorDemo_GlobalShortCut_h_
#define _GoogleTranslatorDemo_GlobalShortCut_h_

#include <CtrlCore/CtrlCore.h>

using namespace Upp;

/*
class GlobalShortCut{
public:
	class KeyNode{
	public:
		Callback WhenKeyPress;	
		Callback WhenKeyRelease;	
	} 
	
	class KeyNodeRoot : public KeyNode{
	public:
		Callback WhenShortCut;	
	} 
	
	volatile Atomic process_key;
private:
	void ProcessGlobalShortCutEvent();
	Callback CallAction1;
	Callback CallAction2;
}
*/

// Need add in Ctrl Core and discuss about this
inline bool get_key_state(char k){
#ifdef PLATFORM_WIN32
			//... Here need to optimize
			return (((char)::GetKeyState(k)) & 128);
#elif PLATFORM_X11
			return false;
			//... how can i do it in X11?
			//return(sKbdState & k);
#else
			//ASSERT_(false, "Get key state is not support by this plathform")
			return false;
#endif

}

#endif
