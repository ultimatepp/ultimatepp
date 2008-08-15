#include <CtrlLib/CtrlLib.h>
#include <GLCtrl/GLCtrl.h>

using namespace Upp;

class GLTest : public GLCtrl
{
	virtual void GLPaint()
	{
		glClearColor(1.0, 1.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
	}
};

class GLControlTest : public TopWindow
{
	typedef GLControlTest CLASSNAME;

private:
	Splitter _splitter;
	GLTest _glTest;
	ImageCtrl _imgCtrl;
	MenuBar _menuBar;
	
	void Test()
	{
	}
	
	void MainMenu(Bar &bar)
	{
		bar.Add("Test", THISBACK(Test));
	}
	
public:
	GLControlTest()
	{
		SetRect(0, 0, 500, 400);
		AddFrame(_menuBar);
		_menuBar.Set(THISBACK(MainMenu));
		
		_imgCtrl.AddFrame(InsetFrame());
		_glTest.AddFrame(InsetFrame());
		
		_splitter.Horz(_imgCtrl, _glTest);
		*this << _splitter;
	}
};

GUI_APP_MAIN
{
	GLControlTest().Run();
}

