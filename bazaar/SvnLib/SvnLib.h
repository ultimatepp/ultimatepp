#ifndef _SvnLib_h_
#define _SvnLib_h_

#include <Core/Core.h>

NAMESPACE_UPP

//////////////////////////////////////////////////////////////////////////////////////////////
// svn interface class
// encapsulates svn operation using command line tool 'svn'
class Svn
{
	public:
		// types of svn nodes
		enum NodeTypes
		{
			SvnUnknown,
			SvnDir
		};
		
		// errors returned by various svn functions
		enum Errors
		{
			Ok			= 0,
			UnknownError,
			NoSvnApp,
			NotARepo,
			NotConnected,
			FileNotFound,
			PathNotFound,
			FileExists,
			DirectoryExists,
			PathExists,
			DirectoryNotEmpty,
			NotADirectory,
			CantRemoveDirectory,
			CantWriteDirectory,
			PathNotInRepo,
			NotAnURL,
			BadXml,
			BadLogin
		};
		
		// Human-readable error messages
		static const char *ErrorMessages[];
		
		// Item status
		enum ItemStatus
		{
			unknown,
			added,
			missing,
			incomplete,
			replaced,
			modified,
			merged,
			conflicted,
			obstructed,
			ignored,
			external,
			unversioned,
			
		}; // END enum ItemStatus
		
		// Human-readable status strings
		static const char *StatusStrings[];
		static const int NumStatus;
		
		// from status string, gets status
		static ItemStatus String2Status(String const &s);
		
		// from status, gets human-readable string
		static String Status2String(ItemStatus s);
		
		// svn entry info
		struct EntryInfo
		{
			String	kind;
			String	path;
			long	revision;
			String	url;
			String	repositoryRoot;
			String	repositoryUUID;
			long	commitRevision;
			String	commitAuthor;
			String	commitDate;
			
		}; // END struct EntryInfo;

		// svn log info
		struct LogInfo
		{
			long	revision;
			String	author;
			String	message;
			String	date;
			
		}; // END struct LogInfo
		
		// svn status info
		struct StatusInfo
		{
			String		path;
			String		props;
			ItemStatus	status;
			long		revision;
			long		commitRevision;
			String		commitAuthor;
			String		commitDate;
			
		}; // END struct StatusInfo
		
	private:
		// log-in stuffs
		String		FUserName;
		String		FPassword;
		bool		FAnonymous;
		
		// svn repository info stuffs
		String		FLocalPath;
		String		FRepositoryRoot;
		String		FRepositoryUUID;
		
		String		FCheckedRevisionAuthor;
		long		FCheckedRevision;
		String		FCheckedRevisionDate;

		String		FHeadRevisionAuthor;
		long		FHeadRevision;
		String		FHeadRevisionDate;

		bool		FConnected;
		Errors		FLastError;
		
		String		FLastCommandOutput;
		String		FLastCommandError;

		// executes svn, passing cmdline as argument, gathering output in Output
		// in xml format (where available) if xml is true
		Svn::Errors ExecSvn(String const &CmdLine, String &OutStr, String &ErrStr, bool xml = false);
		
		// parses output of 'svn info' command and gathers its output
		Errors ParseInfo(String const &str, EntryInfo &info);
		
		//  parses xml output of 'svn log' command
		Errors ParseLog(String const &str, Array<LogInfo> &info);
		
		// parses xml output of 'svn status' command
		Errors ParseStatus(String const &str, Array<StatusInfo> &info);

		// builds username/password string
		String BuildUserPassword(void);
		
		// builds revision string
		String BuildRevision(String const &Revision);
		
		// checks whether a string is a remote URL or a local path
		bool IsUrl(String const &path);
		
		// checks whether a path is inside local repository
		bool IsLocalPath(String const &path);
		
		// checks whether a path is inside remote repository
		bool IsRemotePath(String const &path);
		
		// checks whether a path is inside repository, remote or local side
		bool IsRepositoryPath(String const &path);
		
		// initializer... used by constructor and on disconnection
		void Init(void);

	public:
	
		// constructor
		Svn();
		
		// destructor
		~Svn();
		
		// sets error condition
		Errors SetError(Errors e) { FLastError = e; return e; }
		
		// checks wether a path is a svn
		bool IsSvn(String const &LocalPath);
		
		// checks wether the repository is connected
		bool IsConnected(void) { return FConnected; }
		
		// gets local path of repository
		String		GetLocalPath()						{ return FLocalPath; }
		String		GetRepositoryRoot()					{ return FRepositoryRoot; }
		String		GetRepositoryUUID()					{ return FRepositoryUUID; }
		String		GetCheckedRevisionAuthor()			{ return FCheckedRevisionAuthor; }
		long		GetCheckedRevision()				{ return FCheckedRevision; }
		String		GetCkeckedRevisionDate()			{ return FCheckedRevisionDate; }
		String		GetHeadRevisionAuthor()				{ return FHeadRevisionAuthor; }
		long		GetHeadRevision()					{ return FHeadRevision; }
		String		GetHeadRevisionDate()				{ return FHeadRevisionDate; }

		Errors		GetLastError()						{ return FLastError; }
		String		GetErrorMessage(Svn::Errors err)	{ return ErrorMessages[err]; }
		String		GetErrorMessage(void)				{ return ErrorMessages[FLastError]; }
		
		String		GetLastCommandOutput()				{ return FLastCommandOutput; }
		String		GetLastCommandError()				{ return FLastCommandError; }
		
		// sets user authentication
		void SetUser(String const &UserName, String const &Password);
		void SetUser(void);

		// connects to the local copy
		Errors Connect(String const &LocalPath);
		
		// disconnects from local copy
		Errors Disconnect(void);
		
		// checks out from server
		Errors Checkout(String const &Url, String const &LocalPath, String const &Revision = "");
		
		// updates current copy from server
		Errors Update(String const &Revision = "");
		
		// commits to server
		Errors Commit(String const &Message);
		
		// adds file(s) to current repository
		Errors Add(String const &FileName);
		Errors Add(Array<String> const &FileNames);
		
		// removes file(s) from current repository
		Errors Delete(String const &FileName);
		Errors Delete(Array<String> const &FileNames);
		
		// copy a file inside current repository
		Errors Copy(String const &Src, String const &Dest);

		// moves a file inside current repository
		Errors Move(String const &Src, String const &Dest);
		
		// gets log from remote
		Errors GetLog(Array<Svn::LogInfo>&logArray);
		
		// gets local repository status
		Errors GetStatus(Array<Svn::StatusInfo>&statusArray);
		
		// updates info from current repository
		Errors UpdateInfo(void);
	
}; // END Class Svn

/*	
	blame
	cat
	cleanup
	diff
	export
	import
	list
	lock
	log
	merge
	mkdir
	propdel (pdel, pd)
	propedit (pedit, pe)
	propget (pget, pg)
	proplist (plist, pl)
	propset (pset, ps)
	resolved
	revert
	status (stat, st)
	switch (sw)
	unlock
*/

END_UPP_NAMESPACE

#endif
