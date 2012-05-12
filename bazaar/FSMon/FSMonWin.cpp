#include "FSMon.h"

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

//	Enable/Disable process privileges.
BOOL FSMon::EnablePrivilege(LPCTSTR pszPrivName, BOOL fEnable)
{
	// Assume function fails
	BOOL fOk = FALSE;

	// Try to open this process's access token
	HANDLE hToken;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		// privilege
		TOKEN_PRIVILEGES tp = { 1 };

		if (LookupPrivilegeValue(NULL, pszPrivName,  &tp.Privileges[0].Luid))
		{
			tp.Privileges[0].Attributes = fEnable ?  SE_PRIVILEGE_ENABLED : 0;
			AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
			fOk = (GetLastError() == ERROR_SUCCESS);
		}
		CloseHandle(hToken);
	}
	return(fOk);
}

// check if path is a folder or a file
bool FSMon::isFolder(String const &path)
{
	DWORD dwAttrib	= GetFileAttributes(path);
	return static_cast<bool>((dwAttrib != 0xffffffff && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)));
}

// constructor
FSMon::FSMon(bool nOnClose)
{
	// must create completion port on first use
	completionPort = NULL;
	
	// reset completion keys generator
	// skip 0 descriptor, we never know if 0 is returned...
	lastDescriptor = 1;

	// those are needed for ReadDirectoryChangesW
	EnablePrivilege(SE_BACKUP_NAME, true);
	EnablePrivilege(SE_RESTORE_NAME, true);
	EnablePrivilege(SE_CHANGE_NOTIFY_NAME, true);
	
	shutDown = false;
	threadRunning = false;
}

// destructor
FSMon::~FSMon()
{
	// if thread not started, no need to shutdown
	if(threadRunning)
	{
		// cancels all pending IO operations
		// can't use the CancelIoEx (available from Vista+)
		// so we first cancel IO for current thread
		// then we send a packet that causes worker thread to
		// cancel its part
		for(int i = 0; i < monitoredInfo.GetCount(); i++)
		{
			CancelIo(monitoredInfo[i].hDir);
			// no way to pass valid parameters with PostQueuedCompletionStatus(),
			// so we use a 'cancelling' flag inside Info recors
			INTERLOCKED_(fsmMutex2){
				monitoredInfo[i].cancelling = true;
			}
			PostQueuedCompletionStatus(completionPort, 0, monitoredDescriptors[i], NULL);
			while(true)
			{
				bool cancelling;
				INTERLOCKED_(fsmMutex2){
					cancelling = monitoredInfo[i].cancelling;
				}
				if(!cancelling)
					break;
				else
					Sleep(10);
			}
		}
		
		// wait some time just to allow thread to cancel all pending I/O
		Sleep(10);
		
		// signal thread shutdown
		shutDown = true;
		
		// send a completion packet to completion port
		// in order make it return
		PostQueuedCompletionStatus(completionPort, 0, 0, NULL);
		
		while(shutDown)
			;
		threadRunning = false;
	}
	
	// close handles and completion port
	for(int i = 0; i < monitoredInfo.GetCount(); i++)
		CloseHandle(monitoredInfo[i].hDir);
	CloseHandle(completionPort);
}

// get error string from code
String FSMon::GetErrorStr(HRESULT err)
{
	LPTSTR errorText;
	String res;
	
	FormatMessage(
		// use system message tables to retrieve error text
		FORMAT_MESSAGE_FROM_SYSTEM |
		// allocate buffer on local heap for error text
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		// Important! will fail otherwise, since we're not 
		// (and CANNOT) pass insertion parameters
		FORMAT_MESSAGE_IGNORE_INSERTS,  
		NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
		err,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&errorText,  // output 
		0, // minimum size for output buffer
		NULL);   // arguments - see note 
	
	if(errorText)
	{
		res = errorText;
		LocalFree(errorText);
	}
	
	return res;
}

// add a monitored path
bool FSMon::Add(String const &path)
{
	bool res = true;
	for(int iType = 0; iType < 2; iType++)
	{
		//open the directory to watch
		HANDLE hDir = CreateFile(
			path,
			FILE_LIST_DIRECTORY,
			FILE_SHARE_READ | FILE_SHARE_WRITE ,//| FILE_SHARE_DELETE, <-- removing FILE_SHARE_DELETE prevents the user or someone else from renaming or deleting the watched directory. This is a good thing to prevent.
			NULL, //security attributes
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS | //<- the required priviliges for this flag are: SE_BACKUP_NAME and SE_RESTORE_NAME.  CPrivilegeEnabler takes care of that.
			FILE_FLAG_OVERLAPPED, // for async mode
			NULL
		);

		if (hDir == INVALID_HANDLE_VALUE)
			return false;

		//create a IO completion port/or associate this key with
		//the existing IO completion port
		completionPort = CreateIoCompletionPort(
			hDir,
			// if completionPort is NULL, hDir is associated with a NEW completion port,
			// if completionPort is NON-NULL, hDir is associated with the existing completion
			// port that the handle completionPort references
			completionPort,
			// the completion 'key'... this ptr is returned from GetQueuedCompletionStatus() when one of the events in the dwChangesToWatchFor filter takes place
			lastDescriptor,
			0
		);
		if(!completionPort)
		{
			SetError(GetLastError());
			CloseHandle(hDir);
			return false;
		}
	
		// completion port created, we can now store monitored path and its key
		// and update key generator
	
		// must lock here to avoid working threat to access
		// removed stuffs
		INTERLOCKED_(fsmMutex2)
		{
			monitoredPaths.Add(path);
			monitoredDescriptors.Add(lastDescriptor++);
			monitoredInfo.Add();
		}
	
		CHANGESINFO &info = monitoredInfo.Top();
		info.hDir = hDir;
		info.cancelling = false;
		
		// overlapped MUST be zero filled, otherwise ReadDirectoryChangesW fails
		memset(&info.overlapped, 0, sizeof(OVERLAPPED));
	
		// an now, if not already done, we must start working thread
		if(!threadRunning)
		{
			shutDown = false;
			fsmThread.Start(THISBACK(monitorCb));
			threadRunning = true;
		}

		// thread is started, we shall add the monitored folder with ReadDirectoryChangesW
		DWORD bufLen;
		if (!ReadDirectoryChangesW(
				info.hDir,
				info.buffer,
				READ_DIR_CHANGE_BUFFER_SIZE,
				true, // we want subdirs events
				iType ?
					FILE_NOTIFY_CHANGE_ATTRIBUTES	|
					FILE_NOTIFY_CHANGE_SECURITY		|
					FILE_NOTIFY_CHANGE_DIR_NAME
				:
					FILE_NOTIFY_CHANGE_FILE_NAME	|
					FILE_NOTIFY_CHANGE_SIZE			|
					FILE_NOTIFY_CHANGE_LAST_WRITE,
				&bufLen, //this var not set when using asynchronous mechanisms...
				&info.overlapped,
				NULL)) //no completion routine!
		{
			SetError(GetLastError());
			return false;
		}
	}
	// just in case of 1 of 2 calls went bad... we must have an EVEN
	// numbero of elements in arrays
	if(monitoredPaths.GetCount() & 1)
	{
		INTERLOCKED_(fsmMutex2)
		{
			monitoredPaths.Drop();
			monitoredDescriptors.Drop();
			monitoredInfo.Drop();
		}
	}
	return true;
}

bool FSMon::Remove(String const &path)
{
	int idx = monitoredPaths.Find(path);
	if(idx < 0)
		return false;
	idx &= 0xFFFFFFFFFFFFFFFE;
	
	for(int iType = 0; iType < 2; iType++)
	{
		CHANGESINFO &info = monitoredInfo[idx + iType];
		
		// camcels io for corresponding path
		// as usual, we can't use CancelIoEx because we want it working on XP
		// so we cancel io in current thread and fake the behaviour
		// sending a cancel packet to worker thread
	
		// no way to pass valid parameters with PostQueuedCompletionStatus(),
		// so we use a 'cancelling' flag inside Info recors
		INTERLOCKED_(fsmMutex2)	{
			info.cancelling = true;
		}
		PostQueuedCompletionStatus(completionPort, 0, monitoredDescriptors[idx + iType], NULL);
		CancelIo(info.hDir);
		
		// wait for tread get the cancelling packet and reset the flag...
		while(true)
		{
			bool cancelling;
			INTERLOCKED_(fsmMutex2){
				cancelling = info.cancelling;
			}
			if(!cancelling)
				break;
			else
				Sleep(10);
		}
		
		// free dir handle
		CloseHandle(info.hDir);
	
	}
	// must lock here to avoid working thread to access
	// removed stuffs
	INTERLOCKED_(fsmMutex2)
	{
		monitoredPaths.Remove(idx, 2);
		monitoredInfo.Remove(idx, 2);
		monitoredDescriptors.Remove(idx, 2);
	}
	return true;
}

// scans result buffer for FILE_NOTIFY_INFORMATION records
// and process them
/*
typedef struct _FILE_NOTIFY_INFORMATION {
  DWORD NextEntryOffset;
  DWORD Action;
  DWORD FileNameLength;
  WCHAR FileName[1];
} FILE_NOTIFY_INFORMATION, *PFILE_NOTIFY_INFORMATION;

To separate the events from originating flags, we need a couple of listeners
Flags and corresponding catched events :

							LISTENER 1
		INPUT FLAG					CATCHED EVENTS					MAPPED TO
FILE_NOTIFY_CHANGE_FILE_NAME	FILE_ACTION_ADDED				FSM_Created
								FILE_ACTION_REMOVED				FSM_Deleted
								FILE_ACTION_RENAMED_OLD_NAME	FSM_Moved
								FILE_ACTION_RENAMED_NEW_NAME	

FILE_NOTIFY_CHANGE_DIR_NAME		FILE_ACTION_ADDED				FSM_FolderCreated
								FILE_ACTION_REMOVED				FSM_FolderDeleted
								FILE_ACTION_RENAMED_OLD_NAME	FSM_Moved
								FILE_ACTION_RENAMED_NEW_NAME	

FILE_NOTIFY_CHANGE_SIZE			FILE_ACTION_MODIFIED			FSM_Modified
FILE_NOTIFY_CHANGE_LAST_WRITE	FILE_ACTION_MODIFIED			FSM_Modified

							LISTENER 2
FILE_NOTIFY_CHANGE_ATTRIBUTES	FILE_ACTION_MODIFIED			FSM_AttribChange
FILE_NOTIFY_CHANGE_SECURITY		FILE_ACTION_MODIFIED			FSM_AttribChange
FILE_NOTIFY_CHANGE_LAST_ACCESS	FILE_ACTION_MODIFIED			NOT CATCHED - USELESS
FILE_NOTIFY_CHANGE_CREATION		FILE_ACTION_MODIFIED			NOT CATCHED - USELESS

*/
void FSMon::ProcessNotify(FILE_NOTIFY_INFORMATION *buf, String const &path, bool second)
{
	do
	{
		switch(buf->Action)
		{
			case FILE_ACTION_ADDED :
				INTERLOCKED_(fsmMutex)
				{
					Info &info = changed.Add();
					info.flags = second ? FSM_FolderCreated : FSM_Created;
					WString ws = WString(buf->FileName, buf->FileNameLength / sizeof(WCHAR));
					info.path = AppendFileName(path, ws.ToString());
				}
				callHandlerCb();
				break;
			case FILE_ACTION_REMOVED :
				INTERLOCKED_(fsmMutex)
				{
					Info &info = changed.Add();
					info.flags = second ? FSM_FolderDeleted : FSM_Deleted;
					WString ws = WString(buf->FileName, buf->FileNameLength / sizeof(WCHAR));
					info.path = AppendFileName(path, ws.ToString());
				}
				callHandlerCb();
				break;
			case FILE_ACTION_MODIFIED :
			{
				// skip changes on folders... wathever they can mean
				WString ws = WString(buf->FileName, buf->FileNameLength / sizeof(WCHAR));
				String p = AppendFileName(path, ws.ToString());
				if(isFolder(p) && !second)
					break;
				INTERLOCKED_(fsmMutex)
				{
					Info &info = changed.Add();
					info.flags = second ? FSM_AttribChange : FSM_Modified;
					info.path = p;
				}
				callHandlerCb();
			}
			break;
			case FILE_ACTION_RENAMED_OLD_NAME :
			{
				WString ws = WString(buf->FileName, buf->FileNameLength / sizeof(WCHAR));
				String oldPath = path + ws.ToString();
				if(!buf->NextEntryOffset)
					break;
				buf = (FILE_NOTIFY_INFORMATION *)((byte *)buf + buf->NextEntryOffset);
				if(buf->Action != FILE_ACTION_RENAMED_NEW_NAME)
				{
					break;
				}
				INTERLOCKED_(fsmMutex)
				{
					Info &info = changed.Add();
					info.flags = second ? FSM_FolderMoved : FSM_Moved;
					info.path = oldPath;
					ws = WString(buf->FileName, buf->FileNameLength / sizeof(WCHAR));
					info.newPath = AppendFileName(path, ws.ToString());
				}
				callHandlerCb();
				break;
			}
			case FILE_ACTION_RENAMED_NEW_NAME :
				break;
			default :
				break;
		}
		// go to next record
		buf = (FILE_NOTIFY_INFORMATION *)((byte *)buf + buf->NextEntryOffset);
	}
	while(buf->NextEntryOffset);
}

// monitoring callback (runs in a separate thread)
void FSMon::monitorCb(void)
{
	DWORD numBytes;
	LONG descriptor;
	LPOVERLAPPED overlapped;

	// completion port is ok, start monitoring it
	while(!shutDown)
	{
		// Retrieve the directory info for this directory
		// through the io port's completion key
		if(!GetQueuedCompletionStatus(
				completionPort,
				&numBytes,
				(ULONG *)&descriptor,
				&overlapped,
				INFINITE // remember to send a completion packet to shut down thread !!
		))
		{
			// error state, if 'overlapped' is NULL, the call timed out
			// with no event, otherwise we got some error
			if(!overlapped)
			{
				// this would mean timeout, but shouldn't happen
				// even if called with a null overlapped, it sets up one
				// so this can't be used for cancel packets
				// we use numBytes parameter, using value of 1 which should
				// be never valid
			}
			else
			{
				// here we land on errors or on CancelIo operations
				int err = GetLastError();
				if(err == ERROR_OPERATION_ABORTED)
				{
					// we land here if a CancelIo was called on this handle
					// just do nothing... we don't have to respawn the I/O
					INTERLOCKED_(fsmMutex2)
					{
						int idx = monitoredDescriptors.Find(descriptor);
						if(idx >= 0)
							CancelIo(monitoredInfo[idx].hDir);
					}
				}
				else
					SetError(err);
			}
		}
		else
		{
			// just quick exit if shutting down
			if(shutDown)
				break;
			
			// get path data from descriptor
			// we use a second mutex here, it just lock adding/removing paths
			INTERLOCKED_(fsmMutex2)
			{
				int idx = monitoredDescriptors.Find(descriptor);
				if(idx >= 0)
				{
					// check flag if we wanna cancel...
					CHANGESINFO &info = monitoredInfo[idx];
					if(info.cancelling)
					{
						CancelIo(info.hDir);
						info.cancelling = false;
					}
					else
					{
						String path = monitoredPaths[idx];

						// we got an event, just parse it and fill received event structures
						ProcessNotify((FILE_NOTIFY_INFORMATION *)info.buffer, path, (idx & 1));
						
						// differentiate between the 2 handlers depending on index
						// EVEN  indexes : CREATE/DELETE/RENAME FOR FILES,   MODIFY FOR ALL
						// ODD   indexes : CREATE/DELETE/RENAME FOR FOLDERS, ATTRIBUTES FOR ALL

						// re-post the request, we don't want to loose events here....
						DWORD bufLen;
						if (!ReadDirectoryChangesW(
								info.hDir,
								info.buffer,
								READ_DIR_CHANGE_BUFFER_SIZE,
								true, // we want subdirs events
								(idx & 1) ?
									FILE_NOTIFY_CHANGE_ATTRIBUTES	|
									FILE_NOTIFY_CHANGE_SECURITY		|
									FILE_NOTIFY_CHANGE_DIR_NAME
								:
									FILE_NOTIFY_CHANGE_FILE_NAME	|
									FILE_NOTIFY_CHANGE_SIZE			|
									FILE_NOTIFY_CHANGE_LAST_WRITE,
								&bufLen, //this var not set when using asynchronous mechanisms...
								&info.overlapped,
								NULL)) //no completion routine!
							SetError(GetLastError());
					}
				}
			}
		}
	}
	
	
	// reset shutdown flag to signal tread exiting
	shutDown = false;
}

// callback to call event handler in maint thread
// (via PostCallback) when using GUI
void FSMon::callHandlerCb(void)
{
#ifdef flagGUI
	if(EventHandler)
		PostCallback(EventHandler);
#else
	EventHandler();
#endif
}

// query for changed files/folders
Vector<FSMon::Info> FSMon::GetChanged(void)
{
	Vector<Info> info;
	INTERLOCKED_(fsmMutex)
	{
		info = changed;
		changed.Clear();
	}
	return info;
}

// gets actually opened files
Index<String> FSMon::GetOpenedFiles(void)
{
	// no way on windows to catch open/close on files
	// without a dedicated filter
	return Index<String>();
}

// check error contidion and get error message
VectorMap<String, int> FSMon::GetErrorMap(void)
{
	// just return last error -- on windows we can't have
	// sequences of errors
	VectorMap<String, int> res;
	res.Add("", (int)errCode);
	return res;
}

// query IF some changes happened
bool FSMon::HasChanges(void)
{
	bool res;
	INTERLOCKED_(fsmMutex){
		res = changed.GetCount();
	}
	return res;
}

#endif

END_UPP_NAMESPACE
