#include <CtrlLib/CtrlLib.h>
#include <GLCtrl/GLCtrl.h>

using namespace Upp;

#define LAYOUTFILE <GLCtrlBug/GLCtrlBug.lay>
#include <CtrlCore/lay.h>

class GLCtrlBug : public WithGLCtrlBugLayout<TopWindow> 
{
public:
	typedef GLCtrlBug CLASSNAME;
	GLCtrlBug();
	void test();
};

GLCtrlBug::GLCtrlBug()
{
	CtrlLayout(*this, "Window title");
	_button <<= THISBACK(test);
}

void GLCtrlBug::test()
{
	if(_glCtrl.GetParent())
		RemoveChild(&_glCtrl);
	else
		Add(_glCtrl);
}

GUI_APP_MAIN
{
	GLCtrlBug().Run();
}


