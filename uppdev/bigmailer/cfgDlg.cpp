#include <bigmailer/bigmailer.h>
#include <sTools/sTools.h>


void bigmailer::onPreferences()
{
	cfgDlg c ;
	c.Execute() ;
}



cfgDlg::cfgDlg()
{
	CtrlLayout(*this, t_("Preferences") );
	btOK.WhenAction = THISBACK(onBtOK) ;

	edServer.SetData( theCfg.smtpServer ) ;
	edUser.SetData( theCfg.user ) ;
	edPass.SetData( theCfg.pass ) ;

	edBlackList.SetData( VectorString2String(theCfg.blackList) ) ;


}
cfgDlg::~cfgDlg()
{
}

void cfgDlg::onBtOK()
{
	theCfg.smtpServer = ~edServer ;
	theCfg.user = ~edUser ;
	theCfg.pass = ~edPass ;

	theCfg.blackList = String2VectorString(~edBlackList) ;
	theCfg.Save() ;

	Close() ;
}

/*
*/
