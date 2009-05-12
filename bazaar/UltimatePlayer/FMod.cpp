#include <CtrlLib/CtrlLib.h>

#include <fmod.hpp>
#include <fmod_errors.h>
#include "fmod.h"

bool CheckError(FMOD_RESULT result, bool show_message = true)
{
	if(result != FMOD_OK)
	{
		if(show_message)
			PromptOK(Format("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result)));
		return true;
	}
	return false;
}

SoundSystem::SoundSystem()
{
	system = NULL;
}

SoundSystem::~SoundSystem()
{
	Close();
}

void SoundSystem::Close()
{
	if(!system)
		return;
	system->close();
	system->release();
	system = NULL;
}

int SoundSystem::Open()
{
	result = FMOD::System_Create(&system);
	if(CheckError(result))
		return -1;

	unsigned int version;	
	result = system->getVersion(&version);
	if(CheckError(result))
		return -1;

	if(version < FMOD_VERSION)
	{
		PromptOK(Format("You are using an old version of FMOD %08x&This program requires %08x\n", (int) version, FMOD_VERSION));
		return -1;
    }	
	
	//result = system->setSpeakerMode(FMOD_SPEAKERMODE_5POINT1);
	//if(CheckError())
	//	return -2;
	
	result = system->init(32, FMOD_INIT_NORMAL, 0);
	if(CheckError(result))
		return -3;

	result = system->getMasterChannelGroup(&master);
	if(CheckError(result))
		return -4;
	
	return 0;
}

void SoundSystem::SetVolume(float vol)
{
	master->setVolume(vol / 100.0f);
}

float SoundSystem::GetVolume()
{
	float volume;
	master->getVolume(&volume);
	return volume * 100.0f;
}

/*-----------------------------------------------------------------------------*/

void SoundStream::SetSystem(SoundSystem  &sys)
{
	system = sys.GetSystem();
}

void SoundStream::Init()
{
	system   = NULL;
	channel  = NULL;
	sound    = NULL;
	track    = NULL;
//	length   = 0;
	format   = FMOD_SOUND_FORMAT_NONE;
	type     = FMOD_SOUND_TYPE_UNKNOWN;
	channels = 0;
	bits     = 0;
	track_number = -1;
}

SoundStream::SoundStream()
{
	Init();
}

SoundStream::SoundStream(SoundSystem &sys)
{
	Init();
	system = sys.GetSystem();
}

SoundStream::~SoundStream()
{
}
/*
bool SoundStream::Play(int track_number)
{
	if(sound)
	{
		SetTrack(track_number);
		result = system->playSound(FMOD_CHANNEL_FREE, track, false, &channel);
		if(!CheckError(result, 0))
			return true;
	}
	return false;	
}*/

bool SoundStream::Play(const char * stream, int track_number)
{
	if(track_number >= 0)
	{		
		if(drive != stream)
		{
			Stop();
			drive = stream;	
		}
		this->track_number = track_number;
	}
	else
		Stop();
		
	sound = Open(stream);
	
	if(sound)
	{
		if(track_number < 0)
			result = system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
		else
		{
			SetTrack(track_number);
			result = system->playSound(FMOD_CHANNEL_FREE, track, false, &channel);
		}
		if(!CheckError(result, 0))
			return true;
	}
	return false;
}

bool SoundStream::Stop()
{
	track_number = -1;
	drive.Clear();
	
	if(sound)
	{
		sound->release();
		sound = NULL;
		return true;
	}
	return false;
}

bool SoundStream::Pause()
{
	bool paused;
	channel->getPaused(&paused);
	channel->setPaused(!paused);
	return paused;
}

void SoundStream::SetPos(float pos)
{
	SetMsPos((int((pos * length) / 100.0f)));
}

float SoundStream::GetPos()
{
	return (GetMsPos() * 100) / (float) length;
}

int SoundStream::GetMsPos()
{
	return GetPos(channel, FMOD_TIMEUNIT_MS);
}

int SoundStream::GetBytePos()
{
	return GetPos(channel, FMOD_TIMEUNIT_RAWBYTES);
}

void SoundStream::SetBytePos(int pos)
{
	SetPos(channel, pos, FMOD_TIMEUNIT_RAWBYTES);
}

void SoundStream::SetMsPos(int ms)
{
	SetPos(channel, ms, FMOD_TIMEUNIT_MS);
}

bool SoundStream::IsPlaying()
{
	bool playing;
	channel->isPlaying(&playing);
	return playing;
}

bool SoundStream::IsPaused()
{
	bool paused;
	channel->getPaused(&paused);
	return paused;
}

WString SoundStream::GetTag(const char * tagname)
{
	return GetTag(sound, tagname);
}

WString SoundStream::GetTag(FMOD::Sound * sound, const char * tagname)
{
	FMOD_TAG tag;
	result = sound->getTag(tagname, 0, &tag);
	if(CheckError(result, 0))
		return Null;

	switch(tag.datatype)
	{
		case FMOD_TAGDATATYPE_INT:
			return WString(AsString(*((int *) tag.data)));
		case FMOD_TAGDATATYPE_FLOAT:
			return WString(AsString(*((float *) tag.data)));
		case FMOD_TAGDATATYPE_STRING:
			return ToUnicode((const char *) tag.data, CHARSET_WIN1250);// + atoi(GetUserLocale(LOCALE_IDEFAULTANSICODEPAGE)) - 1250);
		case FMOD_TAGDATATYPE_STRING_UTF16:
		case FMOD_TAGDATATYPE_STRING_UTF16BE:
		case FMOD_TAGDATATYPE_STRING_UTF8:
			//return FromUnicode(WString((const wchar *) tag.data));
			return WString((const wchar *) tag.data);
    }
	return Null;
}


FMOD::Sound * SoundStream::Open(const char * stream, int mode)
{
	if(sound)
		return sound;
	result = system->createStream(stream, mode, 0, &sound);
	if(CheckError(result, 0))
		return NULL;
	
	return sound;
}

FMOD::Sound * SoundStream::Open(const String &stream, int mode)
{
	return Open(~stream, mode);
}

void SoundStream::Close(FMOD::Sound *& sound)
{
	if(sound)
	{
		result = sound->release();
		sound = NULL;
		CheckError(result, 0);
	}
}

void SoundStream::Close()
{
	Close(sound);
}

int SoundStream::GetMsLen()
{
	return GetLen(track_number < 0 ? sound : track, FMOD_TIMEUNIT_MS);
}

int SoundStream::GetByteLen()
{
	return GetLen(track_number < 0 ? sound : track, FMOD_TIMEUNIT_RAWBYTES);
}

int SoundStream::GetLen(FMOD::Sound * sound, int unit)
{
	unsigned int len;
	sound->getLength(&len, unit);
	length = len;
	return len;
}

int SoundStream::GetPos(FMOD::Channel * channel, int unit /* = FMOD_TIMEUNIT_MS*/)
{
	unsigned int pos;
	channel->getPosition(&pos, unit);
	return pos;
}

void SoundStream::SetPos(FMOD::Channel * channel, int pos, int unit /* = FMOD_TIMEUNIT_MS*/)
{
	channel->setPosition(pos, unit);
}

void SoundStream::GetFormat()
{
	sound->getFormat(&type, &format, &channels, &bits);
}

void SoundStream::SetTrack(int n)
{
	sound->getSubSound(n, &track);
}

int SoundStream::GetTracksCount()
{
	int tracks;
	sound->getNumSubSounds(&tracks);
	return tracks;
}

