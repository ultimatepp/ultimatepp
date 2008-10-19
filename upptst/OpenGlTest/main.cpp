#include <CtrlLib/CtrlLib.h>
#include <GLCtrl/GLCtrl.h>
using namespace Upp;

class MyGlCtrl : public GLCtrl
{
	typedef MyGlCtrl CLASSNAME;
	
private:
	LineEdit *_log;
	ToolBar _sliderBar;
	SliderCtrl _slider;
	
public:
	MyGlCtrl()
		: _log(NULL)
	{
		_slider.Range(9);
		_slider.SetData(5);
		_slider <<= THISBACK(SliderCtrlAction);
		
		_sliderBar.Set(THISBACK(InitSliderBar));
		_sliderBar.Bottom();
		AddFrame(_sliderBar);
	}
	
	void SetLog(LineEdit &log)
	{
		_log = &log;
	}
	
	void Log(String const &s) { if(_log == NULL) return; _log->Insert(_log->GetLength(), String().Cat() << "\n" << s); _log->ScrollDown(); }
	
protected:
	virtual void GLInit()
	{
		ASSERT(HasRenderingContext());
		
		Log("-> GLInit()");
		
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glColor3f(0.0, 0.0, 1.0);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
	}
	
	virtual void GLDone()
	{
		ASSERT(HasRenderingContext());
		
		Log("-> GLDone()");
	}
	
	virtual void GLResize(int w, int h)
	{
		ASSERT(HasRenderingContext());
		
		Log(String().Cat() << "-> GLResize(" << w << ", " << h << ")");
	}
	
	virtual void GLPaint()
	{
		ASSERT(HasRenderingContext());
		
		Log("-> GLPaint()");
		
		glClear(GL_COLOR_BUFFER_BIT);
		
		for (int i = 0; i < (int)_slider.GetData(); i++)
		{
			glRectf(-9.0 + i * 2, 5.0, -8.0 + i * 2, -5.0);
		}
	}
	
private:
	void InitSliderBar(Bar &bar)
	{
		bar.Add(_slider, INT_MAX);
	}
	
	void SliderCtrlAction()
	{
		Log(String().Cat() << "-> SliderCtrlAction(" << _slider.GetData() << ")");
		Refresh();
	}
	
	bool HasRenderingContext()
	{
	#ifdef PLATFORM_WIN32
		return wglGetCurrentContext() != NULL;
	#else
		return glXGetCurrentContext() != NULL;
	#endif
	}
};

#define LAYOUTFILE <OpenGlTest/OpenGlTest.lay>
#include <CtrlCore/lay.h>

class OpenGlTest : public WithOpenGlTestLayout<TopWindow> 
{
	typedef OpenGlTest CLASSNAME;
	
public:
	OpenGlTest()
	{
		_log.Insert(_log.GetLength(), "\nStart of OpenGlTest constructor");
		
		_hideButton <<= THISBACK(Hide);
		_showButton <<= THISBACK(Show);
		_addButton <<= THISBACK(Add);
		_removeButton <<= THISBACK(Remove);
		
		_showButton.Enable(false);
		_addButton.Enable(false);
		
		_glCtrl.SetLog(_log);
		CtrlLayout(*this, "OpenGL test");
		
		_log.Insert(_log.GetLength(), "\nEnd of OpenGlTest constructor");
	}
	
private:
	void Hide()
	{
		_glCtrl.Hide();
		_glCtrl.Log("-> Hide()");
		_hideButton.Enable(false);
		_showButton.Enable(true);
	}
	
	void Show()
	{
		_glCtrl.Show();
		_glCtrl.Log("-> Show()");
		_hideButton.Enable(true);
		_showButton.Enable(false);
	}
	
	void Remove()
	{
		RemoveChild(&_glCtrl);
		_glCtrl.Log("-> Remove()");
		_removeButton.Enable(false);
		_addButton.Enable(true);
	}
	
	void Add()
	{
		AddChild(&_glCtrl);
		_glCtrl.Log("-> Add()");
		_removeButton.Enable(true);
		_addButton.Enable(false);
	}
};

GUI_APP_MAIN
{
	OpenGlTest().Sizeable().Run();
}

