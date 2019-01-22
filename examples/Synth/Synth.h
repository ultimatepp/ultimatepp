#ifndef _Synth_Synth_h
#define _Synth_Synth_h

#include <Core/Core.h>

using namespace Upp;

enum FORMS {
	WAVEFORM_SIN = 0,
	WAVEFORM_SQUARE = 1,
	WAVEFORM_TRIANGLE = 2,
	WAVEFORM_SAWTOOTH = 3,

	WAVEFORM_FIRSTSAMPLE = 4,
	WAVEFORM_SAXOPHONE = WAVEFORM_FIRSTSAMPLE,
	WAVEFORM_VIOLIN = 5,
	WAVEFORM_DOUBLEBASS = 6,
	WAVEFORM_BANJO = 7,
	WAVEFORM_TRUMPET = 8,
	WAVEFORM_LASTSAMPLE = 8,

	WAVEFORM_BROWN = 100,
	WAVEFORM_WHITE = 101,
};

struct FMOP {
	double duration = 99000;
	double volume = 0;

	double f = 1;
	double fdrift = 0;

	double attack = 100;
	double decay = 100;
	double sustain = 100;
	double release = 100;
	
	int    waveform = WAVEFORM_SIN;
	
	String Save() const;
	const char *Load(const char *s);
};

#define OPCOUNT 5

struct Sound {
	double f = 440;
	FMOP   op[OPCOUNT];
	double pan = 0.5;
	
	String Save() const;
	void   Load(const char *s);
	
	Sound();
};

struct SoundGen {
	struct FMOPGen : FMOP {
		int     p;
		double  v;
		double  n;
		double *wave_tab;
		
		void   Start() { v = 1e-3; p = 0; n = 0; }
		void   Comp();
		String ToString() const;
		
		double Evaluate(int t, double mf, double mod, double& current_volume);
	};
	
	int      serial;
	int      param_serial;
	int      id;
	int      priority;
	double   f = 440;
	float    lpan = 0.5f;
	float    rpan = 0.5f;
	int      t;
	int      delay;
	FMOPGen  op[OPCOUNT];
	float    feedback[8192];
	double   current_volume = 0;
	double   lfo_mod = 0;
	
	void     Start(const Sound& s);
	float    Get();
	String   ToString() const;
};

#define CHUNK_SIZE   512
#define NUM_CHANNELS 20

void InitSoundSynth(bool initsdl = true);
void CloseSoundSynth(bool exitsdl = true);

void SetChannel(int chi, const Sound& c, int priority = INT_MAX, int id = 0);
void SetChannelVolume(int chi, double volume);
void StopChannelById(int id);
int  FindChannel(int priority, int from, double new_volume);
void StopChannels(int id);

void SetGlobalVolume(float vol);

struct SoundEvent : Moveable<SoundEvent> {
	Sound *snd;
	float  duration;
	float  frequency;
	float  volume;
};

struct SoundSequence {
	mutable int  at = 0;
	int cursor = 0;
	int loop = Null;
	ArrayMap<String, Sound>    bank;
	Vector<Vector<SoundEvent>> event;

	int                 GetAt(double at)  { return (int)(at * 44100 / 512); }
	Vector<SoundEvent>& At(double at)     { return event.At(GetAt(at)); }
	void                LoopAt(double at) { loop = GetAt(at); }
	int  SoundIndex(const String& s);
	void Put(double at, int i,
	         double volume, double freqency, double duration, bool direct = false);
	void Put(double at, const String& snd,
	         double volume, double freqency, double duration, bool direct = false);
};

void PlaySequence(const SoundSequence& s);
void PlayTempSequence(SoundSequence&& s);

void StopSequencer();
bool IsPlayingSequence();

SoundSequence ParseQSF(const String& data);

#endif
