#ifndef _bigmailer_bigmailer_h
#define _bigmailer_bigmailer_h

#include <CtrlLib/CtrlLib.h>
#include <RichEdit/RichEdit.h>
//#include <nmpTools/nmpTools.h>
#include <GridCtrl/GridCtrl.h>
#include <bigmailer/cfg.h>

#define LAYOUTFILE <bigmailer/bigmailer.lay>
#include <CtrlCore/lay.h>

#include <bigmailer/messages.h>


// Configuración
class cfgDlg : public WithCfgLayout<TopWindow> {
public:

	typedef cfgDlg CLASSNAME;

	cfgDlg();
	virtual ~cfgDlg();

	void onBtOK() ;

};

class bigmailer : public TopWindow { //public WithBigMailerLayout<TopWindow> {
	MenuBar mainMenu ;
	TabCtrl tab ;

	//MenuBar mainMenu ;
	void mnuMailAddr( Bar &bar ) ;
	void mnuMsg( Bar &bar ) ;
	void mnuSend( Bar &bar ) ;
	void mnuHelp( Bar &bar ) ;


	void onNewList() ;
	void onOpenList() ;
	void onCloseList() ;
	void onPrintList() ;
	void onExit() ;

	void onNewMsg() ; void onStart() ;
	void onOpenMsg() ;
	void onCloseMsg() ;
	void onPrintMsg() ;

	void onSend() ;

	void onPreferences() ;
	void onAbout() ;

	int lastTabIndex ;
	void onTabSet() ;


	// Páginas
	WithListDataLayout<ParentCtrl> dataPage ;
		EditString dataPage_theEdits[30], dataPage_editError ;
		void DataPageFill() ;
		void DataPageSave() ;
		void DataPageTheGridModification() ;
		void DataPageEditFields() ;
		void DataPageImportCSV() ;

	MessageEditor messagePage ;



	static bigmailer *mailer ;
public:
	static bigmailer& Mailer() { return *mailer ; }
	void SetTitle() ;

	typedef bigmailer CLASSNAME;
	bigmailer();
};

#endif
