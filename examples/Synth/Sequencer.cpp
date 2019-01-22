#include "Synth.h"

void SoundSequence::Put(double at, int i, double volume, double frequency, double duration, bool direct)
{
	SoundEvent& e = At(at).Add();
	e.frequency = (float)frequency;
	e.duration = (float)duration;
	e.volume = (float)volume;
	e.snd = &bank[i];
	if(direct) {
		e.duration = (float)e.snd->op[0].duration;
		e.frequency = (float)e.snd->f;
	}
}

int SoundSequence::SoundIndex(const String& s)
{
	int q = bank.Find(s);
	if(q < 0) {
		q = bank.GetCount();
		bank.Add(s).Load(s);
	}
	return q;
}

void SoundSequence::Put(double at, const String& snd, double volume, double frequency, double duration,
 bool direct)
{
	Put(at, SoundIndex(snd), volume, frequency, duration, direct);
}


const int SEQUENCER_CHANNEL_ID = -123;
std::atomic<const SoundSequence *> sS;

void PlaySequence(const SoundSequence& s)
{
	s.at = s.cursor;
	sS = &s;
}

bool IsPlayingSequence()
{
	return sS.load();
}

void StopSequencer()
{
	sS = NULL;
	StopChannels(SEQUENCER_CHANNEL_ID);
}

void PlayTempSequence(SoundSequence&& s)
{
	static int ii;
	static SoundSequence h[2];
	
	h[ii] = pick(s);
	PlaySequence(h[ii]);
	ii = !ii;
}

void Sequencer(int chunk_size)
{
	static int ch;
	ch += chunk_size;
	if(ch < CHUNK_SIZE)
		return;
	ch -= CHUNK_SIZE;

	int PR = 10;
	const SoundSequence *ss = sS.load();
	if(ss) {
		if(ss->at >= ss->event.GetCount()) {
			if(!IsNull(ss->loop) && ss->event.GetCount())
				ss->at = ss->loop;
			else {
				sS = NULL;
				return;
			}
		}
		const Vector<SoundEvent>& e = ss->event[ss->at++];
		for(const SoundEvent& s : e) {
			int ii = FindChannel(PR, 1, s.volume);
			if(ii >= 0) {
				Sound snd = *s.snd;
				if(s.frequency > 0) {
					snd.f = s.frequency;
					snd.op[0].volume *= s.volume / 100;
					snd.op[0].duration = s.duration;
				}
				SetChannel(ii, snd, 10, SEQUENCER_CHANNEL_ID);
			}
		}
	}
}
