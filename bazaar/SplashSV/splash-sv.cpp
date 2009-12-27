#include <Core/Core.h>
#include <CtrlLib/CtrlLib.h>
#include "splash-sv.h"
//#include "PrintProject.h"


static String splashqtf;
static Image logoNative;
static Image logoInternational;

String native_lang;
Label* SplashStatusPtr;
Size SplashScreen(Ctrl& parent, Array<Ctrl>& ctrl)
{
	Image logo;
	if(ToLower(LNGAsText(GetSystemLNG()& 0xfffff))==native_lang)
		logo = logoNative;
	else
		logo = logoInternational;
	
	Size  isz = logo.GetSize();

	ImageCtrl& l = ctrl.Create<ImageCtrl>();
	l.SetImage(logo);
	Size sz = Size(isz.cx, isz.cy + 260);
	parent.Add(ctrl.Create<StaticRect>().Color(White).SizePos());
	parent.Add(l.TopPos(0, isz.cy).LeftPos(0, isz.cx));

	RichTextView& w = ctrl.Create<RichTextView>();
	SetLanguage(GetSystemLNG()& 0xfffff);
	
	Topic t;
	t = GetTopic(splashqtf+"$"+ ToLower(LNGAsText(GetSystemLNG()& 0xfffff)));
	if (t.text.IsEmpty()) {
		t = GetTopic(splashqtf+"$en-us");
	}
	w.SetQTF(t);
	w.PageWidth(3900);
	w.NoSb();
	w.SetFrame(NullFrame());
	parent.Add(w.TopPos(isz.cy , 240).LeftPos(0, isz.cx));
	
	Label& SplashStatus=ctrl.Create<Label>();
	SplashStatus.SetText(t_("Program Loading..."));
	SplashStatus.SetFont(Arial(12));
	SplashStatus.SetAlign(ALIGN_CENTER);
	SplashStatusPtr = &SplashStatus;
	parent.Add(SplashStatus.HCenterPos(380, 0).BottomPos(2, 20));
	
	
	return sz;
}
void ShowSplashStatus(String SplashStatusString){
	
	if(Single<Splash>().IsOpen()) {
		SplashStatusPtr->SetText(SplashStatusString);
		Cout() << SplashStatusString << "\n";
		Ctrl::ProcessEvents(); 
	}
}
void HideSplash()
{
	if(Single<Splash>().IsOpen())
		Single<Splash>().Close();
}

void ShowSplash()
{
	Single<Splash>().PopUp(NULL, false, false,false,true);
	SetTimeCallback (40000, callback(HideSplash));
	Ctrl::ProcessEvents(); 
}
void SetSplashTimer(int time)
{
	SetTimeCallback(time, callback(HideSplash));
}
void SetSplashQtf(String s) {
	splashqtf = s;
}
void SetSplashLogo(const Image& lruru,const Image& lenus) {
	logoNative = lruru;
	logoInternational = lenus;
}
void SetSplashLogo(const Image& logo) {
	logoNative = logo;
	logoInternational = logo;
}
void SetSplashNativeLang(String s){
	native_lang = s;
}
	

