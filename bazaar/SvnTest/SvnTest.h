#ifndef _SvnTest_SvnTest_h
#define _SvnTest_SvnTest_h

#include <CtrlLib/CtrlLib.h>

#include <SvnLib/SvnLib.h>

using namespace Upp;

#define LAYOUTFILE <SvnTest/SvnTest.lay>
#include <CtrlCore/lay.h>

class SvnTest : public WithSvnTestLayout<TopWindow>
{
	// test repository path and local path
	String DestPath;
	String RepositoryUrl;
	
	// svn object
	Svn svn;
	
	// button handlers
	void Checkout(void);
	void Connect(void);
	void Disconnect(void);
	void Modify(void);
	void GetLog(void);
	void GetStatus(void);
	void ListFiles(void);
	void WipeFolder(void);
	void Quit(void);
	
	// error messagebox
	void ShowError(void);
	void ShowError(String const &msg);

	public:
		typedef SvnTest CLASSNAME;

		SvnTest();
};

#endif

