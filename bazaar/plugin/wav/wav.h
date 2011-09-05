#ifndef _plugin_wav_wav_h_
#define _plugin_wav_wav_h_

#include "wave.h"
#include <Sound/Sound.h>

NAMESPACE_UPP

SampleFormat WavBitsToSampleFormat(int bits);
bool IsWavFile(const char* fn);
bool PlayWav(const char* fn);

END_UPP_NAMESPACE

#endif
