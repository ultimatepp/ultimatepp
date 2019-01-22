#include "Synth.h"

#include "sample.i"

double waveform_table[9][4096];

INITBLOCK {
	for(int i = 0; i < 4096; i++) {
		double arg = M_2PI * i / 4096;
		waveform_table[WAVEFORM_SIN][i] = sin(arg);
		waveform_table[WAVEFORM_SQUARE][i] = sgn(waveform_table[WAVEFORM_SIN][i]);
		waveform_table[WAVEFORM_TRIANGLE][i] = abs(2048 - i) / 1024.0 - 1;
		waveform_table[WAVEFORM_SAWTOOTH][i] = 2 * i / 4096.0 - 1;
	}
	
	for(int i = 0; i < 4096; i++) {
		waveform_table[4][i] = wave_saxophone[i];
		waveform_table[5][i] = wave_violin[i];
		waveform_table[6][i] = wave_doublebass[i];
		waveform_table[7][i] = wave_banjo[i];
		waveform_table[8][i] = wave_trumpet[i];
	}
}

double Rate(double x)
{
	return pow(10, (6 * x / 100 - 6));
}

void SoundGen::FMOPGen::Comp()
{
	attack = 1 + Rate(attack);
	decay = 1 - Rate(decay);
	release = 1 - Rate(release);
	duration = 44100 * duration / 1000;
	fdrift = exp2(fdrift / 12 / 44100);
	if(volume > 0)
		volume = pow(10, (volume - 100) / 40);
	if(sustain > 0)
		sustain = pow(10, (sustain - 100) / 40);
	wave_tab = waveform_table[clamp(waveform, 0, (int)WAVEFORM_LASTSAMPLE)];
}

String SoundGen::FMOPGen::ToString() const
{
	return String() << "Volume: " << volume << ", phase: " << p << ", ADSR volume: " << v;
}

force_inline
double SoundGen::FMOPGen::Evaluate(int t, double mf, double mod, double& cv)
{
	if(volume == 0)
		return 0;
	if(t >= duration) {
		v *= release;
		cv = v;
	}
	else
	switch(p) {
	case 0:
		v *= attack;
		if(v >= 1.0) {
			v = 1.0;
			p = 1;
		}
		cv = v > 0;
		break;
	case 1:
		v *= decay;
		if(v < sustain) {
			v = sustain;
			p = 2;
		}
		cv = v;
		break;
	default:
		cv = v = sustain;
	}
	cv *= volume;
	f *= fdrift;
	double fn;
	if(findarg(waveform, WAVEFORM_BROWN, WAVEFORM_WHITE) >= 0) {
		static dword state = 1;
		state ^= state << 13;
		state ^= state >> 17;
		state ^= state << 5;
		fn = 2.0 / 4294967295.0 * state - 1;
		if(waveform == WAVEFORM_BROWN)
			fn = n = clamp(n + 0.06 * fn, -1.0, 1.0);
	}
	else {
#if 0
		double arg = mf * f * t + mod;
		if(waveform == WAVEFORM_SIN)
			fn = sin(arg);
		else {
			arg = fmod(arg, M_2PI);
			fn = waveform == WAVEFORM_SQUARE ? sgn(abs(M_PI - arg) / (M_PI / 2) - 1) :
			     waveform == WAVEFORM_TRIANGLE ? abs(M_PI - arg) / (M_PI / 2) - 1 :
			     waveform == WAVEFORM_SAWTOOTH ? arg / M_PI - 1 :
			     0;
		}
#else
		double arg = mf * f * t + mod;
		fn = wave_tab[dword((4096 / M_2PI) * arg) & 4095];
#endif
	}
	
	return volume * v * fn;
}

void SoundGen::Start(const Sound& s)
{
	for(int i = 0; i < OPCOUNT; i++) {
		(FMOP&)op[i] = s.op[i];
		op[i].Comp();
		op[i].Start();
	}
	f = s.f / 22100 * M_PI;
	lpan = (float)s.pan;
	rpan = (float)(1 - s.pan);
	for(int i = 3; i <= 4; i++) // vibrato, tremolo
		op[i].f = op[i].f / 22100 * M_PI;
	t = 0;
	memset(feedback, 0, sizeof(feedback));
	current_volume = 1;
	lfo_mod = Randomf() * M_2PI;
}

float SoundGen::Get()
{
#if 0
	float r = op[0].Evaluate(t, f, 0, current_volume);
#else
	if(current_volume < 0.001)
		return 0;
	double dummy;
	double lfo1 = op[3].Evaluate(t, 1, lfo_mod, dummy);
	double lfo2 = op[4].Evaluate(t, 1, lfo_mod, dummy);
	double v = (1 - op[4].volume + lfo2);
	float r =
		(float)(v *
			op[0].Evaluate(t, f, op[1].Evaluate(t, f, lfo1, dummy) + op[2].Evaluate(t, f, lfo1, dummy) + lfo1, current_volume)
	     );
#endif
	t++;
	return r;
}

String SoundGen::ToString() const
{
	String r;
	r << "----- Current volume: " << current_volume << ", time: " << t << '\n';
	for(int i = 0; i < OPCOUNT; i++)
		r << i << ": " << op[i] << '\n';
	return r;
}
