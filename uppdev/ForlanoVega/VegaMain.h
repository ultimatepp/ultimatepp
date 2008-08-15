#ifndef _ForlanoVega_VegaMain_h
#define _ForlanoVega_Vega_Main_h

#include <CtrlLib/CtrlLib.h>

//this is the only file you include manually...
#include <ForlanoVega/OptionImage.h>
//#include <ForlanoVega/TwoCtrl.h>


#define IMAGECLASS    VegaImg
#define IMAGEFILE <ForlanoVega/Vega.iml>
#include <Draw/iml_header.h>

//aris I've got rid off that kind of status...

//put all used class declarations above *.lay
#define LAYOUTFILE <ForlanoVega/Vega.lay>
#include <CtrlCore/lay.h>


class VegaTabs : public WithVegaTabsLayout<TopWindow> {
public:

	typedef VegaTabs CLASSNAME;

	VegaTabs();
	~VegaTabs(){;}
};

class VegaConsoleFrame : public WithVegaConsoleFrameLayout<TopWindow> {
public:

	typedef VegaConsoleFrame CLASSNAME;

	VegaConsoleFrame();
};


//these 4 below are auto generated from the designer...
class VegaTab1 : public WithVegaTab1Layout<TopWindow> {

public:
	void SortRecord(int column);
	void AddPlayer();
//	void ShowNumberRecord(); you don't need it here...
	int GetAvail();
	void ModifyPlayer();
	void MaskDefaultValue();
	void FillFieldsFromRow();
	void PromptCell();

	typedef VegaTab1 CLASSNAME;

	VegaTab1();
	~VegaTab1(){;}
};

class VegaTab2 : public WithVegaTab2Layout<TopWindow> {
public:

	typedef VegaTab2 CLASSNAME;

	VegaTab2();
	~VegaTab2(){;}
};


class VegaTab3 : public WithVegaTab3Layout<TopWindow> {
public:

	typedef VegaTab3 CLASSNAME;

	VegaTab3();
	~VegaTab3(){;}
};


class VegaTab4 : public WithVegaTab4Layout<TopWindow> {
public:

	typedef VegaTab4 CLASSNAME;

	VegaTab4();
	~VegaTab4(){;}
};


class VegaMain : public WithVegaMainLayout<TopWindow> {
//these are protected to access then from app instance...
protected:
	TabCtrl tabs;
	VegaTab1 tab1; //may change the name for better one...
	VegaTab2 tab2; //may change the name for better one...
	VegaTab3 tab3; //may change the name for better one...
	VegaTab4 tab4; //may change the name for better one...

	VegaConsoleFrame consoleFrame;
//	ArrayCtrl console;
public:
//all these declarations are copied with Ctrl_A and alt_C in "one go"...
	void VegaMainMenu(Bar& menu);
	void File(Bar& bar);
	void Export(Bar& bar);
	void ConnectDatabase(Bar& bar);
	void FideReport(Bar& bar);
	void NationalReport(Bar& bar);
//	void PlayerList(Bar& bar);
	void Help(Bar& bar);

	void NewTournament();
	void OpenTournament();
	void SaveTournament();

	void SortCrossTable();
	void RankDateGroup();
	void RankRatedGroup();
	void RoundRobinCalendar();

	void MPScoreSystem();
	void BackupVegFile();
	void SelectLanguage();
	void DBConversion(Bar& bar);
	void DangerousStuff(Bar& bar);

	void HelpContent();
	void HelpRegistered();
	void HelpAbout();

	void Extras(Bar& bar);
	void ShowNumber(int n);

	void SetBars();
	void SetTabs();
	void Init();
	//later this must be moved and optimized... Vector?...
//		void UpdateFocusAndInfo(const String& _who, const String& _what);

	void UpdateInfo(const String& _who, const String& _action, const String& _what);
	String InfoCurrentPlayerAndStatus();
//	void AddPlayerAndUpdateInfo(const String& _who, const String& _what);
	void arrCtrlsAction();
	void btnAddAction();

	void Exit();

//	void UpdateInfo1(String s)  { info1 = s;}
//	void UpdateInfo2(String s)  { info2 = s;}

public:
	typedef VegaMain CLASSNAME;
	VegaMain();
};

#endif

	void VegaMainMenu(Bar& menu);
	void File(Bar& bar);
	void Export(Bar& bar);
	void ConnectDatabase(Bar& bar);
	void FideReport(Bar& bar);
	void NationalReport(Bar& bar);
//	void PlayerList(Bar& bar);
	void Help(Bar& bar);
	void NewTournament();
	void OpenTournament();
	void SaveTournament();
	void SortCrossTable();
	void RankDateGroup();
	void RankRatedGroup();
	void RoundRobinCalendar();
	void MPScoreSystem();
	void BackupVegFile();
	void SelectLanguage();
	void DBConversion(Bar& bar);
	void DangerousStuff(Bar& bar);
	void HelpContent();
	void HelpRegistered();
	void HelpAbout();
	void Extras(Bar& bar);
	void ShowNumber(int n);
	void SetBars();
	void Exit();
	void SetTabs();
	void UpdateInfo(const String& _who, const String& _action, const String& _what);
	String InfoCurrentPlayerAndStatus();
	void AddPlayerAndUpdateInfo(const String& _who, const String& _what);
	void arrCtrlsAction();
	void btnAddAction();
	void Init();
