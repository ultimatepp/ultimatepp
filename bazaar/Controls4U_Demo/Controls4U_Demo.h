#ifndef _Controls4U_Demo_Controls4U_Demo_h
#define _Controls4U_Demo_Controls4U_Demo_h

#define LAYOUTFILE <Controls4U_Demo/Controls4U_Demo.lay>
#include <CtrlCore/lay.h>

class EditFileFolder_Demo : public WithEditFileFolder<StaticRect> {
public:
	typedef EditFileFolder_Demo CLASSNAME;
	EditFileFolder_Demo();
	void OnNewFile();
	void ChangeProperties();
};

class StaticCtrls_Demo : public WithStaticCtrls<StaticRect> {
public:
	typedef StaticCtrls_Demo CLASSNAME;
	StaticCtrls_Demo();
};

class StaticCtrlsTest_Demo : public WithStaticCtrlsTest<StaticRect> {
public:
	typedef StaticCtrlsTest_Demo CLASSNAME;
	StaticCtrlsTest_Demo();
};

class StaticClock_Demo : public WithStaticClock<StaticRect> {
public:
	typedef StaticClock_Demo CLASSNAME;
	StaticClock_Demo();
	void UpdateInfo();
	void ChangeProperties();	
};

class Meter_Demo : public WithMeter<StaticRect> {
public:
	typedef Meter_Demo CLASSNAME;
	Meter_Demo();
	void ChangeValueKnob(Knob *knob, Meter *meter);
	void ChangeProperties();
	void ChangePropertiesKnob();
};

class FileBrowser_Demo : public WithFileBrowser<StaticRect> {
public:
	typedef FileBrowser_Demo CLASSNAME;
	FileBrowser_Demo();
	
	void FileSelected();
	void FileOpened();
	void ChangeProperties();
};

class Functions4U_Demo : public WithFunctions4U<StaticRect> {
public:
	typedef Functions4U_Demo CLASSNAME;
	Functions4U_Demo();
	void OnDiff();
	void OnPatch();
	void OnSet();
};

class PainterCanvas_Demo : public WithPainterCanvas<StaticRect> {
public:
	typedef PainterCanvas_Demo CLASSNAME;
	PainterCanvas_Demo();
};

class StaticImageSet_Demo : public WithStaticImageSet<StaticRect> {
public:
	typedef StaticImageSet_Demo CLASSNAME;
	StaticImageSet_Demo();
};

class Controls4U_Demo : public WithMain<TopWindow> {
public:
	typedef Controls4U_Demo CLASSNAME;
	Controls4U_Demo();
	
	EditFileFolder_Demo editFileFolder_Demo;
	StaticCtrls_Demo staticCtrls_Demo;
	StaticCtrlsTest_Demo staticCtrlsTest_Demo;
	StaticClock_Demo staticClock_Demo;
	Meter_Demo meter_Demo;
	JBControlsDemo jbcontrols_Demo;
	FileBrowser_Demo fileBrowser_Demo;
	Functions4U_Demo functions4U_Demo;
	PainterCanvas_Demo painterCanvas_Demo;
	StaticImageSet_Demo staticImageSet_Demo;
	AboutUpp aboutDlg;
#if defined(PLATFORM_WIN32) 
	Firefox_Demo firefox_Demo;
	IExplorer_Demo iexplorer_Demo;
	VLC_Demo vlc_Demo;
#endif
	int timerOn;
	void Timer();
	
private:
	Vector<StaticRect *> controls;
	void OnGridSel();;
};

#endif

