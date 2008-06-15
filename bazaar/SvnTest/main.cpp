#include "SvnTest.h"

///////////////////////////////////////////////////////////////////////////////////////////////
// constructor
SvnTest::SvnTest()
{
	// initializes repository and local copy locations
	DestPath = "/tmp/svn-test";
	RepositoryUrl = "https://upp.svn.sourceforge.net/svnroot/upp/trunk/linux_scripts";

	// connects the layout
	CtrlLayout(*this, "Window title");
	
	// sets up handlers
	CheckoutButton		<<= THISBACK(Checkout);
	ConnectButton		<<= THISBACK(Connect);
	DisconnectButton	<<= THISBACK(Disconnect);
	ModifyButton		<<= THISBACK(Modify);
	LogButton			<<= THISBACK(GetLog);
	StatusButton		<<= THISBACK(GetStatus);
	ListButton			<<= THISBACK(ListFiles);
	WipeButton			<<= THISBACK(WipeFolder);
	QuitButton			<<= THISBACK(Quit);
	
}

///////////////////////////////////////////////////////////////////////////////////////////////
// 											handlers

///////////////////////////////////////////////////////////////////////////////////////////////
void SvnTest::Checkout(void)
{
	// clears text area
	TextArea.Clear();
	
	// sets up a welcome message
	TextArea.Set(TextArea.Get() + "Checking out '" + RepositoryUrl + "' repository... please wait\n\n");
	TextArea.Sync();
	
	// error if already connected
	if(svn.IsConnected())
	{
		ShowError("Already connected!!!");
		return;
	}

	// connects to repository
	if(svn.Checkout(RepositoryUrl, DestPath) == Svn::Ok)
	{
		StringStream s;
		s << "Local path            : " << svn.GetLocalPath()					<< "\n";	
		s << "Repository root       : " << svn.GetRepositoryRoot()				<< "\n";
		s << "Checked in revision   : " << AsString(svn.GetCheckedRevision())	<< "\n";
		s << "Author                : " << svn.GetCheckedRevisionAuthor()		<< "\n";
		s << "Head revision         : " << svn.GetHeadRevision()				<< "\n";
		s << "Revision Author       : " << svn.GetHeadRevisionAuthor()			<< "\n";
		TextArea.Set(TextArea.Get() + s);
	}
	else
		ShowError();
	
} // END SvnTest::Checkout()

///////////////////////////////////////////////////////////////////////////////////////////////
void SvnTest::Connect(void)
{
	// clears text area
	TextArea.Clear();
	
	// sets up a welcome message
	TextArea.Set(TextArea.Get() + "Connecting to local repository '" + DestPath + "' ... please wait\n\n");
	TextArea.Sync();
	
	// error if already connected
	if(svn.IsConnected())
	{
		ShowError("Already connected!!!");
		return;
	}

	// connects to repository
	if(svn.Connect(DestPath) == Svn::Ok)
	{
		StringStream s;
		s << "Local path            : " << svn.GetLocalPath()					<< "\n";	
		s << "Repository root       : " << svn.GetRepositoryRoot()				<< "\n";
		s << "Checked in revision   : " << AsString(svn.GetCheckedRevision())	<< "\n";
		s << "Author                : " << svn.GetCheckedRevisionAuthor()		<< "\n";
		s << "Head revision         : " << svn.GetHeadRevision()				<< "\n";
		s << "Revision Author       : " << svn.GetHeadRevisionAuthor()			<< "\n";
		TextArea.Set(TextArea.Get() + s);
	}
	else
		ShowError();
	
} // END SvnTest::Connect()

///////////////////////////////////////////////////////////////////////////////////////////////
void SvnTest::Disconnect(void)
{
	// clears text area
	TextArea.Clear();
	
	// sets up a welcome message
	TextArea.Set(TextArea.Get() + "Disconnecting from local repository '" + DestPath + "' ... please wait\n\n");
	TextArea.Sync();
	
	// disconnects from repository
	if(svn.Disconnect() == Svn::Ok)
		TextArea.Set(TextArea.Get() + "Success.\n");
	else
		ShowError();
	
} // END SvnTest::Disconnect()

///////////////////////////////////////////////////////////////////////////////////////////////
void SvnTest::Modify(void)
{
	// changes readme file
	String s = LoadFile(DestPath + "/README") + "\nA dummy change";
	SaveFile(DestPath + "/README", s);
	
	// removes dobeta file
	FileDelete(DestPath + "/dobeta");
	
	// adds an unversioned file
	SaveFile(DestPath + "/UnversionedFile", s);
	
	// adds a versioned file
	SaveFile(DestPath + "/VersionedFile", s);
	svn.Add(DestPath + "/VersionedFile");
	
} // END SvnTest::Modify()

///////////////////////////////////////////////////////////////////////////////////////////////
void SvnTest::GetLog(void)
{
	// clears text area
	TextArea.Clear();
	
	// sets up a welcome message
	TextArea.Set(TextArea.Get() + "Getting log from remote repository '" + RepositoryUrl + "' repository... please wait\n\n");
	TextArea.Sync();
	
	// gets log from svn repository
	TextArea.Set(TextArea.Get() + " ----- LOG OUTPUT -----\n");
	Array<Svn::LogInfo>logArray;
	if(svn.GetLog(logArray) == Svn::Ok)
	{
		StringStream s;
		for(int i = 0; i< logArray.GetCount(); i++)
		{
			s << "Revision : " << logArray[i].revision	<< "\n";
			s << "Author   : " << logArray[i].author	<< "\n";
			s << "Message  : " << logArray[i].message	<< "\n";
			s << "Date     : " << logArray[i].date		<< "\n";
			s << "-------------------------------------------------------------\n";
		}
		TextArea.Set(TextArea.Get() + s);
	}
	else
		ShowError();
	
} // END SvnTest::GetLog()

///////////////////////////////////////////////////////////////////////////////////////////////
void SvnTest::GetStatus(void)
{
	// clears text area
	TextArea.Clear();
	
	// sets up a welcome message
	TextArea.Set(TextArea.Get() + "Getting status from local repository '" + DestPath + "' ... please wait\n\n");
	TextArea.Sync();
	
	// gets repository status
	TextArea.Set(TextArea.Get() + " ----- STATUS OUTPUT -----\n");
	Array<Svn::StatusInfo>statusArray;
	if(svn.GetStatus(statusArray) == Svn::Ok)
	{
		StringStream s;
		for(int i = 0; i< statusArray.GetCount(); i++)
		{
			s << "Path            : " << statusArray[i].path						<< "\n";
			s << "Properties      : " << statusArray[i].props						<< "\n";
			s << "Status          : " << svn.Status2String(statusArray[i].status)	<< "\n";
			s << "Revision        : " << statusArray[i].revision					<< "\n";
			s << "Commit Revision : " << statusArray[i].commitRevision				<< "\n";
			s << "Commit Author   : " << statusArray[i].commitAuthor				<< "\n";
			s << "Commit Date     : " << statusArray[i].commitDate					<< "\n";
			s << "-------------------------------------------------------------\n";
		}
		TextArea.Set(TextArea.Get() + s);
	}
	else
		ShowError();
	
} // END SvnTest::GetStatus()


///////////////////////////////////////////////////////////////////////////////////////////////
void SvnTest::ListFiles(void)
{
	// clears text area
	TextArea.Clear();
	
	// sets up a welcome message
	TextArea.Set(TextArea.Get() + "List of files in test folder:\n\n");
	TextArea.Sync();
	
	StringStream s;
	Array<FileSystemInfo::FileInfo> files = StdFileSystemInfo().Find(DestPath + "/*");
	for(int i = 0; i < files.GetCount(); i++)
		s << files[i].filename << "\n";
	TextArea.Set(TextArea.Get() + s);
		
	
} // END SvnTest::ListFiles()

///////////////////////////////////////////////////////////////////////////////////////////////
void SvnTest::WipeFolder(void)
{
	// clears text area
	TextArea.Clear();
	
	// sets up a welcome message
	TextArea.Set(TextArea.Get() + "Wiping test folder '" + DestPath + "'.....\n\n");
	TextArea.Sync();
	
	DeleteFolderDeep(DestPath);
	
	TextArea.Set(TextArea.Get() + "Success.\n");
	
} // END SvnTest::WipeFolder()

///////////////////////////////////////////////////////////////////////////////////////////////
void SvnTest::Quit(void)
{
	Break();
	
} // END SvnTest::Quit()

///////////////////////////////////////////////////////////////////////////////////////////////
// error messagebox
void SvnTest::ShowError(void)
{
	StringStream s;
	s << "ERROR : " << svn.GetErrorMessage();
	TextArea.Set(TextArea.Get() + s + "\n");
	TextArea.Sync();
	PromptOK(String(s));
	
} // END SvnTest::ShowError()

void SvnTest::ShowError(String const &str)
{
	StringStream s;
	s << "ERROR : " << str;
	TextArea.Set(TextArea.Get() + s + "\n");
	TextArea.Sync();
	PromptOK(String(s));
	
} // END SvnTest::ShowError()

GUI_APP_MAIN
{
	SvnTest().Run();
}

