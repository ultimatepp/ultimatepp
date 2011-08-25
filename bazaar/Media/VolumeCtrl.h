#ifndef _Media_VolumeCtrl_h_
#define _Media_VolumeCtrl_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include "audioSystem.h"


class SliderVolume : public SliderCtrl {
typedef SliderVolume CLASSNAME;		
public:
	SliderVolume();
	bool IsMoving()		{return moving;};
	virtual void SetData(const Value& v);
	virtual Value GetData() const;
	
private:
	bool moving;
	
	void LeftDown(Point pos, dword keyflags);
	void LeftUp(Point pos, dword keyflags);
};

class VolumeCtrl : public StaticRect {
typedef VolumeCtrl CLASSNAME;	
public:
	VolumeCtrl();
	VolumeCtrl &SetStereo(bool _stereo = false)	{stereo = _stereo;	return *this;};
	VolumeCtrl &ShowLR(bool _showLR = true)		{showLR = _showLR;	return *this;};
	
private:
	bool stereo;
	bool showLR;
	
	SliderVolume sliderL, sliderR;
	Label labelL, labelR;
	
	void Action();
	void TimerAction();

	virtual void Layout();
};

#endif
