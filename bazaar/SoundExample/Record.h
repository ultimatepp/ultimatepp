#ifndef _portaudioTest_Record_h_
#define _portaudioTest_Record_h_

class Record:public WithRecordLayout<ParentCtrl>{
	Sound snd;
	struct Data{
		int index;  /* Index into sample array. */
		Vector<uint8> samples;
	};
	Data userdata;

public:
	typedef Record CLASSNAME;
	Record(){
		CtrlLayout(*this);
		play.Disable(); save.Disable();
		reclbl.Hide(); playlbl.Hide();
		rec.SetImage(SoundImg::Record());
		play.SetImage(SoundImg::Play());
		save.SetImage(CtrlImg::save());
		rec<<=THISBACK(OnRecord);
		play<<=THISBACK(OnPlay);
		save<<=THISBACK(OnSave);
		file<<=GetHomeDirFile("out.wav");
		type<<=0;
		file.Disable(); type.Disable();
		snd.WhenFinished=THISBACK(OnFinish);
		snd.SetSampleRate(44100).SetFramesPerBuffer(1024).SetFlags(SND_NOCLIP);
		PopulateSrc();
	}
private:
	void RecordCallback(StreamCallbackArgs& args) {
		Data *data = (Data*)args.data;
		const uint8 *rptr = (const uint8*)args.input;
		if (args.input == NULL)
			for (int i=0; i<args.fpb; i++) data->samples.Add(0);
		else
			for (int i=0; i<args.fpb; i++) data->samples.Add(*rptr++);
		data->index += args.fpb;
		if(data->samples.GetCount()>5*44100)
			args.state=SND_COMPLETE;
	}
	void PlayCallback(StreamCallbackArgs& args){
		Data* data = (Data*)args.data;
		uint8* wptr = (uint8*)args.output;
		unsigned int framesLeft = data->samples.GetCount() - data->index;
		for (int i=0; i<args.fpb; i++) *wptr++ = data->samples[data->index++];
		args.state = (data->samples.GetCount()==data->index)?SND_COMPLETE:SND_CONTINUE;
	}
	void PopulateSrc(){
		const SoundSystem& s=SoundSys();
		src->SetRoot(Null,-1,"Use default input device");
		for(int i=0;i<s.GetAPICount();i++){
			SoundAPI a=s.GetAPI(i);
			src->Add(0,SoundImg::API(),a.index,a.name + String(" (default input device)"),true);
		}
		for(int i=0;i<s.GetCount();i++){
			SoundDevice d=s[i];
			if(d.InputChannels)
				src->Add(src->Find(d.API),SoundImg::Device(),d.index+1024,d.name,false);
		}
		src->OpenDeep(0);
		src<<=-1;
	}
	void OnRecord(){
		userdata.samples.Clear();
		SoundDevice dev;
		int n=~src;
		if(n==-1)
			dev=SoundSys().GetDefaultInput();
		else if(n<1024)
			dev=SoundSys().GetAPI(n).defaultInputDevice;
		else
			dev=SoundSys().GetDevices()[n-1024];
		if(IsNull(dev)){error<<="[1 Error: No default input device."; return;}
		else error.Clear();
		StreamParameters param(dev,1,SND_UINT8,dev.LowInputLatency);
		snd<<=THISBACK(RecordCallback);
		snd.Open(&userdata,param,Null);
		if(snd.IsError()){error<<="[1 Error: "+snd.GetError(); return;}
		else error.Clear();
		rec.Disable(); play.Disable(); save.Disable(); file.Disable(); type.Enable();
		reclbl.Show();
		snd.Start();
		if(snd.IsError()){error<<="[1 Error: "+snd.GetError(); return;}
		else error.Clear();
	}
	void OnPlay(){
		userdata.index=0;
		SoundDevice dev=SoundSys().GetDefaultOutput();
		if (IsNull(dev)){error<<="[1 Error: No default output device."; return;}
		else error.Clear();
		StreamParameters param(dev,1,SND_INT8,dev.LowOutputLatency);
		snd<<=THISBACK(PlayCallback);
		snd.Open(&userdata,Null,param);
		if(snd.IsError()){error<<="[1 Error: "+snd.GetError(); return;}
		else error.Clear();
		rec.Disable(); play.Disable();
		playlbl.Show();
		snd.Start();
		if(snd.IsError()){error<<="[1 Error: "+snd.GetError(); return;}
		else error.Clear();
	}
	void OnFinish(void*){
		playlbl.Hide(); reclbl.Hide();
		rec.Enable(); play.Enable(); save.Enable(); file.Enable(); type.Enable();
		snd.Close();
	}
	void OnSave(){
		if(~type==1){
			// save raw bytes
			FileOut f(AsString(~file));
			f.Put(userdata.samples.Begin(),userdata.samples.GetCount());
			f.Close();
		} else {
			// save .wav file
			WaveFile f;
			f.OpenWrite(AsString(~file));
			f.SetupFormat(44100, 8, 1);
			f.WriteSamples(userdata.samples.Begin(),userdata.samples.GetCount());
			f.Close();
		}
	}
};

#endif
