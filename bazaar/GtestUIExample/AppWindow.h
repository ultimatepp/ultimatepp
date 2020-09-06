#ifndef _Test_AppWindow_h_
#define _Test_AppWindow_h_

#include <CtrlLib/CtrlLib.h>

namespace Upp
{
	class AppWindow : public TopWindow
	{
	public:
		Button button;
		
	public:
		AppWindow();
	
	private:
		virtual void OnButtonClick();
	};
}

#endif
