#include "wav.h"

NAMESPACE_UPP

SampleFormat WavBitsToSampleFormat(int bits){
	switch(bits){
		case 8:  return SND_UINT8;
		case 16: return SND_INT16;
		case 24: return SND_INT24;
		case 32: return SND_INT32;
	}
	return SND_UNKNOWN;
}

bool IsWavFile(const char* fn){
	RiffFile f(fn);
	if(f.filep()==NULL)
		return false;
	return strcmp(f.subType(),"WAVE")==0;
}

bool PlayWav(const char* fn){
	WaveFile f;
	if(!f.OpenRead(fn)) return false;
	if(f.IsCompressed()){
		// TODO ...
		LOG("Compressed wave files not supported yet");
		return false;
	}
	SoundStream s;
	SoundDevice dev=SoundSys().GetDefaultOutput();
	SampleFormat format=WavBitsToSampleFormat(f.GetBitsPerChannel());
	s.SetFlags(SND_NOCLIP)
	 .SetFramesPerBuffer(512)
	 .SetSampleRate(f.GetSampleRate());
	s.Open(Null,StreamParameters(dev,f.GetNumChannels(),format,dev.LowOutputLatency));
	s.Start();
	if(s.IsError()){
		LOG("Failed to open Sound Stream for file "<<fn<<"\nError: "<<s.GetError());
		return false;
	}
	
	int i=f.GetNumSamples();                  // total length in frames
	const int len=1024;                       // length of our buffer (in frames)
	int n=min(s.WriteAvailable(),len);        // how many frames can be written
	int bps=f.GetBytesPerSample();            // how many bytes makes up one frame
	int t=1000.0/f.GetSampleRate()/2.0*1024;  // the sleep time depends on buffer size and sample rate
	Buffer<char> buf(len*bps);
	while(f.ReadRaw(~buf,n*bps)){
		s.Write(~buf,n);                       // write n frames
		i-=n;
		Sleep(t);                              // we can wait a while, before filing the buffer again
		n=min(s.WriteAvailable(),len);
	}
	s.Write(~buf,i);                           // write the leftovers
	
	s.Close();
	f.Close();
	return true;
}

END_UPP_NAMESPACE