#include "Sound.h"

NAMESPACE_UPP;

/* SoundStream */

void SoundStream::Open(const StreamParameters& inparam,const StreamParameters& outparam){
	OpenStream(NULL,NULL,inparam,outparam);
}

void SoundStream::OpenDefault(int inchannels, int outchannels, SampleFormat format){
	OpenDefaultStream(NULL,NULL,inchannels,outchannels,format);
}

int SoundStream::ReadAvailable(){
	int n=Pa_GetStreamReadAvailable(stream);
	if(n<0){
		err=n;
		CHECK_ERR;
		return -1;
	}else
		return n;
}

int SoundStream::WriteAvailable(){
	int n=Pa_GetStreamWriteAvailable(stream);
	if(n<0){
		err=n;
		CHECK_ERR;
		return -1;
	}else
		return n;
}



END_UPP_NAMESPACE;
