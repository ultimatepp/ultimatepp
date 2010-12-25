#ifndef _portaudioTest_Info_h_
#define _portaudioTest_Info_h_

class AudioInfo:public WithInfoLayout<ParentCtrl>{
	typedef AudioInfo CLASSNAME;
public:
	AudioInfo(){
		CtrlLayout(*this);
		PopulateTree();
		tree.NoRoot().OpenDeep(0);
		tree.WhenSel=THISBACK(OnSel);
		OnSel();
	}
	void PopulateTree(){
		const SoundSystem& s=SoundSys();
		for(int i=0;i<s.GetAPICount();i++){
			SoundAPI a=s.GetAPI(i);
			tree.Add(0,SoundImg::API(),a.index,a.name,true);
		}
		for(int i=0;i<s.GetCount();i++){
			SoundDevice d=s[i];
			tree.Add(tree.Find(d.API),SoundImg::Device(),d.index,d.name,false);
		}
	}
	void OnSel(){
		if(tree.GetSel().GetCount()==0){
			info<<="[1 Select one of the items from the tree on the left...]";
			return;
		}
		int n=tree.GetSel()[0];
		String s;
		if(tree.GetParent(n)==0){
			SoundAPI a(tree.Get(n));
			s<<"[ [ "<<a.name<<"&][1 &][ {{1:1@NFNGN^ "
			   "[ [1 Default input:]]:: [ [1 "<<SoundDevice(a.defaultInputDevice).name<<"]]:: "
			   "[ [1 Default output:]]:: [ [1 "<<SoundDevice(a.defaultOutputDevice).name<<"]]:: "
			   "[ [1 Number of devices:]]:: [ [1 "<<a.deviceCount <<"]]"
			   "}}]";
		}else{
			SoundDevice d(tree.Get(n));
			s<<"[ [ "<<d.name<<"&][1 &][ {{1:1@NFNGN^ "
			   "[ [1 API:]]:: [ [1 "<<SoundAPI(d.API).name<<"]]:: "
			   "[ [1 Input Channels:]]:: [ [1 "<<d.InputChannels<<"]]:: "
			   "[ [1 Output Channels:]]:: [ [1 "<<d.OutputChannels<<"]]:: "
			   "[ [1 Default sample rate:]]:: [ [1 "<<d.SampleRate<<"]]:: "
			   "[ [1 Default input latency:]]:: [ [1 Low: "<<d.LowInputLatency<<" s&High: "<<d.HighInputLatency<<" s ]]:: "
			   "[ [1 Default output latency:]]:: [ [1 Low: "<<d.LowOutputLatency<<" s&High: "<<d.HighOutputLatency<<" s ]]"
			   "}}]";
		}
		info<<=s;
	}
};

#endif
