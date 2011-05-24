#include "Sound.h"

NAMESPACE_UPP;

extern "C"{
	//this is a C callable function, to wrap U++ Callback into PaStreamCallback
	int StreamCallback(const void *input, void *output, unsigned long frames, 
	          const PaStreamCallbackTimeInfo *timeinfo, unsigned long flags, void *data)
	{
		StreamCallbackData *scd = static_cast<StreamCallbackData*>(data);
		StreamCallbackArgs sca(input, output, frames, timeinfo, flags,scd->data);
		scd->func(sca);
		return(sca.state);
	}

	//this is a C callable function, to wrap WhenFinish into PaStreamFinishedCallback
	void StreamFinishedCallback(void *data){
		StreamCallbackData *scd = static_cast<StreamCallbackData*>(data);
		scd->finish(scd->data);
	}
}


/* ASound */

ASound::ASound():err(paNoError),samplerate(44100), fpb(1024),flags(SND_NOFLAG){
	SoundSys();
}

void ASound::OpenStream(PaStreamCallback* cb, void* data,
                        const StreamParameters& inparam, const StreamParameters& outparam){
	ASSERT(SoundSystem::Exists());
	const PaStreamParameters* noparam=NULL;
	err = Pa_OpenStream(&stream, IsNull(inparam)?noparam:inparam, IsNull(outparam)?noparam:outparam,
	                    samplerate, fpb, flags, cb, data);
	CHECK_ERR;
}

void ASound::OpenDefaultStream(PaStreamCallback* cb, void* data,
                               int inchannels,int outchannels, SampleFormat format){
	ASSERT(SoundSystem::Exists());
	err = Pa_OpenDefaultStream(&stream,inchannels,outchannels,format,samplerate,fpb,cb,data);
	CHECK_ERR;
}

void ASound::Close(){
	if (IsOpen()){
		err = Pa_CloseStream(stream);
		CHECK_ERR;
		stream = NULL;
	}
}

bool ASound::IsStopped() const{
	int err = Pa_IsStreamStopped(stream);
	CHECK_ERR;
	return err;
}

bool ASound::IsActive() const{
	int err = Pa_IsStreamActive(stream);
	CHECK_ERR;
	return err;
}

double ASound::GetInputLatency() const{
	const PaStreamInfo *info = Pa_GetStreamInfo(stream);
	if (info != NULL) return info->inputLatency;
	else return 0;
}

double ASound::GetOutputLatency() const{
	const PaStreamInfo *info = Pa_GetStreamInfo(stream);
	if (info != NULL) return info->outputLatency;
	else return 0;
}

double ASound::GetSampleRate() const{
	const PaStreamInfo *info = Pa_GetStreamInfo(stream);
	if (info != NULL) return info->sampleRate;
	else return 0;
}

/* Sound */

void Sound::Open(void* data, const StreamParameters& inparam,const StreamParameters& outparam){
	scallback=StreamCallbackData(WhenAction,WhenFinished,data);
	OpenStream(&StreamCallback,static_cast<void *>(&scallback),inparam,outparam);
	if(err==paNoError) SetFinishCallback();
}

void Sound::Open(const StreamParameters& inparam,const StreamParameters& outparam){
	Open(NULL, inparam, outparam);
}

void Sound::OpenDefault(void* data, int inchannels,int outchannels, SampleFormat format){
	scallback=StreamCallbackData(WhenAction,WhenFinished,data);
	OpenDefaultStream(&StreamCallback, static_cast<void *>(&scallback),inchannels,outchannels,format);
	if(err==paNoError) SetFinishCallback();
}

void Sound::OpenDefault(int inchannels, int outchannels, SampleFormat format){
	OpenDefault(NULL, inchannels, outchannels, format);
}

void Sound::SetFinishCallback(){
	err=Pa_SetStreamFinishedCallback(stream,&StreamFinishedCallback);
	CHECK_ERR;
}


END_UPP_NAMESPACE;
