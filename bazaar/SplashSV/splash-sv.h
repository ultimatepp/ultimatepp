using namespace Upp ;

Size SplashScreen(Ctrl& parent, Array<Ctrl>& ctrl);
struct Splash : Ctrl {
	Array<Ctrl> ctrl;

	Splash() {
		SetRect(GetWorkArea().CenterRect(SplashScreen(*this, ctrl) + 2));
		SetFrame(BlackFrame());
	}
};


void SetSplashQtf(String s);
void SetSplashLogo(const Image& lruru,const Image& lenus);
void SetSplashLogo(const Image& logo);
void ShowSplashStatus(String SplashStatusString);
void HideSplash();
void ShowSplash();
void SetSplashTimer(int time);
void SetSplashNativeLang(String s);

#define SplashInit() \
	SetSplashQtf((String)(SPLASHQTF));\
	SetSplashLogo(SPLASH_LOGO_NATIVE,SPLASH_LOGO_INTERNATIONAL);\
	SetSplashNativeLang(SPLASH_NATIVE_LANG);\
	ShowSplash();
	

