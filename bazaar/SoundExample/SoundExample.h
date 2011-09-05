#ifndef _portaudioTest_AudioTest_h_
#define _portaudioTest_AudioTest_h_

#include <Sound/Sound.h>
#include <plugin/wav/wav.h>
#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#define LAYOUTFILE <SoundExample/SoundExample.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS SoundImg
#define IMAGEFILE <SoundExample/SoundExample.iml>
#include <Draw/iml_header.h>

#include "Wave.h"
#include "Info.h"
#include "Record.h"
#include "Wav.h"

class AudioApp:public WithAudioLayout<TopWindow>{
public:
	typedef AudioApp CLASSNAME;
	AudioInfo info;
	Wave wave;
	Record record;
	Wav wav;
	AudioApp(){
		CtrlLayout(*this,"Audio Example");
		Sizeable().Icon(SoundImg::Device());
		tabs.Add(info.SizePos(),"Device Info");
		tabs.Add(wave.SizePos(),"Device test");
		tabs.Add(record.SizePos(),"Record");
		tabs.Add(wav.SizePos(),"Wav file");
	}
};
#endif
