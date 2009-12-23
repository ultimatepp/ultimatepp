#include "Controls4U_Demo.h"

#define IMAGEFILE <Controls4U_Demo/Controls4U_Demo.iml>
#define IMAGECLASS Images
#include <Draw/iml.h>

Controls4U_Demo::Controls4U_Demo() {
	CtrlLayout(*this, "Controls4U Demo");
	Sizeable().Zoomable();
	
	tab.Add(meter_Demo.SizePos(), "Meter");
	tab.Add(staticClock_Demo.SizePos(), "StaticClock");
	tab.Add(editFileFolder_Demo.SizePos(), "StaticImage & EditFile/Folder");
	tab.Add(staticCtrls_Demo.SizePos(), "Static Controls");
	tab.Add(staticCtrlsTest_Demo.SizePos(), "Static Controls Test");
	
	timerOn = false;
	SetTimeCallback(-100, THISBACK(Timer));
}
void Controls4U_Demo::Timer() {
	if (timerOn)
		return;
	timerOn = true;
	staticClock_Demo.Update();
	timerOn = false;
}
GUI_APP_MAIN {
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

void StaticClock_Demo::Update() {	
	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext()) {
		if (StaticClock *c = dynamic_cast<StaticClock *>(q))
			if (c != &clock10)
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
