#include "Synth.h"

double ADSR::Evaluate(double t, double duration)
{
	if(t < delay)
		return 0;
	if(t > duration) {
		t -= duration;
		if(t > release)
			return 0;
		return sustain * (release - t) / release;
	}
	t -= delay;
	if(t < attack)
		return t / attack;
	t -= attack;
	if(t < decay) {
		return (sustain * t + decay - t) / decay;
	}
	return sustain;
}

struct FMSound : SoundGenerator, FMPatch {
	virtual bool Get(float *data, int len);

	double  volume;
	double  duration;

	double  fc, fm1, fm2, fmf;

	int     ti = 0;
	
	double  noisedir = 0;
	double  noiseval = 0;

	void SetVolume(double vol)          { volume = vol; }
	
	void Set(double volume, double frequency, double duration, const FMPatch& m);

	FMSound(double volume, double frequency, double duration, const FMPatch& m) {
		Set(volume, frequency, duration, m);
	}
};

void FMSound::Set(double volume, double frequency, double duration, const FMPatch& m)
{
	(FMPatch&)*this = m;

	this->volume = volume;
	this->duration = duration;
	
	fc = frequency;
	fm1 = fc * m1;
	fm2 = fc * m2;
	fmf = fc * mf;
}

bool FMSound::Get(float *b, int len)
{
	bool plays = true;
	double t;
	for(int i = 0; i < len; i++) {
		t = ti * (1.0 / 44200);
		double pt = M_2PI * t;
		
		static dword state = 1;
		state ^= state << 13;
		state ^= state >> 17;
		state ^= state << 5;
		double white = 2.0 / 4294967295.0 * state - 1;
/*		noisedir = clamp(noisedir + 0.0001 * fc * white, -0.08, 0.08);
		noiseval = noisedir;
		if(noiseval > 1) {
			noisedir = -abs(noisedir);
			noiseval = 1;
		}
		if(noiseval < 0) {
			noisedir = abs(noisedir);
			noiseval = -1;
		}
		
		*b++ = tanh(noiseval);
*/		
		
		*b++ = float(volume * adsr.Evaluate(t, duration)
		                    * sin(pt * fc + adsr1.Evaluate(t, duration)
		                                    * beta1
		                                    * sin(pt * fm1 + betan * white + betaf * adsrf.Evaluate(t, duration) * sin(pt * fmf))
		                                   + adsr2.Evaluate(t, duration) * beta2 * sin(pt * fm2)));

		ti++;
	}
	return t < duration + adsr.release;
}

int64 Play(double volume, double frequency, double duration, const FMPatch& patch)
{
	return AddSound(new FMSound(volume, frequency, duration, patch));
}
