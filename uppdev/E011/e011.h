#ifndef _E011_E011_h
#define _E011_E011_h

#include <Core/Core.h>
#include <CtrlLib/CtrlLib.h>
//#include <CtrlCore/CtrlCore.h>
#include <Topic/Topic.h>

// immagini
#define IMAGECLASS AppImg
#define IMAGEFILE <E011/e011.iml>
#include <Draw/iml.h>

// keys
#define KEYGROUPNAME "E011"
#define KEYNAMESPACE E011Keys
#define KEYFILE      <E011/e011.key>
#include             <CtrlLib/key_header.h>

using namespace E011Keys;

#include <src/support/support.h>
#include <src/Comm/Comport.hpp>
#include <E011/help/help.h>
#include <Display/Display.h>
#include <E011/utils.h>
#include "release.h"
#include "ascii.h"
#include "console.h"
#include "_treeCtrl.h"
#include "sep1.h"
#include "/Lavori/LavoriELCA/picc/E011-V14/common.h"

#define		I5									0.00092

// layout
#define LAYOUTFILE <E011/e011.lay>
#include <CtrlCore/lay.h>

extern	HelpWin	help;
extern	unsigned int id, unlockNumber;
extern	Vector<String> recents, paths;
extern	String lastComPort;
extern	bool fullRelease, hardwareFound;
extern	COMPort	port;

Array<EditString> es;
Array<EditInt> ei;

struct DisplayOnline : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword) const {
		w.DrawRect(r, SLtGray);
		Image m = AppImg::online_16();
		w.DrawImage(r.CenterRect(m.GetSize()), m);
	}
};

struct DisplayOnline_1 : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword) const {
		w.DrawRect(r, SLtGray);
		Image m = AppImg::online1_16();
		w.DrawImage(r.CenterRect(m.GetSize()), m);
	}
};

struct DisplayOnline_2 : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword) const {
		w.DrawRect(r, SLtGray);
		Image m = AppImg::online2_16();
		w.DrawImage(r.CenterRect(m.GetSize()), m);
	}
};

struct DisplayOnline_3 : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword) const {
		w.DrawRect(r, SLtGray);
		Image m = AppImg::online3_16();
		w.DrawImage(r.CenterRect(m.GetSize()), m);
	}
};

class App : public WithmainForm<TopWindow> {
public:
	bool       								hardwareFound, workOpen;
	MenuBar     							menu;
	ToolBar     							toolbar;
	String										hardware;
	int												verA, verB, mode, icon_step;
	char											verV;
	FileSelector 							fs;
	WithMapLay<StaticRect> 		mapForm;
	WithConLay<StaticRect> 		conForm;
	Splitter									leftFrame, bottomFrame, mainFrame;
	TreeCtrl									tree;
	Console										console;
	InfoCtrl									status;

	struct Config {
		int							vref;
		int							i_doppler;
		int							vu;
		int							ng;
	} conf;

	struct Mappa {
		String					descrizione;
		int							vu, ng, vref;
		Vector<int>			dati;
		Vector<int>			resistenza;
		int							version;
		char						variant;
		int							i_doppler;

		void Serialize(Stream& s) {
			s % descrizione % vu % ng % dati % resistenza % version % variant;

			if (version >= 258) {
				s % vref;
			}
			if (version >= 260) {
				s % i_doppler;
			}
		}
	} mappa;

  void Serialize(Stream& s);
  void ComChange();
  void ComConfig();
  void Quit();
  bool TestHardware(void);
  bool TestHardwareType(void);
  void GoOnline(void);
  void Animation();
  void AnimatedOnlineIcon();
  void OpenFile(void);
  void NewFile(void);
  void CloseFile(void);
  void DeleteFile(void);
  void SaveFile(void);
  void DownloadMap(void);
  void UploadMap(void);
  void VerifyMap();
  void EvaluateData(void);
  void MakeMap(void);
  void LoadConfForm(void);
  void CheckVref(void);

	// menu.cpp
  void MainMenu(Bar& bar);
  void FileMenu(Bar& bar);
  void OptionMenu(Bar& bar);
  void CommMenu(Bar& bar);
	void helpMenu(Bar& menu);
	void toolsBar(Bar& bar);
 	void setToolsBar();

  void HelpRmax(void);
  void HelpVolume(void);
  void HelpGradini(void);

  // tree
  void InitTree();
  void TreeCursor();

	// help.cpp
 	void About();

 	// paths.cpp
 	void SetupPath();

	typedef App CLASSNAME;

public:
  App();
};

class RealTimeD : public WithdisplayLay<TopWindow> {
public:
	void Perform();
	void RealTime();

	typedef RealTimeD CLASSNAME;

public:
	RealTimeD();
	~RealTimeD()																							{ KillTimeCallback(); }
};

void DirSel(EditField& f, FrameRight<Button>& b);
static void sSetFolder(EditField *f);

#define IMAGEFILE <E011/e011.iml>
#include <Draw/iml_source.h>

#endif
