#include <CtrlLib/CtrlLib.h>

class j2 : public Withj2Layout<TopWindow> {
public:
	typedef j2 CLASSNAME;
	j2();
};

j2 HG;
String qtfx;

j2::j2() {
	CtrlLayout(*this, "Joachim 0.1");
	SetLanguage( LNG_('D','E','D','E') );
	SetDefaultCharset(CHARSET_UTF8);
}

void textin_cb(int x,String farbe,String Sender,String text) {
	qtfx<<"\1"<<Sender<<":"<<text<<"\1]&";
	HG.e.SetQTF(qtfx);
	HG.e.ScrollEnd();
}


GUI_APP_MAIN
{
	readinifile();
	int ids = 1;
	pthread_t threads;
	pfi = &ParseChatLine;
	pthread_create (&threads, NULL, readchat, &ids);
	HG.Run();
}

GUI_APP_MAIN
{
}

