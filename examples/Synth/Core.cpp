#include "Synth.h"

#ifdef PLATFORM_POSIX
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

SoundGen app_sch[NUM_CHANNELS]; // to avoid lengthy locking, keep copy under different mutex
SoundGen gen_sch[NUM_CHANNELS]; // this is copied to generator and used to actually generate the sound

INITBLOCK {
	for(int i = 0; i < NUM_CHANNELS; i++) // initialize wave_tab
		for(int j = 0; j < OPCOUNT; j++) {
			app_sch[i].op[j].Comp();
			gen_sch[i].op[j].Comp();
		}
}

int      in_sch_serial;

#if 1
struct AppSoundLock {
	AppSoundLock() { SDL_LockAudio(); }
	~AppSoundLock() { SDL_UnlockAudio(); }
};

struct GenSoundLock {
	GenSoundLock() {}
	~GenSoundLock() {}
};
#else
SpinLock s_in_sch_lock;

struct AppSoundLock {
	AppSoundLock() { s_in_sch_lock.Enter(); }
	~AppSoundLock() { s_in_sch_lock.Leave(); }
};

struct GenSoundLock {
	GenSoundLock() { s_in_sch_lock.Enter(); }
	~GenSoundLock() { s_in_sch_lock.Leave(); }
};
#endif

void SetChannel(int chi, const Sound& c, int priority, int id)
{
	AppSoundLock __;
	SoundGen& ch = app_sch[chi];
	ch.Start(c);
	ch.priority = priority;
	ch.serial = ++in_sch_serial;
	ch.id = id;
}

void StopChannels(int id)
{
	AppSoundLock __;
	for(int i = 0; i < NUM_CHANNELS; i++) {
		SoundGen& ch = app_sch[i];
		if(ch.id == id) {
			ch.serial = ++in_sch_serial;
			ch.current_volume = app_sch[i].op[0].volume = 0;
		}
	}
}

void SetChannelVolume(int chi, double volume)
{
	AppSoundLock __;
	SoundGen& ch = app_sch[chi];
	ch.param_serial = ++in_sch_serial;
	ch.op[0].volume = pow(10, (volume - 100) / 40);
}

int FindChannel(int priority, int from, double new_volume)
{
	new_volume = pow(10, (new_volume - 100) / 40);
	AppSoundLock __;
	int    besti = -1;
	int    best_priority = INT_MAX;
	double best_volume = 100;
	for(int i = from; i < NUM_CHANNELS; i++) {
		SoundGen& ch = app_sch[i];
//		if(ch.current_volume < 0.001) {
//			besti = i;
//			break;
//		}
		if(/*ch.priority <= priority &&
		   (ch.priority < best_priority || ch.priority == best_priority && */ch.current_volume < best_volume) {
		       besti = i;
//		       best_priority = ch.priority;
		       best_volume = ch.current_volume;
		}
	}
//	return besti;
	return best_volume < new_volume ? besti : -1;
//	DDUMP(best_volume);
//	DDUMP(besti);
/*	if(besti < 0)
		for(int i = from; i < NUM_CHANNELS; i++)
			if(app_sch[i].current_volume < 0.01) {
				besti = i;
				break;
			}
	return besti;*/
}

extern void Sequencer(int chunk_size);

float global_volume = 1;

void SetGlobalVolume(float vol)
{
	AppSoundLock __;
	global_volume = vol;
}

void MyAudioCallback(void *, Uint8 *stream, int len)
{
	int chunk_size = len / (2 * sizeof(float));
	Sequencer(chunk_size);
	{
#if 0
		RLOG("====================================");
#endif
		GenSoundLock __;
		for(int i = 0; i < NUM_CHANNELS; i++) {
			if(app_sch[i].serial != gen_sch[i].serial)
				gen_sch[i] = app_sch[i];
			if(app_sch[i].param_serial != gen_sch[i].param_serial) {
				gen_sch[i].op[0].volume = app_sch[i].op[0].volume;
				gen_sch[i].current_volume = 1;
				app_sch[i].param_serial = gen_sch[i].param_serial;
			}
#if 0
			RLOG(i);
			RLOG(gen_sch[i]);
#endif
		}
	}
	
	float *d = (float *)stream;

	memset(d, 0, len);
	for(int j = 0; j < NUM_CHANNELS; j++) {
		SoundGen& ch = gen_sch[j];
		d = (float *)stream;
		double v = 0;
		for(int i = 0; i < chunk_size; i++) {
			float h = ch.Get();
			*d++ += ch.lpan * h;
			*d++ += ch.rpan * h;
		}
	}

	float *e = (float *)stream + 2 * chunk_size;
	for(float *d = (float *)stream; d < e; d++)
		*d = clamp(*d * global_volume, -1.0f, 1.0f);

	{
		GenSoundLock __;
		for(int i = 0; i < NUM_CHANNELS; i++) {
			app_sch[i].current_volume = gen_sch[i].current_volume;
		}
	}
}

void InitSoundSynth(bool initsdl)
{
	if(initsdl && SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0)
		return;
	
	SDL_AudioSpec want;
	memset(&want, 0, sizeof(want));
	want.freq = 44100;
	want.format = AUDIO_F32SYS;
	want.channels = 2;
	want.samples = CHUNK_SIZE;
	want.callback = MyAudioCallback;
	
	if(SDL_OpenAudio(&want, NULL) < 0)
	   LOG("Failed to open audio: " + (String)SDL_GetError());
	
	SDL_PauseAudio(0);
}

void CloseSoundSynth(bool exitsdl)
{
	SDL_CloseAudio();
	if(exitsdl)
		SDL_Quit();
}
