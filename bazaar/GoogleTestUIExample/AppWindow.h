#ifndef _Test_AppWindow_h_
#define _Test_AppWindow_h_

#include <CtrlLib/CtrlLib.h>

namespace Upp
{
	class AppWindow final : public TopWindow
	{
	public:
		AppWindow();
	    
	private:
		void OnClick();
	
	private:
	    Button button;
	};
}

#endif
