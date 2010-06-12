#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include "Controls4U/Controls4U.h"
#include "Functions4U/Functions4U.h"


#if defined(PLATFORM_WIN32) 	
#include "Controls4U_Demo_win.h"
#endif
#include "Controls4U_Demo.h"

#define IMAGEFILE <Controls4U_Demo/Controls4U_Demo.iml>
#define IMAGECLASS Images
#include <Draw/iml.h>

Controls4U_Demo::Controls4U_Demo() {
	CtrlLayout(*this, "Controls4U Demo");
	Sizeable().Zoomable();

#if defined(PLATFORM_WIN32) 	
	tab.Add(vlc_Demo.SizePos(), "VLC ActiveX");
	tab.Add(firefox_Demo.SizePos(), "Firefox ActiveX");
	tab.Add(iexplorer_Demo.SizePos(), "Internet Explorer ActiveX");
#endif
//tab.Add(fileBrowser_Demo.SizePos(), "FileBrowser Text");	
	tab.Add(meter_Demo.SizePos(), "Meter");
	tab.Add(staticClock_Demo.SizePos(), "StaticClock");
	tab.Add(editFileFolder_Demo.SizePos(), "StaticImage & EditFile/Folder");
	tab.Add(staticCtrls_Demo.SizePos(), "Static Controls");
	tab.Add(staticCtrlsTest_Demo.SizePos(), "Static Controls Test");
	tab.Add(miscellaneous_Demo.SizePos(), "Miscellaneous functions");

	timerOn = false;
	SetTimeCallback(-100, THISBACK(Timer));
}
void Controls4U_Demo::Timer() {
	if (timerOn)
		return;
	timerOn = true;
	staticClock_Demo.UpdateInfo();
	timerOn = false;
#if defined(PLATFORM_WIN32)
	firefox_Demo.UpdateInfo();
	iexplorer_Demo.UpdateInfo();
	vlc_Demo.UpdateInfo();
#endif	
}

String GetTrashBinDirectory();

GUI_APP_MAIN {
	/*GetTrashBinDirectory();
	return;*/
	Controls4U_Demo().Run();
}

EditFileFolder_Demo::EditFileFolder_Demo() {
	CtrlLayout(*this);

	FileName.ActiveDir(GetDesktopFolder());
	FileName.Type("Image files", "*.png, *.jpg, *.jpeg, *.tiff, *.bmp, *.gif");
	FileName.AllFilesType();
	FileName.WhenChange = THISBACK(OnNewFile);
	angleList.Add(0, "0º").Add(1, "90º").Add(2, "180º").Add(3, "270º").SetData(0);
	angleList.WhenAction = THISBACK(ChangeProperties);
	imageFit.Add(0, "BestFit").Add(1, "FillFrame").Add(2, "NoScale").Add(3, "RepeatToFill").SetData(0);
	imageFit.WhenAction = THISBACK(ChangeProperties);
	
	back.Set(Images::paper());
}
void EditFileFolder_Demo::OnNewFile() {
	if (!clipImage.Set(~FileName))
		Exclamation("File not found");
}
void EditFileFolder_Demo::ChangeProperties() {
	clipImage.SetAngle(~angleList);
	clipImage.SetFit(~imageFit);
}

StaticCtrls_Demo::StaticCtrls_Demo() {
	CtrlLayout(*this);
	
	back.Set(Images::paper());
}
StaticCtrlsTest_Demo::StaticCtrlsTest_Demo() {
	CtrlLayout(*this);
}

void StaticClock_Demo::UpdateInfo() {	
	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext()) {
		if (StaticClock *c = dynamic_cast<StaticClock *>(q))
			if (!c->IsAuto())
				c->SetTime();
	}
}
void StaticClock_Demo::ChangeProperties() {
	clock10.SetHourType(~hourType);
	clock10.SetNumberType(~numberType);
	clock10.SetColorType(~colorType);
	clock10.Seconds(checkSeconds);
	if (checkImage)
		clock10.SetImage(Images::ClockImage());
	else
		clock10.SetImage((Image)Null);
}

StaticClock_Demo::StaticClock_Demo() {
	CtrlLayout(*this);
	clock9.SetImage(Images::ClockImage());
	hourType.Add(0, "No").Add(1, "Square").Add(2, "Rectangle").SetData(2);
	hourType.WhenAction = THISBACK(ChangeProperties);
	numberType.Add(0, "NoNumber").Add(1, "Small").Add(2, "Big").Add(3, "BigSmall").Add(4, "Big4").SetData(0);
	numberType.WhenAction = THISBACK(ChangeProperties);
	colorType.Add(0, "WhiteType").Add(1, "BlackType").SetData(1);
	colorType.WhenAction = THISBACK(ChangeProperties);
	checkSeconds = true;
	checkSeconds.WhenAction = THISBACK(ChangeProperties);
	checkImage = false;
	checkImage.WhenAction = THISBACK(ChangeProperties);
	back.Set(Images::cream2());
};

void Meter_Demo::ChangeValue() {
	int val = slider.GetData();
	meter1.SetData(val);
	meter2.SetData(val);
	meter3.SetData(val);
	meter4.SetData(val);
	meter5.SetData(val);		
}

void Meter_Demo::ChangeProperties() {
	meter1.SetColorType(~colorType);
	meter1.SetNumber(checkNumber);
}

Meter_Demo::Meter_Demo() {
	CtrlLayout(*this);
	slider.MinMax(100, 0);
	slider.SetData(0);
	slider.WhenSlideFinish = THISBACK(ChangeValue);
	colorType.Add(0, "WhiteType").Add(1, "BlackType").SetData(1);
	colorType.WhenAction = THISBACK(ChangeProperties);	
	checkNumber.WhenAction = THISBACK(ChangeProperties);
	checkNumber = true;
	back.Set(Images::cream2());
}

FileBrowser_Demo::FileBrowser_Demo() {
	CtrlLayout(*this);

	browser.SetFlags(USE_TRASH_BIN | BROWSE_LINKS | DELETE_READ_ONLY);
	back.Set(Images::paper());
}

void FileBrowser_Demo::OnNewFile() {
	if (!clipImage.Set(~browser))
		Exclamation("File not found");
}

void FileBrowser_Demo::ChangeProperties() {
}

Miscellaneous_Demo::Miscellaneous_Demo() {
	CtrlLayout(*this);
	
	String myqtf;

	QtfRichObject a = QtfEquation("(2/3)");
	QtfRichObject b = QtfEquation("integral(sqrt(cos(phi_ini^2)) + i^2 + 6, i = 1, 10)*dx = cos((27+x^2)^3.25)/(PI*R_0^2)");
	QtfRichObject c = QtfEquation("delta_i = a+b*x+c*x^2+d*x^3");
	QtfRichObject d = QtfEquation("sqrt(cos(phi_ini^2))");
	QtfRichObject e = QtfEquation("summation(a+b*x+c*x^2+d*x^3, x = h, h+1)*dx = SI_h");
	
	myqtf << "[ {{10000 [s0;= This feature is experimental]}}]&[R3 This are some formulas in QTF:&" << a << "&" << b << "&" << c << "&" << d << "&" << e;

	equation.SetData(myqtf);

	butDiff.WhenAction = THISBACK(OnDiff);	
	butPatch.WhenAction = THISBACK(OnPatch);
}

void Miscellaneous_Demo::OnDiff() {
	if (!BSDiff(~editOriginal, ~editNew, ~editPatch)) 
		Exclamation(DeQtf(BsGetLastError()));
}

void Miscellaneous_Demo::OnPatch() {
	if (!BSPatch(~editOriginal, ~editNew, ~editPatch)) 
		Exclamation(DeQtf(BsGetLastError()));
}
