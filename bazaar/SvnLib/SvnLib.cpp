#include <SysExec/SysExec.h>

#include "SvnLib.h"

NAMESPACE_UPP


///////////////////////////////////////////////////////////////////////////////////////////////
// Human-readable error messages
const char *Svn::ErrorMessages[] =
{
	"Svn : Ok",
	"Svn : Unknown Error",
	"Svn : Svn application not found",
	"Svn : Not a SVN repository",
	"Svn : Not connected to a repository",
	"Svn : File not found",
	"Svn : Path not found",
	"Svn : File exists",
	"Svn : Directory exists",
	"Svn : Path exists",
	"Svn : Directory is not empty",
	"Svn : Path is not a directory",
	"Svn : Can't remove directory. Read only?",
	"Svn : Can't write do directory. Read only?"
	"Svn : Path is not in active repository",
	"Svn : Invalid repository URL",
	"Svn : Bad XML found",
	"Svn : Bad login. Check username/password"
	
} ; // END Svn::ErrorMessages[]

///////////////////////////////////////////////////////////////////////////////////////////////
// Human-readable status strings
const char *Svn::StatusStrings[] =
{
	"unknown",
	"added",
	"missing",
	"incomplete",
	"replaced",
	"modified",
	"merged",
	"conflicted",
	"obstructed",
	"ignored",
	"external",
	"unversioned"
			
}; // END Svn::StatusStrings

const int Svn::NumStatus = sizeof(StatusStrings) / sizeof(char *);
		
///////////////////////////////////////////////////////////////////////////////////////////////
// from status string, gets status
Svn::ItemStatus Svn::String2Status(String const &s)
{
	for(int i = 0; i < NumStatus; i++)
		if(s == StatusStrings[i])
			return (ItemStatus)i;
	return unknown;			

} // END Svn::String2Status()

///////////////////////////////////////////////////////////////////////////////////////////////
// from status, gets human-readable string
String Svn::Status2String(ItemStatus s)
{
	if(s >= unknown && s <= unversioned)
		return StatusStrings[s];
	else
		return "BAD STATUS";
	
} // END Svn::Status2String()

///////////////////////////////////////////////////////////////////////////////////////////////
// executes svn, passing cmdline as argument, gathering output in Output
// in xml format (where available) if xml is true
Svn::Errors Svn::ExecSvn(String const &SvnArgs, String &OutStr, String &ErrStr, bool xml)
{
	// adjust args if we wants xml output
	String args = (xml ? "--xml " : "") + SvnArgs;

	// adjust environment to get output in en_IN locale
	// otherwise it becomes difficult to understand command responses....
	VectorMap<String, String> env;
	env.Add("LC_MESSAGES", "en_IN");

	// executes svn command
	bool result = SysExec("svn", args, env, OutStr, ErrStr);
	FLastCommandOutput = OutStr;
	FLastCommandError = ErrStr;
	
	// if all ok, returns ok
	if(result && ErrStr == "")
		return SetError(Svn::Ok);
	
	// catches spawning errors
	if(ErrStr.Find("Couldn't fork") >=0 || ErrStr.Find("Error spawning process") >=0)
	   return SetError(NoSvnApp);
	
	// catches svn command errors
	
	// checks for ill-formed URLs
	if(ErrStr.Find("does not appear to be a URL") >= 0)
		return SetError(Svn::NotAnURL);
		
	// ckecks if could write to folder
	if(ErrStr.Find("Can't create directory") >= 0)
		return SetError(Svn::CantWriteDirectory);
	
	// checks wether we tried to operate on a non-svn folder
	if (ErrStr.Find("is not a working copy") >= 0)
		return SetError(Svn::NotARepo);
	
	return SetError(Svn::UnknownError);

} // END Svn::ExecSvn()

///////////////////////////////////////////////////////////////////////////////////////////////
// builds username/password string
String Svn::BuildUserPassword(void)
{
	if(FAnonymous)
		return " ";
	else
		return " --username " + FUserName + " --password " + FPassword + " ";
	
} // END Svn::BuildUserPassword()
		
///////////////////////////////////////////////////////////////////////////////////////////////
// builds revision string
String Svn::BuildRevision(String const &Revision)
{
	if(Revision != "")
		return " --revision " + Revision + " ";
	else
		return " ";

} // END Svn::BuildRevision()

///////////////////////////////////////////////////////////////////////////////////////////////
// checks wether a string is a remote URL or a local path
bool Svn::IsUrl(String const &path)
{
	if(
		path.Find("http://")	>= 0	||
		path.Find("https://")	>= 0	||
		path.Find("svn://")		>= 0	||
		path.Find("file://")	>= 0
	)
		return true;
	return false;

} // END Svn::IsUrl()

///////////////////////////////////////////////////////////////////////////////////////////////
// checks whether a path is inside local repository
bool Svn::IsLocalPath(String const &path)
{
	return (path.Find(FLocalPath) == 0);
	
} // END Svn::IsLocalPath()
		
///////////////////////////////////////////////////////////////////////////////////////////////
// checks whether a path is inside remote repository
bool Svn::IsRemotePath(String const &path)
{
	return (path.Find(FRepositoryRoot) == 0);
	
} // END Svn::IsRemotePath()
		
///////////////////////////////////////////////////////////////////////////////////////////////
// checks whether a path is inside repository, remote or local side
bool Svn::IsRepositoryPath(String const &path)
{
	return IsLocalPath(path) || IsRemotePath(path);
	
} // END Svn::IsRepositoryPath()

///////////////////////////////////////////////////////////////////////////////////////////////
// initializer... used by constructor and on disconnection
void Svn::Init(void)
{
	// log-in stuffs
	FUserName = "";
	FPassword = "";
	FAnonymous = true;
		
	// svn repository info stuffs
	FLocalPath				= "";
	FRepositoryRoot			= "";
	FRepositoryUUID			= "";
	FCheckedRevisionAuthor	= "";
	FCheckedRevision		= 0;
	FCheckedRevisionDate	= "";
	FHeadRevisionAuthor		= "";
	FHeadRevision			= 0;
	FHeadRevisionDate		= "";

	FLastError			= Ok;
	FConnected			= false;

	FLastCommandOutput	= "";
	FLastCommandError	= "";
	
} // END Svn::Init()

///////////////////////////////////////////////////////////////////////////////////////////////
// constructor class Svn
Svn::Svn()
{
	Init();
	
} // END constructor class Svn


///////////////////////////////////////////////////////////////////////////////////////////////
// destructor class Svn
Svn::~Svn()
{

} // END destructor class Svn


///////////////////////////////////////////////////////////////////////////////////////////////
// checks wether a path is a svn
bool Svn::IsSvn(String const &LocalPath)
{
	String Out, Err;
	return( ExecSvn("info " + LocalPath, Out, Err) == Svn::Ok);

} // END Svn::IsSvn()


///////////////////////////////////////////////////////////////////////////////////////////////
// parses xml output of 'svn info' command
Svn::Errors Svn::ParseInfo(String const &str, EntryInfo &info)
{
	try
	{
		XmlParser p(str);
		while (!p.IsTag() &&!p.End())
			p.Skip();
		p.PassTag("info");
		while (!p.End())
		{
			if (p.Tag("entry"))
			{
				info.kind		= p["kind"];
				info.revision	= atoi(p["revision"]);
				info.path		= p["path"];
				while (!p.End())
				{
					if (p.Tag("url"))
					{
						info.url = p.ReadText();
						p.SkipEnd();
					}
					else if(p.Tag("repository"))
					{
						while(!p.End())
						{
							if(p.Tag("root"))
							{
								info.repositoryRoot = p.ReadText();
								p.SkipEnd();
							}
							else if(p.Tag("uuid"))
							{
								info.repositoryUUID = p.ReadText();
								p.SkipEnd();
							}
							else
								p.Skip();
						}
					}
					else if(p.Tag("commit"))
					{
						while(!p.End())
						{
							if(p.Tag("revision"))
							{
								info.commitRevision = atoi(p.ReadText());
								p.SkipEnd();
							}
							else if(p.Tag("author"))
							{
								info.commitAuthor = p.ReadText();
								p.SkipEnd();
							}
							else if(p.Tag("date"))
							{
								info.commitDate = p.ReadText();
								p.SkipEnd();
							}
							else
								p.Skip();
						}
					}
					else
						p.Skip();
				}
			}
			else
				p.Skip();
		}
		
		return Ok;
	}
	catch (XmlError)
	{
		return BadXml;
	}

} // END Svn::ParseInfo()
		
///////////////////////////////////////////////////////////////////////////////////////////////
// parses xml output of 'svn log' command
Svn::Errors Svn::ParseLog(String const &str, Array<LogInfo>&infoArray)
{
	try
	{
		XmlParser p(str);
		while (!p.IsTag() &&!p.End())
			p.Skip();
		p.PassTag("log");
		while (!p.End())
		{
			if (p.Tag("logentry"))
			{
				LogInfo &info = infoArray.Add();
				info.author = "";
				info.date = "";
				info.message = "";
				info.revision	= atoi(p["revision"]);
				while (!p.End())
				{
					if (p.Tag("author"))
					{
						info.author = p.ReadText();
						p.SkipEnd();
					}
					else if(p.Tag("date"))
					{
						info.date = p.ReadText();
						p.SkipEnd();
					}
					else if(p.Tag("msg"))
					{
						info.message = p.ReadText();
						p.SkipEnd();
					}
					else
						p.Skip();
				}
			}
			else
				p.Skip();
		}
		
		return Ok;
	}
	catch (XmlError)
	{
		return BadXml;
	}

} // END Svn::ParseLog()

///////////////////////////////////////////////////////////////////////////////////////////////
// parses xml output of 'svn status' command
Svn::Errors Svn::ParseStatus(String const &str, Array<StatusInfo> &infoArray)
{
	try
	{
		XmlParser p(str);
		while (!p.IsTag() &&!p.End())
			p.Skip();
		p.PassTag("status");
		p.PassTag("target");
		while (!p.End())
		{
			if (p.Tag("entry"))
			{
				StatusInfo &info = infoArray.Add();
				info.path = "";
				info.props = "";
				info.status = unknown;
				info.revision = -1;
				info.commitRevision = -1;
				info.commitAuthor = "";
				info.commitDate = "";

				info.path	= p["path"];
				while (!p.End())
				{
					if (p.Tag("wc-status"))
					{
						info.props		= p["props"];
						info.status		= String2Status(p["item"]);
						info.revision	= atoi(p["revision"]);
						p.SkipEnd();
					}
					else if(p.Tag("commit"))
					{
						info.commitRevision = atoi(p["revision"]);
						p.SkipEnd();
					}
					else if(p.Tag("author"))
					{
						info.commitAuthor = p.ReadText();
						p.SkipEnd();
					}
					else if(p.Tag("date"))
					{
						info.commitDate = p.ReadText();
						p.SkipEnd();
					}
					else
						p.Skip();
				}
			}
			else
				p.Skip();
		}
		
		return Ok;
	}
	catch (XmlError)
	{
		return BadXml;
	}
	
} // END Svn::ParseStatus()

///////////////////////////////////////////////////////////////////////////////////////////////
// sets user authentication
void Svn::SetUser(String const &UserName, String const &Password)
{
	FUserName = UserName;
	FPassword = Password;
	FAnonymous = false;
	
} // END Svn::SetUser()

///////////////////////////////////////////////////////////////////////////////////////////////
// sets user authentication
void Svn::SetUser(void)
{
	FUserName = "";
	FPassword = "";
	FAnonymous = true;
	
} // END Svn::SetUser()

///////////////////////////////////////////////////////////////////////////////////////////////
// connects to the local copy
Svn::Errors Svn::Connect(String const &LocalPath)
{
	Svn::Errors result;
	String Out, Err;
	
	// first, checks wether is a svn repo
	if(!IsSvn(LocalPath))
		return SetError(Svn::NotARepo);

	// executes 'svn info' command -- gets current checked-in information
	result = ExecSvn("info " + BuildUserPassword() + LocalPath, Out, Err, true);
	if(result != Svn::Ok)
		return SetError(result);
	
	// parses xml info output
	EntryInfo info;
	result = ParseInfo(Out, info);
	if(result != Svn::Ok)
		return SetError(result);
	
	// stores repository info
	FLocalPath				= LocalPath;
	FRepositoryRoot			= info.repositoryRoot;
	FRepositoryUUID			= info.repositoryUUID;
	FCheckedRevisionAuthor	= info.commitAuthor;
	FCheckedRevision		= info.revision;
	FCheckedRevisionDate	= info.commitDate;

	// executes 'svn info -r HEAD' command -- gets HEAD information
	result = ExecSvn("info -r HEAD " + BuildUserPassword() + LocalPath, Out, Err, true);
	if(result != Svn::Ok)
		return SetError(result);

	// parses xml info output
	result = ParseInfo(Out, info);
	if(result != Svn::Ok)
		return SetError(result);
	
	// stores repository info
	FHeadRevisionAuthor	= info.commitAuthor;
	FHeadRevision		= info.revision;
	FHeadRevisionDate	= info.commitDate;

	FConnected			= true;
	FLastError			= Ok;

	return SetError(Svn::Ok);	

} // END Svn::Connect()

///////////////////////////////////////////////////////////////////////////////////////////////
// disconnects from local copy
Svn::Errors Svn::Disconnect(void)
{
	// if not connected, does nothing
	if(!FConnected)
		return SetError(Svn::NotConnected);

	// reinitializes
	Init();
	
	return Svn::Ok;
	
} // END Svn::Disconnect()

///////////////////////////////////////////////////////////////////////////////////////////////
// checks out from server
Svn::Errors Svn::Checkout(String const &Url, String const &LocalPath, String const &Revision)
{
	String Out, Err;
	String Path;
	Svn::Errors result;
	
	// adjust final path separator
	Path = LocalPath;
	if(Path[Path.GetLength()-1] != DIR_SEP)
		Path << DIR_SEP;
	
	// fists, looks at localpath; if it does exist, it must be empty
	if(Upp::DirectoryExists(Path))
	{
		// checks if directory is empty
		Array<FileSystemInfo::FileInfo> fileInfo = StdFileSystemInfo().Find(Path + "*");
		bool found = false;
		for(int i = 0; i < fileInfo.GetCount(); i++)
		{
			if(fileInfo[i].filename != "." && fileInfo[i].filename != "..")
			{
				found = true;
				break;
			}
		}
		if(found)
			return SetError(DirectoryNotEmpty);
		
		// removes the path
		if(!DirectoryDelete(Path))
			return SetError(CantRemoveDirectory);
	}
	
	// well, now we can do the checkout
	result = ExecSvn("checkout -q --no-auth-cache --non-interactive" + BuildUserPassword() + BuildRevision(Revision) + Url + " " + Path, Out, Err, false);
	if(result != Svn::Ok)
		return SetError(result);
	
	// checkout should be done, connect to checked out folder
	return SetError(Connect(Path));

} // END Svn::Checkout()

///////////////////////////////////////////////////////////////////////////////////////////////
// updates current copy from server
Svn::Errors Svn::Update(String const &Revision)
{
	String Out, Err;
	Svn::Errors result;

	// if not connected, does nothing
	if(!FConnected)
		return SetError(Svn::NotConnected);

	// well, now we can do the update
	result = ExecSvn("update -q --no-auth-cache --non-interactive" + BuildUserPassword() + BuildRevision(Revision) + FLocalPath, Out, Err, false);
	if(result != Svn::Ok)
		return SetError(result);
	
	// executes 'svn info' command -- gets current checked-in information
	result = ExecSvn("info " + BuildUserPassword() + FLocalPath, Out, Err, true);
	if(result != Svn::Ok)
		return SetError(result);
	
	// parses xml info output
	EntryInfo info;
	result = ParseInfo(Out, info);
	if(result != Svn::Ok)
		return SetError(result);
	
	// stores repository info
	FRepositoryRoot			= info.repositoryRoot;
	FRepositoryUUID			= info.repositoryUUID;
	FCheckedRevisionAuthor	= info.commitAuthor;
	FCheckedRevision		= info.revision;
	FCheckedRevisionDate	= info.commitDate;
	
	return SetError(Svn::Ok);

} // END Svn::Update()

///////////////////////////////////////////////////////////////////////////////////////////////
// commits to server
Svn::Errors Svn::Commit(String const &Message)
{
	String Out, Err;
	Svn::Errors result;

	// if not connected, does nothing
	if(!FConnected)
		return SetError(Svn::NotConnected);

	// well, now we can do the commit
	result = ExecSvn("commit -q --no-auth-cache --non-interactive" + BuildUserPassword() + " -- message " + Message, Out, Err, false);
	if(result != Svn::Ok)
		return SetError(result);
	
	return SetError(Svn::Ok);
	
} // END Svn::Commit()

///////////////////////////////////////////////////////////////////////////////////////////////
// adds file(s) to current repository
Svn::Errors Svn::Add(String const &FileName)
{
	String Out, Err;
	Svn::Errors result;

	// if not connected, does nothing
	if(!FConnected)
		return SetError(Svn::NotConnected);
	
	// checks whether given file/directory exists
	if(!Upp::FileExists(FileName) && !Upp::DirectoryExists(FileName))
		return SetError(Svn::PathNotFound);
	
	// checks whether given path is inside repository
	if(!IsLocalPath(FileName))
		return SetError(Svn::PathNotInRepo);

	// now we can add file to repository
	result = ExecSvn("add -q " + FileName, Out, Err, false);
	if(result != Svn::Ok)
		return SetError(result);
	
	return SetError(Svn::Ok);

} // END Svn::Add()

Svn::Errors Svn::Add(Array<String> const &FileNames)
{
	String Out, Err;
	Svn::Errors result;

	// if not connected, does nothing
	if(!FConnected)
		return SetError(Svn::NotConnected);
	
	// checks if all furnished files/directories exists and are in local repository
	for(int i = 0; i < FileNames.GetCount(); i++)
	{
		if(!Upp::FileExists(FileNames[i]) && !Upp::DirectoryExists(FileNames[i]))
			return SetError(Svn::PathNotFound);

		if(!IsLocalPath(FileNames[i]))
			return SetError(Svn::PathNotInRepo);
	}

	// now we can add files to repository
	for(int i = 0; i < FileNames.GetCount(); i++)
	{
		result = ExecSvn("add -q " + FileNames[i], Out, Err, false);
		if(result != Svn::Ok)
			return SetError(result);
	}
	return SetError(Svn::Ok);

} // END Svn::Add()

///////////////////////////////////////////////////////////////////////////////////////////////
// removes file(s) from current repository
Svn::Errors Svn::Delete(String const &FileName)
{
	String Out, Err;
	Svn::Errors result;

	// if not connected, does nothing
	if(!FConnected)
		return SetError(Svn::NotConnected);
	
	// must NOT check if files exists, they can be deleted
	// from folder but be listed in repo
	
	// checks whether given path is inside repository
	if(!IsLocalPath(FileName))
		return SetError(Svn::PathNotInRepo);

	// now we can remove file to repository
	result = ExecSvn("delete -q " + FileName, Out, Err, false);
	if(result != Svn::Ok)
		return SetError(result);
	
	return SetError(Svn::Ok);

} // END Svn::Delete()

Svn::Errors Svn::Delete(Array<String> const &FileNames)
{
	String Out, Err;
	Svn::Errors result;

	// if not connected, does nothing
	if(!FConnected)
		return SetError(Svn::NotConnected);
	
	// must NOT check if files exists, they can be deleted
	// from folder but be listed in repo

	// checks if all furnished files/directories are in local repository
	for(int i = 0; i < FileNames.GetCount(); i++)
	{
		if(!IsLocalPath(FileNames[i]))
			return SetError(Svn::PathNotInRepo);
	}

	// now we can add files to repository
	for(int i = 0; i < FileNames.GetCount(); i++)
	{
		result = ExecSvn("delete -q " + FileNames[i], Out, Err, false);
		if(result != Svn::Ok)
			return SetError(result);
	}
	return SetError(Svn::Ok);

} // END Svn::Delete()

///////////////////////////////////////////////////////////////////////////////////////////////
// copy a file inside current repository
Svn::Errors Svn::Copy(String const &Src, String const &Dest)
{
	String Out, Err;
	Svn::Errors result;
	String AuthStr;

	// if not connected, does nothing
	if(!FConnected)
		return SetError(Svn::NotConnected);
	
	// first, checks that given paths are inside current repository
	if(!IsRepositoryPath(Src) || !IsRepositoryPath(Dest))
		return SetError(Svn::PathNotInRepo);
	
	// if source path is local, checks if exists
	if(!IsUrl(Src) && !Upp::FileExists(Src) && !Upp::DirectoryExists(Src))
		return SetError(Svn::PathNotFound);
	
	// dest path is local, it should not exist
	if(!IsUrl(Dest) && (Upp::FileExists(Dest) || Upp::DirectoryExists(Dest)))
		return SetError(Svn::PathExists);
	
	// if source or dest are remote, may require authentication
	if(IsUrl(Src) || IsUrl(Dest))
		AuthStr = BuildUserPassword();
	else
		AuthStr = " ";
	
	// now we can do the copy
	result = ExecSvn("copy -q " + AuthStr + Src + " " + Dest, Out, Err, false);
	if(result != Svn::Ok)
		return SetError(result);
	
	return SetError(Svn::Ok);

} // END Svn::Copy()

///////////////////////////////////////////////////////////////////////////////////////////////
// moves a file inside current repository
Svn::Errors Svn::Move(String const &Src, String const &Dest)
{
	String Out, Err;
	Svn::Errors result;
	String AuthStr;

	// if not connected, does nothing
	if(!FConnected)
		return SetError(Svn::NotConnected);
	
	// first, checks that given paths are inside current repository
	if(!IsRepositoryPath(Src) || !IsRepositoryPath(Dest))
		return SetError(Svn::PathNotInRepo);
	
	// if source path is local, checks if exists
	if(!IsUrl(Src) && !Upp::FileExists(Src) && !Upp::DirectoryExists(Src))
		return SetError(Svn::PathNotFound);
	
	// dest path is local, it should not exist
	if(!IsUrl(Dest) && (Upp::FileExists(Dest) || Upp::DirectoryExists(Dest)))
		return SetError(Svn::PathExists);
	
	// if source or dest are remote, may require authentication
	if(IsUrl(Src) || IsUrl(Dest))
		AuthStr = BuildUserPassword();
	else
		AuthStr = " ";
	
	// now we can do the move
	result = ExecSvn("move -q " + AuthStr + Src + " " + Dest, Out, Err, false);
	if(result != Svn::Ok)
		return SetError(result);
	
	return SetError(Svn::Ok);

} // END Svn::Move()

///////////////////////////////////////////////////////////////////////////////////////////////
// gets log from remote
Svn::Errors Svn::GetLog(Array<Svn::LogInfo>&logArray)
{
	String Out, Err;
	Svn::Errors result;

	// if not connected, does nothing
	if(!FConnected)
		return SetError(Svn::NotConnected);
	
	// executes 'svn log' command -- gets current checked-in information
	result = ExecSvn("log " + BuildUserPassword() + FLocalPath, Out, Err, true);
	if(result != Svn::Ok)
		return SetError(result);
		
	// parses log into logArray
	return SetError(ParseLog(Out, logArray));
	 
} // END Svn::GetLog()
		
///////////////////////////////////////////////////////////////////////////////////////////////
// gets local repository status
Svn::Errors Svn::GetStatus(Array<Svn::StatusInfo>&statusArray)
{
	String Out, Err;
	Svn::Errors result;

	// if not connected, does nothing
	if(!FConnected)
		return SetError(Svn::NotConnected);
	
	// executes 'svn status' command -- gets current checked-in information
	result = ExecSvn("status " + FLocalPath, Out, Err, true);
	if(result != Svn::Ok)
		return SetError(result);
		
	// parses log into logArray
	return SetError(ParseStatus(Out, statusArray));
	
} // END Svn::GetStatus()
		
///////////////////////////////////////////////////////////////////////////////////////////////
// updates info from current repository
Svn::Errors Svn::UpdateInfo(void)
{

} // END Svn::UpdateInfo()

END_UPP_NAMESPACE
