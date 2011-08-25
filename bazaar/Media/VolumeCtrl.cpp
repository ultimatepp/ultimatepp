#include "VolumeCtrl.h"


SliderVolume::SliderVolume() {
	moving = false;
	Jump();
}

void SliderVolume::LeftDown(Point pos, dword keyflags) {
	SliderCtrl::LeftDown(pos, keyflags);
	moving = true;
}

void SliderVolume::LeftUp(Point pos, dword keyflags) {
	SliderCtrl::LeftUp(pos, keyflags);
	moving = false;
}

void SliderVolume::SetData(const Value& v) {
	Size sz = GetSize();
	bool horiz = sz.cx > sz.cy;
	
	if (horiz)
		SliderCtrl::SetData(v);	
	else
		SliderCtrl::SetData(GetMax() - int(v));
}

Value SliderVolume::GetData() const {
	Size sz = GetSize();
	bool horiz = sz.cx > sz.cy;
	
	Value val = SliderCtrl::GetData();
	if (horiz)
		return val;	
	else
		return GetMax() - int(val);
}

VolumeCtrl::VolumeCtrl() {
	Add(sliderL);
	Add(sliderR);
	sliderL.MinMax(0, MAX_MAINVOLUME);
	sliderR.MinMax(0, MAX_MAINVOLUME);
	sliderL.WhenAction = THISBACK(Action);
	sliderR.WhenAction = THISBACK(Action);
	Add(labelL); 
	Add(labelR);
	labelL.SetLabel("L");
	labelR.SetLabel("R");
	SetTimeCallback(-100, THISBACK(TimerAction));
	stereo = false;
	showLR = true;
	TimerAction();
}
	
void VolumeCtrl::Action() {
	if (stereo) {
		int left, right;
		if (!GetMainVolume(left, right))
			return;
		int valL = sliderL.GetData();
		int valR = sliderR.GetData();
		if (valL != left || valR != right)
			SetMainVolume(valL, valR);
	} else {
		int val = sliderL.GetData();
		if (val != GetMainVolume())
			SetMainVolume(val);
	}
}

void VolumeCtrl::TimerAction() {
	if(sliderL.IsMoving() && (sliderR.IsMoving() || !stereo))
		return;
	if (stereo) {
		int left, right;
		if (!GetMainVolume(left, right))
			return;
		if (left != sliderL.GetData())
			sliderL.SetData(left);
		if (right != sliderR.GetData())
			sliderR.SetData(right);
	} else {
		int val = GetMainVolume();
		if ((val != -1) && (val != sliderL.GetData()))
			sliderL.SetData(val);
	}
}

void VolumeCtrl::Layout() {
	Rect r = GetRect();
	Size sz = GetSize();
	
	if (stereo) {
		sliderR.Show();
		labelL.Show(showLR);
		labelR.Show(showLR);
		if (sz.cx < sz.cy) {
			Font f = Arial(sz.cx/4);
			Size fontSize = GetTextSize("X", f);
			if (showLR) 
				labelL.SetFont(f).BottomPosZ(0, fontSize.cy).LeftPosZ(int(0.15*sz.cx), 2*fontSize.cx);
			sliderL.VSizePosZ(0, (showLR ? fontSize.cy : 0)).LeftPosZ(0, sz.cx/2);
			if (showLR) 
				labelR.SetFont(f).BottomPosZ(0, fontSize.cy).LeftPosZ(int(0.65*sz.cx), 2*fontSize.cx);
			sliderR.VSizePosZ(0, (showLR ? fontSize.cy : 0)).LeftPosZ(sz.cx/2, sz.cx/2);			
		} else {
			Font f = Arial(sz.cy/4);
			Size fontSize = GetTextSize("X", f);
			if (showLR) 
				labelL.SetFont(f).HSizePosZ(0, 0).TopPosZ(int(0.1*sz.cy), fontSize.cy);
			sliderL.HSizePosZ((showLR ? int(1.5*fontSize.cx) : 0), 0).TopPosZ(0, sz.cy/2);
			if (showLR) 
				labelR.SetFont(f).HSizePosZ(0, 0).TopPosZ(int(0.6*sz.cy), fontSize.cy);			
			sliderR.HSizePosZ((showLR ? int(1.5*fontSize.cx) : 0), 0).TopPosZ(sz.cy/2, sz.cy/2);
		} 
	} else {
		sliderL.SizePos();
		sliderR.Hide();
		labelL.Hide();
		labelR.Hide();
	}
}
