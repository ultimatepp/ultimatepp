#ifdef _WIN32

#include <CtrlLib/CtrlLib.h>
#include "ActiveX.h"

NAMESPACE_UPP

bool VLCPlayer::AddTarget(const String movie) {
	if (IsPlaying())
		return false;
	Detach();
	Attach(GetHWND());
	{
		IIVLC vlc(this);
		if (!vlc) 
			return false;
		
		VARIANT options;
		VariantInit(&options);
		VariantClear(&options);
	
		CBSTR bsmovie;
		if(!bsmovie.Set(movie))
			return false;
		
		if (S_OK != vlc->addTarget(bsmovie, options, VLCPlayListAppendAndGo, 0)) 
			return false;
	}
	return true;
}


bool VLCPlayer::Play() {
	IIVLC vlc(this);
	if (!vlc) 
		return false;
	
	if (S_OK != vlc->play()) 
		return false;
	return true;
}

bool VLCPlayer::Pause() {
	IIVLC vlc(this);
	if (!vlc) 
		return false;
	
	if (S_OK != vlc->pause()) 
		return false;
	return true;
}

bool VLCPlayer::PlayFaster() {
	IIVLC vlc(this);
	if (!vlc) 
		return false;
	
	if (S_OK != vlc->playFaster()) 
		return false;
	return true;
}

bool VLCPlayer::PlaySlower() {
	IIVLC vlc(this);
	if (!vlc) 
		return false;
	
	if (S_OK != vlc->playSlower()) 
		return false;
	return true;
}

bool VLCPlayer::Stop() {
	IIVLC vlc(this);
	if (!vlc) 
		return false;
	
	if (S_OK != vlc->stop()) 
		return false;
	return true;
}

bool VLCPlayer::Shuttle(long secs) {
	IIVLC vlc(this);
	if (!vlc) 
		return false;
	
	if (S_OK != vlc->shuttle(secs)) 
		return false;
	return true;
}

bool VLCPlayer::IsPlaying() {
	IIVLC vlc(this);
	if (!vlc) 
		return false;
	
	VARIANT_BOOL isplaying;
	if (S_OK != vlc->get_Playing(&isplaying)) 
		return false;
	return isplaying == VARIANT_TRUE;
}

int VLCPlayer::GetVolume() {
	IIVLC vlc(this);
	if (!vlc) 
		return -1;
	
	int volume;
	if (S_OK != vlc->get_Volume(&volume)) 
		return -1;
	return volume;
}

bool VLCPlayer::PutVolume(int volume) {
	IIVLC vlc(this);
	if (!vlc) 
		return false;
	
	if (S_OK != vlc->put_Volume(volume)) 
		return false;
	return true;
}

int VLCPlayer::GetTime() {
	IIVLC vlc(this);
	if (!vlc) 
		return -1;
	
	int mseconds;
	if (S_OK != vlc->get_Time(&mseconds)) 
		return -1;
	return mseconds;
}

bool VLCPlayer::PutTime(int mseconds) {
	IIVLC vlc(this);
	if (!vlc) 
		return false;
	
	if (S_OK != vlc->put_Time(mseconds)) 
		return false;
	return true;
}

float VLCPlayer::GetPosition() {
	IIVLC vlc(this);
	if (!vlc) 
		return -1;
	
	float position;
	if (S_OK != vlc->get_Position(&position)) 
		return -1;
	return position;
}

bool VLCPlayer::PutPosition(float position) {
	IIVLC vlc(this);
	if (!vlc) 
		return false;
	
	if (S_OK != vlc->put_Position(position)) 
		return false;
	return true;
}

int VLCPlayer::GetLength() {
	IIVLC vlc(this);
	if (!vlc) 
		return -1;
	
	int seconds;
	if (S_OK != vlc->get_Length(&seconds)) 
		return -1;
	return seconds;
}

END_UPP_NAMESPACE

#endif
