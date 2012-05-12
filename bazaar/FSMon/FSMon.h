#ifndef _FSMon_FSMon_h
#define _FSMon_FSMon_h

#include <Core/Core.h>

#ifdef flagGUI
#include <CtrlLib/CtrlLib.h>
#endif

NAMESPACE_UPP

class FSMon
{
	public:
		// flags stating type of changes
		typedef enum
		{
			FSM_NOP				= 0x00,
			FSM_Created			= 0x01,
			FSM_Deleted			= 0x02,
			FSM_Moved			= 0x04,
			FSM_FolderCreated	= 0x08,
			FSM_FolderDeleted	= 0x10,
			FSM_FolderMoved		= 0x20,
			FSM_Modified		= 0x40,
			FSM_AttribChange	= 0x80
		} Flags;

		struct Info :  public Moveable<Info>
		{
			// path of changed file
			String path;
			
			// new file path for renamed/moved files
			String newPath;
			
			// type of change flags
			dword flags;
		};
	
	private:

#ifdef PLATFORM_WIN32

		//	Enable/Disable process privileges.
		BOOL EnablePrivilege(LPCTSTR pszPrivName, BOOL fEnable);

		// check if path is a folder or a file
		bool isFolder(String const &path);

		// struct containing info needed for ReadDirectoryChangesW
		#define READ_DIR_CHANGE_BUFFER_SIZE	8192
		struct CHANGESINFO
		{
			HANDLE hDir;
			byte buffer[READ_DIR_CHANGE_BUFFER_SIZE];
			OVERLAPPED overlapped;

			bool cancelling;
		};
		Array<CHANGESINFO>monitoredInfo;
		
		// error code
		DWORD errCode;

		// get error string from code
		String GetErrorStr(HRESULT err);

		// completion port for async I/O
		HANDLE completionPort;
		
		// keys for watched folders -- need to be passed to completion port
		LONG lastDescriptor;
		
		// scans result buffer for FILE_NOTIFY_INFORMATION records
		// and process them
		void ProcessNotify(FILE_NOTIFY_INFORMATION *buf, String const &path, bool second);

		// monitored descriptors
		Index<LONG> monitoredDescriptors;

#else	
		// error code
		int errCode;
		
		// get error string from code
		String GetErrorStr(int err);
		
		int iNotifyHandle;

		// scans a newly created folder to look for files
		// being created BEFORE notify handler was in place
//		void ScanCreatedFolder(String path);
		
		// recursively add or remove monitors for paths
		bool AddWatch(String const &path);
		bool RemoveWatch(String const &path);

		// event handling selector
		void EventsSelector(uint32 mask, String const &path, String const &newPath);

		// monitored descriptors
		Index<int> monitoredDescriptors;

#endif

		// flag stating that we want to be notified only on file close
		// this spares many notifications and the caveats of working on
		// opened file, but don't work for files always opened
		// (example : big database files)
		bool notifyOnClose;

		// error string
		String errMsg;
		
		// array of errors paths and codes filled by
		// recursive watcher -- if can be useful to
		// allow some errors when watching
		VectorMap<String, int> errMap;
	
		// mutex for thread locking
		Mutex fsmMutex;
		
		// a secondary mutex, used for locking on folder creation
		// this one avoids waiting when not needed
		Mutex fsmMutex2;
		
		// the checking thread
		volatile bool threadRunning;
		Thread fsmThread;
	
		// changed files/folders list
		Vector<Info> changed;
		
		// monitored paths
		Index<String> monitoredPaths;
		
		// actually opened files -- may be handy
		// for a sync application and for locking purposes
		Index<String>openedFiles;
		
		// sets error code message from errno
		void SetError(int err) { errCode = err; errMsg = GetErrorStr(err); }
		
		// monitoring callback (runs in a separate thread)
		volatile bool shutDown;
		void monitorCb(void);
		
		// callback to call event handler in maint thread
		// (via PostCallback) when using GUI
		void callHandlerCb(void);
		
	protected:
	
	public:
	
		typedef FSMon CLASSNAME;
	
		// constructor
		FSMon(bool notifyOnClose = false);
		
		// destructor
		~FSMon();
	
		// add a monitored path
		bool Add(String const &path);
		
		// remove a monitored path
		bool Remove(String const &path);
		
		// query for changed files/folders
		Vector<Info> GetChanged(void);
		
		// query IF some changes happened
		bool HasChanges(void);
		
		// gets actually opened files
		Index<String>GetOpenedFiles(void);
		
		// check error contidion and get error message
		bool IsError(void) { return errCode != 0; }
		bool GetErrorCode(void) { return errCode; }
		String GetErrorMsg(void) { return errMsg; }
		VectorMap<String, int> GetErrorMap(void);
		
		// callback to signal happened event
		// avoids polling, if needed
		Callback EventHandler;
};

END_UPP_NAMESPACE

#endif
