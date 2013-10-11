#ifndef _Controls4U_Demo_Controls4U_Demo_win_h_
#define _Controls4U_Demo_Controls4U_Demo_win_h_

#define LAYOUTFILE <Controls4U_Demo/Controls4U_Demo_win.lay>
#include <CtrlCore/lay.h>

class Firefox_Demo : public WithFirefox<StaticRect> {
public:
	typedef Firefox_Demo CLASSNAME;
	Firefox_Demo();

	void Browse();
	void ShowHTML();
	void Forward();
	void Backward();
	void UpdateInfo();
	void Home();
	void Stop();
	void RefreshPage();
	void SwitchOn();	
};

class IExplorer_Demo : public WithIExplorer<StaticRect> {
public:
	typedef IExplorer_Demo CLASSNAME;
	IExplorer_Demo();
	
	void Browse();
	void ShowHTML();
	void Forward();
	void Backward();
	void UpdateInfo();
	void Home();
	void Stop();
	void RefreshPage();
	void SwitchOn();
};

class VLC_Demo : public WithVLC<StaticRect> {
public:
	typedef VLC_Demo CLASSNAME;
	VLC_Demo();
	
	void Load();
	void Play();
	void Pause();
	void Stop();
	void UpdateInfo();
	void SwitchOn();
};

#endif
