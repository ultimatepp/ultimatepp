#ifndef _Vega_VegaMain_h_
#define _Vega_VegaMain_h_

#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>
#include <CtrlCore/CtrlCore.h>
#include <RichEdit/RichEdit.h>
#include "Tournament.h"

#define IMAGECLASS    Vega5Img
#define IMAGEFILE     <VegaTeam5/Vega.iml>
#include <Draw/iml_header.h>
//#include <Draw/iml.h>


class ScrollArea : public StaticRect {
public:
	ScrollBars sc;
	StaticRect work;
	void SetWorkSize(Size sz);
	virtual void Scroll();

	typedef ScrollArea CLASSNAME;
	ScrollArea();
	~ScrollArea() {;}
};


//put all used class declarations above *.lay
#define LAYOUTFILE     <VegaTeam5/Vega.lay>
#include <CtrlCore/lay.h>

class OptionImage : public Option {
	void ReFocus();
protected:
	Image imgYes, imgNo, imgMaybe;
public:
//	Callback WhenChange;
	virtual void   Paint(Draw& draw);
	OptionImage& SetImage(const Image& m1, const Image& m2, const Image& m3)
			            { imgYes = m1; imgNo = m2; imgMaybe = m3; Refresh(); return *this; }
	OptionImage& SetImage(const Image& m1, const Image& m2)
			            { imgYes = m1; imgNo = m2; Refresh(); return *this; }

	OptionImage& ThreeState(bool b = true)    { threestate = b; notnull = false; return *this; }

//	void OnChange() {WhenChange();}
	typedef OptionImage CLASSNAME;
	OptionImage();
	~OptionImage(){;}
};

class VegaConsoleFrame : public WithVegaConsoleFrameLayout<ParentCtrl> {
PlayerArchive play;
public:
    String filedbname; //database filename
    enum{ FNAME=0, FRATING, FTITLE, FKCOEFF, FCOUNTRY, FBIRTH, FIDCODE, FGENDER };
    int IMPORT_DB; //depend by the file type
    void EditSearchCB();
    void MoveItemCB();
    void RegisterPlayerCB( Vector<Value>& playerData );
    //	void RegisterPlayerCB();
	void ParsePlayerFSI(String player);
	void ParsePlayerFIDE(String playerstr);
	void ParsePlayerVEG(String playerstr);
	void ParsePlayerFixedLenghtDB(String playerstr);
	void ParsePlayerUSCFregular(String playerstr);
	void ParsePlayerUSCFquick(String playerstr);
	void SetFilterCB();
    void SetConsoleDatabse(String fn, int flagDB);
//	virtual bool Key(dword key, int count);

	typedef VegaConsoleFrame CLASSNAME;
	VegaConsoleFrame();
};

#define NUM_BTN 9
class NewTournamentDlg : public WithNewTournamentLayout<TopWindow> {
public:
    int flag;
    Option opt[NUM_BTN+1]; // warning!!!!!
	DropList dropRating;
	EditInt edtmin, edtmax;
	Label dv___30, dv___32, dv___34;
    void AddTyebreak( int i);
    void DoneDlg();
    void ExportDlg();
    void SetTournamentDir(String dir);
	typedef NewTournamentDlg CLASSNAME;

	NewTournamentDlg(int flag);  // flag = 1 for a new torneo, 0 to modify it
	~NewTournamentDlg() {;}
};

#define FIELDS 8
class SetFilterDlg : public WithSetFilterLayout<TopWindow> {
Option  tbutton[FIELDS];
EditInt edit[FIELDS];
Label   label[FIELDS];
public:
	void LoadFilterCB(); 		//driver of LoadFilter()
	void LoadFilter(String fn);
	void SaveFilterCB();
	void DoneFilterCB();
	void SetSelection(int i, int l);

	typedef SetFilterDlg CLASSNAME;
	SetFilterDlg(String& testdb);
	~SetFilterDlg(){;}
};


class NonStandardResultDlg : public WithNonStandardResult<TopWindow> {
int round;
public:
	void DoneNonStandardCB(int idw, int idb);
	void CloseCB();

	typedef NonStandardResultDlg CLASSNAME;
	NonStandardResultDlg(int idw, int idb, int rnd);
	~NonStandardResultDlg(){;}
};

class PrintTableDlg : public WithPrintTable<TopWindow> {
Option opt[13];
public:
	ArrayCtrl *arr;
	bool printed;
	void PrintDlgCB();
	void PrintPartecipantsHTML();
	typedef PrintTableDlg CLASSNAME;
	PrintTableDlg(ArrayCtrl *array);
	~PrintTableDlg(){;}
};

class VegaTab1 : public WithVegaTab1Layout<ParentCtrl> {
	EditString optGenderP,edtNameP, edtCountryP, edtBirth;
	DropList edtTitleP;
	EditInt edtIDFideP, edtRtgFideP, edtIDNatP, edtRtgNatP, edtKP;
    EditInt  optStatusP;

//    ButtonOption optGenderP, optStatusP;
public:
    int countID;
    void SortRecord(int column);
	void AddPlayer(int flag);
	int  GetAvail();
	void MaskDefaultValue();
	void FillFieldsFromRow();
	void PromptCell();
	void ModifyCrossTableCB();
	void RemovePlayer();

	typedef VegaTab1 CLASSNAME;
	VegaTab1();
	~VegaTab1(){;}
};

class ModifyCrossTableDlg : public WithModifyCrossTable<TopWindow> {
int id;
public:
	void DoneCB();
    void FillEditCB();

	typedef ModifyCrossTableDlg CLASSNAME;
	ModifyCrossTableDlg(int idplayer);
	~ModifyCrossTableDlg(){;}
};

enum {WY=0, WN, BY, BN, NG};
class ColoredRow : public WithRowLayout<ParentCtrl> {
public:
    int wid, bid, board;
    int wrtg, brtg;
    float wscr, bscr;
    int wflag, bflag;
	typedef ColoredRow CLASSNAME;

	ColoredRow();
	void Set(int board, int wid, int bid, int wrtg, int brtg, float wscr, float bscr, int wflag, int bflag);
	void Get(int& wid, int& bid, int& wrtg, int& brtg);
	void SetSide(int side, int id, int rtg, float scr, int flag);
	void GetSide(int side, int& id, int& rtg, float& scr, int& flag);
};

class Abbinamento : public WithAbbinamento<TopWindow> {
	ColoredRow a[501];
//	ScrollArea scrollarea;
    int lastlabel, numlabel;
    int id1, id2; // remind the two ids has been selected
    int iboard[2], side[2]; // remind the board (1...npair) and the side(0=left, 1=right)
	void FillEdit(int board, int id);
    void CloseCB();
    bool ShowInfo();
    void VerifyPairCB();
    void ExchangePlayersCB();
    void DoneCB();
public:
	typedef Abbinamento CLASSNAME;
	Abbinamento();
};

class ShowVerboseDlg : public TopWindow {
public:
    Splitter h, v;
    LineEdit a, b;
	typedef ShowVerboseDlg CLASSNAME;
	ShowVerboseDlg(int round);
	~ShowVerboseDlg(){;}
};

class VegaTab3 : public WithVegaTab3Layout<ParentCtrl> {
	void UpdateData(bool flag);
	void HideTab(ArrayCtrl *x);
//	bool Key(dword key, int count);
    void StartArrayCursor();
	void MoveNextPair( int position);
public:
	int round;
	WithTabPairing<ParentCtrl> tabpair;
	WithTabResult<ParentCtrl> tabinsert;

	typedef VegaTab3 CLASSNAME;
	VegaTab3();
	~VegaTab3(){;}
	void ShowPairingCB();
	void UpdateLabelResultCB();
	void SpinRoundCB();
	void BtnResultCB(int idresult);
	void BtnOption();
	void SetSpinRound(int r);
	void ShowVerboseCB();
    void SetOptionCB(int flag);
	void LoadResultCB();
  	void SwapColorsCB();
	int  InvertResult(int res);
    void ImproveColorsCB();
};


class ManualPairingDlg : public WithManualPairing<TopWindow> {
	int npair, remainedplayer, flagcall;
	int * availplayer; // player to be paired
public:
    void AddPairCB();
    void VerifyPairCB();
    void LoadPairingCB(int flag);
	void SavePairingCB();
	void RemovePairCB();
	void DoneCB();
	void SelectionListAvalCB();

	typedef ManualPairingDlg CLASSNAME;
	ManualPairingDlg(int flagcall);
	~ManualPairingDlg(){;}
};

class VegaTab4 : public WithVegaTab4Layout<ParentCtrl> {
ToolBar toolbar;
FontHeight edt;
EditString edts;
public:
String  filename;
	typedef VegaTab4 CLASSNAME;
	VegaTab4();
	~VegaTab4(){;}
	void ShowFile(int idfile);
	void MainBar(Bar& bar);
	void New();
	void Load(const String& name);
	void Open();
	void Save();
	void SaveAs();
	void PrintCB();
	void PrintSelectedTextCB();
	void NewQtf();

};


class UWord : public TopWindow {
protected:
	RichEdit   editor;
	MenuBar    menubar;
	ToolBar    toolbar;
	StatusBar  statusbar;
	String     filename;

	void Load(const String& filename);
	void New();
	void Open();
	void Save();
	void SaveAs();
	void Print();
	void Pdf();
	void About();
	void Destroy();
	void SetBar();
	void FileBar(Bar& bar);
	void AboutMenu(Bar& bar);
	void MainMenu(Bar& bar);
	void MainBar(Bar& bar);

public:
	typedef UWord CLASSNAME;

	UWord();
};

//Section
class USCFSectionReport : public WithUSCFRatingReport<TopWindow> {
public:
	typedef USCFSectionReport CLASSNAME;
	USCFSectionReport();
	~USCFSectionReport(){;}
	void DoneCB();
};


//do USCF Tournament report
class USCFTournamentReport : public WithUSCFTournamentReport<TopWindow> {
String filesection[20];
int Ndirectory, max_rounds;
public:
	typedef USCFTournamentReport CLASSNAME;
	USCFTournamentReport(int SectionNumber, int max_rounds, String listfile[]);
	~USCFTournamentReport(){;}
	void DoneCB();
};


//collect sections
class USCFReport : public WithUSCFReport<TopWindow> {
int Ndirectory, max_rounds;
public:
	typedef USCFReport CLASSNAME;
	USCFReport();
	~USCFReport(){;}
	void OpenFileCB();
	void SaveFileCB();
	void RemoveCB();
	void AddCB();
	void DoneCB();
};


class FIDERatingReport : public WithFIDERatingReport<TopWindow> {
public:
	typedef FIDERatingReport CLASSNAME;
	FIDERatingReport();
	~FIDERatingReport(){;}
	void DoneCB();
};


class FIDECertificate : public WithFIDECertificate<TopWindow> {
public:
	typedef FIDECertificate CLASSNAME;
	FIDECertificate();
	~FIDECertificate(){;}
	void CloseCB();
	void DoneCB();
};

#define GROUPS 5
class RankRatedGroup : public WithRankRatedGroup<TopWindow> {
EditInt editgroup[GROUPS+1][2];
int    limitgroup[GROUPS+1][2];
public:
	typedef RankRatedGroup CLASSNAME;
	RankRatedGroup();
	~RankRatedGroup(){;}
	void CloseCB();
	void DoneCB();
	void SaveRankRatedGroup();
};


class RankDateGroup : public WithRankDateGroup<TopWindow> {
EditInt editgroup[GROUPS+1][2];
int    limitgroup[GROUPS+1][2];
public:
	typedef RankDateGroup CLASSNAME;
	RankDateGroup();
	~RankDateGroup(){;}
	void CloseCB();
	void DoneCB();
	void SaveRankDateGroup();
};


class VegaMain : public WithVegaMainLayout<TopWindow> {
//these are protected to access then from app instance...
	void DataFromArchiveToArray(int fromPlayer, int toPlayer);
    void DataFromArchiveToArray();
    void DataFromArchiveToArray(int board, Vector<Value>& v);
	void DataFromArrayToArchive();
    void ReadVegFile(String filename);
protected:
	TabCtrl tabs;
	VegaTab1 tab1; //may change the name for better one...
	VegaTab3 tab3; //may change the name for better one...

	VegaConsoleFrame consoleFrame;

public:
	void VegaMainMenu(Bar& menu);
	void File(Bar& bar);
	void Export(Bar& bar);
	void NationalReport(Bar& bar);

	void NewTournament();
    void ExportTournament();
    void ExportAllPlayers();
    void ExportPlayer();
	void OpenTournament();
	void SaveTournament();
	void ConnectDatabase(int i);
    void SelectDbase(Bar& bar);
	void CloseRegistration();
	void AssignIDtoPlayers(int n);

	void Extras(Bar& bar);  //menu Extra
	void FideReport(Bar& bar);   // submenu FideReport
	void FindNorm();
	void FideEloVariation();
	void FIDERatingReportCB();
	void Report(Bar& bar);
	void FindDuplicate();
	void FidePlayerCard();
	void ShowStatistics();
    void PrintFIDECertificateCB();
	void FSIReportCB();
	void USCFReportMenu(Bar& bar);
	void USCFSectionReportCB();
	void USCFTournamentReportCB();
	void SortCrossTable();
	void RankDateGroupCB();
	void RankRatedGroupCB();
	void RoundRobinCalendar();
	void SelectLanguage(Bar& bar);
	void ChooseLng(int lng);
	void MakeSBadge();
	void MPScoreSystem();
	void BackupVegFile();
	void DBConversion();
	void PrintReceipt();

	void DangerousStuff(Bar& bar); // submenu DangerousStuff
	void ModifyTournament();
    void LoadBackupRoundCB();
    void ResetTournamentCB();
    void ReopenRegistrationCB();
    void DeleteRoundCB();

    void Help(Bar& bar);           // menu Help
	void HelpContent();
	void HelpRegistered();
	void HelpAbout();

	void SetBars();
	void SetTabs();
	void DopairingCB();
	void DoManualPairingCB();
	void ModifyPairingCB();
	void PrintTable();
	void PrintPairingCB();
	void Init();

	void UpdateInfo(const String& _who, const String& _action, const String& _what);
	String InfoCurrentPlayerAndStatus();
    void ThreeButtonAction();
    void arrCtrlsAction();
	void btnAddAction();
	void btnModifyAction();
    void btnRemoveAction();
	void ShowStatusBar();
	void AddImportedPlayer();
	void Exit();
	virtual bool Key(dword key, int count);

public:
	typedef VegaMain CLASSNAME;
	VegaMain();
};


#endif
