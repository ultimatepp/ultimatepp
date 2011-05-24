#ifndef _portaudioTest_Wave_h_
#define _portaudioTest_Wave_h_

class Wave:public WithWaveLayout<ParentCtrl>{
	Sound snd;
	double f1,f2,a1,a2,sr;
public:
	typedef Wave CLASSNAME;
	Wave(){
		CtrlLayout(*this);
		freq1<<=THISBACK(OnChange);FileIn f;
		freq2<<=THISBACK(OnChange);
		amp1<<=THISBACK(OnChange);
		amp2<<=THISBACK(OnChange);
		device<<=THISBACK(OnChange);
		samplerate<<=THISBACK(OnChange);
		fpb<<=THISBACK(OnChange);
		button<<=THISBACK(OnPlay);
		button.SetLabel("Play");
		freq1<<=440;  freq2<<=440;
		amp1 <<=1;    amp2 <<=1;
		for(int i=0;i<SoundSys().GetCount();i++){
			device.Add(i,SoundSys()[i].name);
		}
		device<<=SoundSys().GetDefaultOutput().index;
		samplerate.AddList(4000.0);
		samplerate.AddList(8000.0);
		samplerate.AddList(9600.0);
		samplerate.AddList(11025.0);
		samplerate.AddList(12000.0);
		samplerate.AddList(16000.0);
		samplerate.AddList(22050.0);
		samplerate.AddList(24000.0);
		samplerate.AddList(32000.0);
		samplerate.AddList(44100.0);
		samplerate.AddList(48000.0);
		samplerate.AddList(88200.0);
		samplerate.AddList(96000.0);
		samplerate<<=44100;
		fpb<<=1024;
		snd<<=THISBACK(Generator);
		snd.SetFlags(SND_NOCLIP);
		button.SetImage(SoundImg::Play(),SoundImg::Pause());
	}
private:
	void Generator(StreamCallbackArgs& arg){
		static unsigned int n=0;
		double T=M_2PI/sr;
		for(int i=0;i<arg.fpb;i++){
			((float*)arg.output)[2*i]  =a1*sin(f1*n*T);
			((float*)arg.output)[2*i+1]=a2*sin(f2*n*T);
			n++;
		}
	}
	void OnChange(){
		if(!~button) return;
		KillTimeCallback();
		cpu.SetLabel("CPU usage: -");
		snd.Close();
		Start();
	}
	void CpuUsage(){
		if(!snd.IsActive()) cpu.SetLabel("CPU usage: -");
		else cpu.SetLabel(Format("CPU usage: %1n ",snd.CpuLoad()*100)+"\x25");
	}
	void Start(){
		f1=~freq1;f2=~freq2;
		a1=~amp1; a2=~amp2;
		sr=~samplerate;
		snd.SetSampleRate(sr).SetFramesPerBuffer((fpb>0)?(int)fpb:1);
		snd.Open(Null,StreamParameters(~device,2,SND_FLOAT32,SoundDevice(~device).LowOutputLatency));
		if(CheckError()) return;
		snd.Start();
		SetTimeCallback(-250,THISBACK(CpuUsage));
	};
	void OnPlay(){
		if(~button){
			button.SetLabel("Stop");
			Start();
		}else{
			button.SetLabel("Play");
			KillTimeCallback();
			cpu.SetLabel("CPU usage: -");
			snd.Close();
		}
	}
	bool CheckError(){
		if(snd.IsError()){
			error<<="[1 Error: "+snd.GetError();
			return true;
		}
		error<<="";
		return false;
	}
};

#endif
