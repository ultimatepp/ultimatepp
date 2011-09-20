#include <CtrlLib/CtrlLib.h>
#include <VLCPlayer/VLCPlayer.h>

#define LAYOUTFILE <VLCPlayerTest/main.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS MainImg
#define IMAGEFILE <VLCPlayerTest/main.iml>
#include <Draw/iml.h>


class MainLayDlg : public WithMainLay<TopWindow> {
	typedef MainLayDlg CLASSNAME;

public:
	Mutex mtx;
	MainLayDlg();
	void ClosePl();
	virtual void Close();
	
	void TestClear();
	void Layout();
};

void MainLayDlg::Layout() {
	LOG("======vvv Layout");
	Size ws = GetSize()/2;
	ws.cy = ws.cx / 16 * 9;
	player.SetVSize(ws);
	if (AtomicRead(player.playeractive)!=1) {
		INTERLOCKED_(mtx) { 
			
			
			if (ws.cx > 30) player.HCenterPos( ws.cx,0);
			DUMP(ws);
			player.TopPos(0,ws.cy);
		}
	}
	LOG("======^^^ Layout");
}


MainLayDlg::MainLayDlg()
{
	CtrlLayout(*this, "Window");
	//Sizeable();
	//Zoomable();
	MaximizeBox();
	Maximize();
	FullScreen();


	static Button::Style bs = Button::StyleNormal();
	bs.look[0] = MainImg::button();
	bs.look[1] = MainImg::button1();
	bs.look[2] = MainImg::button2();
	
	
	b1.SetStyle(bs);
	b2.SetStyle(bs);
	b3.SetStyle(bs);
	b4.SetStyle(bs);
	b5.SetStyle(bs);
	b6.SetStyle(bs);
	b7.SetStyle(bs);
	b8.SetStyle(bs);
	b9.SetStyle(bs);
	b10.SetStyle(bs);
	b11.SetStyle(bs);
	b12.SetStyle(bs);
	b9.WhenPush = THISBACK(TestClear);
}
void MainLayDlg::ClosePl(){
	player.Play();
	
}
void MainLayDlg::Close(){
	player.Close();
	Break();
	
	
}


void MainLayDlg::TestClear(){
	player.ClearPlayList();
	player.AddPlayList(GetHomeDirFile("Видео/X-M/09-moroz5.avi"));
//	player.AddPlayList(GetHomeDirFile("Видео/Фильмы/Hunting__(2009-09).avi"));
//	player.AddPlayList(GetHomeDirFile("Видео/Фильмы/Varshavyanka__(2009-09).avi"));
	player.Play();
}
GUI_APP_MAIN {
	MainLayDlg app;
	app.OpenMain();
	app.player.AddPlayList(GetHomeDirFile("Видео/Avaria-Leto vsegda1-5.avi"));
	
	app.Run();
}
