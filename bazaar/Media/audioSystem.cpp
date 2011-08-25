#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <Functions4U/Functions4U.h>
#include "audioSystem.h"

int GetMainVolume() {
	int left, right;
	
	if(!GetMainVolume(left, right))
		return -1;
	return Average(left, right);
}

bool SetMainVolume(int vol) {
	return SetMainVolume(vol, vol);
}

#ifdef PLATFORM_POSIX
//#include <fcntl.h> 
#include <linux/soundcard.h> 
//#include <stdio.h>
//#include <stdlib.h>
#include <sys/ioctl.h>

// See more details here

bool GetMainVolume(int &left, int &right) {
   	int fAudio = open("/dev/mixer", O_RDWR, 0);
   	if (fAudio == -1)
   		return false;
   	int vol;
   	bool ret;
   	if(ioctl(fAudio, MIXER_READ(SOUND_MIXER_VOLUME), &vol) == -1) 
      	ret = false;
  	else
  		ret = true;
  	close(fAudio);
  	
  	// Linux levels are in percentage
  	if (ret) {
  		left = (MAX_MAINVOLUME/100)*(vol & 0x7f);
  		right = (MAX_MAINVOLUME/100)*((vol >> 8) & 0x7f);
  	} else
  		left = right = -1;
   	return ret;
}

bool SetMainVolume(int left, int right) {
   	int vol = ((right/(MAX_MAINVOLUME/100)) << 8) | (left/(MAX_MAINVOLUME/100));
   	int fAudio = open("/dev/mixer", O_RDWR, 0);
   	if (fAudio == -1)
   		return -1;
   	bool ret;
   	if(ioctl(fAudio, MIXER_WRITE(SOUND_MIXER_VOLUME), &vol) == -1) 
      	ret = false;
  	else
  		ret = true;
  	close(fAudio);
   	return ret;
}

#else

#include <mmsystem.h> 

// See more details here http://echochamber.me/viewtopic.php?f=11&t=15858

bool GetVolumeCtrl(HMIXER &hMixer, MIXERCONTROL &mc) {
	MMRESULT result;
	
	if (MMSYSERR_NOERROR != (result = mixerOpen(&hMixer, MIXER_OBJECTF_MIXER, 0, 0, 0)))
		return false;

	// Get the speaker line of the mixer device.
	MIXERLINE ml = {0};
	ml.cbStruct = sizeof(MIXERLINE);
	ml.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	result = mixerGetLineInfo((HMIXEROBJ) hMixer, &ml, MIXER_GETLINEINFOF_COMPONENTTYPE);
	if (result != MMSYSERR_NOERROR) {
		mixerClose(hMixer);
		return false;														
	}
	
	// Get the volume control of the speaker line.
	MIXERLINECONTROLS mlc = {0};
	mlc.cbStruct = sizeof(MIXERLINECONTROLS);
	mlc.dwLineID = ml.dwLineID;
	mlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
	mlc.cControls = 1;
	mlc.pamxctrl = &mc;
	mlc.cbmxctrl = sizeof(MIXERCONTROL);
	result = mixerGetLineControls((HMIXEROBJ) hMixer, &mlc, MIXER_GETLINECONTROLSF_ONEBYTYPE);
	if (result != MMSYSERR_NOERROR) {
		mixerClose(hMixer);
		return false;														
	}	
	return true;
}

bool SetMainVolume(int left, int right) {
	HMIXER hMixer;
	MIXERCONTROL mc = {0};
	
	GetVolumeCtrl(hMixer, mc);
	
	MIXERCONTROLDETAILS mcd = {0};
	MIXERCONTROLDETAILS_UNSIGNED mcdVol[2] = {0};
	double range = mc.Bounds.dwMaximum - mc.Bounds.dwMinimum;
	DWORD dwLeft  = DWORD(left *range/MAX_MAINVOLUME);
    DWORD dwRight = DWORD(right*range/MAX_MAINVOLUME);
    
	memcpy(&mcdVol[0], &dwLeft, sizeof(MIXERCONTROLDETAILS_UNSIGNED));
    memcpy(&mcdVol[1], &dwRight, sizeof(MIXERCONTROLDETAILS_UNSIGNED));

	mcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mcd.hwndOwner = 0;
	mcd.dwControlID = mc.dwControlID;
	mcd.paDetails = &mcdVol;
	mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	mcd.cChannels = 2;
	MMRESULT result = mixerSetControlDetails((HMIXEROBJ) hMixer, &mcd, 
										MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE);
	mixerClose(hMixer);
	 
	return result == MMSYSERR_NOERROR;
}

bool GetMainVolume(int &left, int &right) {
	HMIXER hMixer;
	MIXERCONTROL mc = {0};
	
	GetVolumeCtrl(hMixer, mc);
	
	MIXERCONTROLDETAILS mcd = {0};
	MIXERCONTROLDETAILS_UNSIGNED mcdVol[2] = {0};

	mcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mcd.hwndOwner = 0;
	mcd.dwControlID = mc.dwControlID;
	mcd.paDetails = &mcdVol;
	mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	mcd.cChannels = 2;
	MMRESULT result = mixerGetControlDetails((HMIXEROBJ) hMixer, &mcd, 
										MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE);
	
	if (result == MMSYSERR_NOERROR) {
		double range = mc.Bounds.dwMaximum - mc.Bounds.dwMinimum;
		left  = int((mcdVol[0].dwValue - mc.Bounds.dwMinimum)*MAX_MAINVOLUME/range);
	    right = int((mcdVol[1].dwValue - mc.Bounds.dwMinimum)*MAX_MAINVOLUME/range);
	} else
		left = right = -1;
	
	mixerClose(hMixer);
	
	return result == MMSYSERR_NOERROR;
}

const char* GetAudioErrorDescription(MMRESULT code) {
 	switch(code) {
	case MMSYSERR_ERROR: 		return "Unspecified Error";
	case MMSYSERR_BADDEVICEID: 	return "Device ID out of range";
	case MMSYSERR_NOTENABLED: 	return "Driver failed enable";
	case MMSYSERR_ALLOCATED: 	return "Device already allocated";
	case MMSYSERR_INVALHANDLE: 	return "Device handle is invalid";
	case MMSYSERR_NODRIVER: 	return "No device driver present";
	case MMSYSERR_NOMEM: 		return "Memory allocation error";
	case MMSYSERR_NOTSUPPORTED: return "Function isn't supported";
	case MMSYSERR_BADERRNUM: 	return "Error value out of range";
	case MMSYSERR_INVALFLAG: 	return "Invalid flag passed";
	case MMSYSERR_INVALPARAM: 	return "Invalid parameter passed";
	case MMSYSERR_LASTERROR: 	return "Last error in range";
	default:					return "Unknown error";
   }
}

#endif