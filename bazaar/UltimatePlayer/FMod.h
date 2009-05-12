#ifndef _UltimatePlayer_FMod_h_
#define _UltimatePlayer_FMod_h_

using namespace Upp;

class SoundSystem
{
	FMOD::System * system;
	FMOD::ChannelGroup * master;
	FMOD_RESULT result;
	
	public:
	
		FMOD::System * GetSystem() { return system; }
		
		SoundSystem();
		~SoundSystem();
		
		int   Open();
		void  Close();
		
		void  SetVolume(float vol);
		float GetVolume();
	
};

class SoundStream
{
	FMOD_RESULT result;

	FMOD::Sound   * sound;	
	FMOD::Sound   * track;
	
	FMOD::System  * system;
	FMOD::Channel * channel;
	
	FMOD_SOUND_TYPE type;
	FMOD_SOUND_FORMAT format;
	int channels;
	int bits;
	int length;
	
	String drive;
	int track_number;

	public:
	
		SoundStream();
		SoundStream(SoundSystem &sys);
		~SoundStream();
		
		void   Init();
		
		void   SetSystem(SoundSystem &sys);
		
		bool   Play(const char * stream, int track_number = -1);
		bool   Stop();
		bool   Pause();

		int    GetMsLen();
		int    GetByteLen();
		
		float  GetPos();
		void   SetPos(float pos);
		
		int    GetBytePos();
		void   SetBytePos(int pos);
		
		int    GetMsPos();
		void   SetMsPos(int ms);
		
		int    GetBits()     { return bits;     }
		int    GetChannels() { return channels; }
		void   GetFormat();
		
		bool   IsPlaying();
		bool   IsPaused();
		
		void   SetTrack(int n);
		int    GetTracksCount();
		FMOD::Sound * GetTrack() { return track; }
		
		FMOD::Sound * Open(const char * stream, int mode = FMOD_OPENONLY);
		FMOD::Sound * Open(const String &stream, int mode = FMOD_OPENONLY);
		
		void Close(FMOD::Sound *& sound);
		void Close();
		
		int  GetLen(FMOD::Sound * sound, int unit = FMOD_TIMEUNIT_MS);
		int  GetPos(FMOD::Channel * channel, int unit = FMOD_TIMEUNIT_MS);
		void SetPos(FMOD::Channel * channel, int pos, int unit = FMOD_TIMEUNIT_MS);
		
		
		WString GetTag(const char * tagname);		
		WString GetTag(FMOD::Sound * sound, const char * tagname);
		
};

#endif
