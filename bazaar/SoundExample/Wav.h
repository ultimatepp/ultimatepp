#ifndef _SoundExample_WavInfo_h_
#define _SoundExample_WavInfo_h_

NAMESPACE_UPP;
String FormatFileSize(int64 n); //defined in FileSel.cpp
END_UPP_NAMESPACE;

class Wav : public WithWavLayout<TopWindow> {
	typedef Wav CLASSNAME;
	FileSel fs;
public:
	Wav(){
		CtrlLayout(*this, "");
		browse.SetImage(CtrlImg::open());
		play.SetImage(SoundImg::Play());
		play.Disable();
		browse<<=THISBACK(OnBrowse);
		play<<=THISBACK(OnPlay);
		fn<<=THISBACK(OnChange);
		fs.Type("Wave files","*.wav");
		fs.Type("All files","*.*");
	}
	void OnBrowse(){
		if(fs.ExecuteOpen())
			fn<<=fs.Get();
		OnChange();
	}
	void OnChange(){
		String file=fn;
		if(!FileExists(file)){
			info<<="";
			play.Disable();
			return;
		}
		if(!IsWavFile(file)){
			info<<="[1 This is not a valid WAVE file.]";
			play.Disable();
			return;
		}
		WaveFile f;
		f.OpenRead(file);
		String s="[2 Audio information:] [1 &";
		s<<"[ [ {{1:1@NFNGN^<200 "
		   "[ File size:]:: [ "<<FormatFileSize(GetFileLength(file))<<"]:: "
		   "[ Audio data:]:: [ "<<FormatFileSize(f.GetDataLength())<<"]:: "
		   "[ Non-audio data:]:: [ "<<FormatFileSize(GetFileLength(file)-f.GetDataLength())<<"]:: "
		   "[ Format type:]:: [ "<<f.GetFormatType()<<"]:: "
		   "[ Compressed:]:: [ "<<(f.IsCompressed()?"yes":"no")<<"]:: "
		   "[ Sample rate:]:: [ "<<f.GetSampleRate()<<"]:: "
		   "[ Channels:]:: [ "<<f.GetNumChannels()<<"]:: "
		   "[ Bits per channel:]:: [ "<<f.GetBitsPerChannel()<<"]:: "
		   "[ Length:]:: [ "<<f.GetNumSeconds()<<" s ]:: "
		   "[ Samples:]:: [ "<<f.GetNumSamples()<<"]:: "
		   "[ Bytes per sample:]:: [ "<<f.GetBytesPerSample()<<"]:: "
		   "[ Bytes per second:]:: [ "<<f.GetBytesPerSecond()<<"] "
		   "}}]";
		String extra;
		f.ResetToStart(); //just to be sure
		std::string type,value;
		while(f.GetNextExtraItem(type,value)){
			extra<<"[ "<<String(type)<<":]:: [ "<<String(value)<<" ]:: ";
		}
		if(!extra.IsEmpty()){
			extra=extra.Left(extra.GetLength()-3); //avoid last ":: " messing up the table
			s<<"[2 &Aditional information available:] {{1:1@NFNGN^<200 "<<extra<<"}}]";
		};
		info<<=s;
		play.Enable(!f.IsCompressed());
	}
	void OnPlay(){
		play.Disable();
		if(!PlayWav(String(fn)))
			Exclamation("[1 ERROR: Unable to play file :-( &"
			            "File might be in format that is not supported by your hardware "
			            "(wrong sample rate,number of channels,...)&]");
		play.Enable();
	}
};

#endif
